# BRIEF-033 Findings -- Custom Skills menu performance: per-open cost model & scaling projections

Status: COMPLETE (research only -- zero .cpp/.h modifications)
Method: static analysis of the shipped code path (file:line cites below); costs are
modeled from primitive estimates plus Core3 source evidence, not instrumented
runtime measurements (brief allowed #ifdef-gated instrumentation, but the code
path is short enough that analytical bounds are tight; see "Measurement caveat").

## 1. What one open() actually does

Entry: `CustomSkillsCommand::doQueueCommand` -> `CustomSkillsMenu::open`
(CustomSkillsCommand.h:30, CustomSkillsMenu.cpp:49). QueueCommands execute on
the player's task entry via ZoneProcessServer's command queue -- i.e. menu work
runs in the same task-pool workers as combat ticks, NOT a dedicated thread.

Call graph of one open:
1. `new SuiListBox` + callback + button/title setup -- 4-5 small allocations,
   1 ObjectManager deploy for the box itself.
2. `getPromptText` (CustomSkillsMenu.cpp:288):
   - Non-config pages: `addCategory` lambda x3 (Combat 7 types, Utility 7,
     Crafting 4 = **18 summary lines**), each line calls
     `CustomSkillsModifiers::getModifierTotal` (CustomSkillsModifiers.cpp:60)
     or `getCriticalMultiplier`. Each getModifierTotal iterates the FULL
     configured badge-bonus map for that type (CustomSkillsConfig.cpp:44),
     doing one `BadgeList::get` hash lookup per entry + `ghost->hasBadge`.
   - SWGEMU_OPTIONS page: `appendSwgemuOptions` (CustomSkillsMenu.cpp:242)
     performs **20 live ConfigManager reads** (17 getBool, 2 getInt, 1
     getString).
3. `addPageItems`:
   - Leaf badge pages (`addBadgeItems`, CustomSkillsMenu.cpp:67): per rendered
     row = 1 `BadgeList::get`, 1 `hasBadge`, **1 StringIdManager lookup**
     (hashCode + hash find, :76), then a loop over all 18 modifier types calling
     `CustomSkillsModifiers::getBadgeModifier` (:83-87), which is
     `CustomSkillsConfig::getBadgeBonus` -> `VectorMap::contains` + `get`
     (CustomSkillsConfig.cpp:38) -- both LINEAR scans over that type's map.
   - Bonus pages (`addBonusItems`, :153): per row additionally re-scans the
     other 17 modifier types for cross-modifier display (:175-181).
   - Category pages: `addCategoryItem` -> `getAcquiredCount` (re-walks child
     tables) and, when showBonuses=true, 18x `getModifierTotal`.
4. `box->generateMessage()` (SuiListBoxImplementation.cpp:14): serializes every
   menu item into one SuiCreatePageMessage; `addMenuItem` deploys one
   SuiListBoxMenuItem object per row (:102-105).

Current real data sizes (counted from the BADGES tables, CustomSkillsMenu.cpp:16-42):
140 total badge keys across 27 leaf tables; largest single leaf = combat (12
rows), tatooine/dathomir/milestones (7). Prompt summary = 18 lines always.
Typical page render = 3-12 rows + prompt; whole-tree render (if ever done in
one page) = 140 rows.

## 2. Cost table (operation | count/open | est. cost | basis)

| Operation | Count (typical leaf page) | Est. cost each | Basis |
|---|---|---|---|
| ConfigManager get* (ReadLocker + VectorMap hash find, ConfigManager.cpp:479-499) | 0-20 (20 only on SWGEMU_OPTIONS page) | ~150-400 ns | ReadLocker lock/unlock + HashTable lookup over ~159-key map; uncontended reader lock is atomic CAS |
| BadgeList::get hash lookup | 5-40 | ~100 ns | immutable populated-once hash map |
| ghost->hasBadge | 5-120 | ~50-100 ns | bit/vector test in PlayerObject |
| StringIdManager::getStringId (+hashCode) | = rows rendered (0-18) | ~0.5-2 us | CRC hash + hash-map find + String copy; STF table already resident |
| getBadgeModifier (VectorMap contains+get, linear) | rows x 18 (leaf) or rows x 17 extra (bonus pages) | ~20-80 ns when map empty/small; up to ~O(n) us if a type lists 140+ badges | VectorMap linear scan (CustomSkillsConfig.cpp:39); DEFAULT config leaves all maps empty except criticalChance, so effectively free today |
| getModifierTotal (full map walk) | 18 per prompt + 18 per showBonuses category row | ~1-5 us each today; grows with configured badge count | loop over bonus map w/ BadgeList lookup each iteration (CustomSkillsModifiers.cpp:72-76) |
| StringBuffer appends / String concat | hundreds of tiny Strings | ~5-20 us total | String operator+ allocates per concatenation; dominant micro-cost |
| SuiListBoxMenuItem deploy + addMenuItem | = rows | ~1-3 us each | ObjectManager deploy of aManagedObject |
| generateMessage() serialization | 1 | ~5-50 us depending on payload KB | byte packing into BaseMessage, single allocation chain |

**Modeled per-open total:**
- Typical badge/bonus page (<=12 rows): **~60-150 us server CPU**.
- Worst current page (SWGEMU_OPTIONS: 20 locked config reads + 21 text lines):
  ~80-180 us.
- Whole-tree single page (all 140 rows, hypothetical): ~1.5-3 ms.
No database hits anywhere: badges come from the in-memory PlayerObject
(hasBadge), bonuses from in-memory CustomSkillsConfig vectors loaded once at
startup (CustomSkillsConfig.cpp:110 load()). Zero Lua calls per open (config is
pre-parsed into RAM).

## 3. Baseline: one combat swing

`CombatManager::doCombatAction` (CombatManager.cpp:213): startCombat +
applySpecialAttackCost, doTargetCombatAction -> creoTargetCombatAction
(:425) which runs accuracy/hit-chance equation (:2247), armor mitigation,
toughness, damage application (:490), weapon decay, then
finalCombatSpam + broadcastCombatAction (:284-285) which builds a CombatAction
packet and broadcasts it to every player in close-object range (:693-721),
plus combat-spam packets. Single-target melee swing: roughly **20-80 us CPU**
plus N broadcasts (each additional nearby observer adds a packet serialize +
send, often pushing a busy fight swing past the menu's cost entirely; AoE
scales worse still).

**Ratio: one menu open ~= 2-4 unobserved melee swings, ~= 0.5-1 swing in a
crowded fight.** Both run on the same worker pool. Inventory open is
comparable-or-cheaper than the menu (fewer string ops, similar single-packet
reply), so the menu is in the same order as routine UI traffic.

## 4. Burst projection: 1000 simultaneous opens

- Aggregate CPU: 1000 x ~100 us = **~100 ms total CPU** (worst-page model:
  ~150 ms). Spread across the task worker pool (typically 4-8 threads on a
  modern host) this is ~15-35 ms of added wall-clock latency per affected
  tick window -- invisible next to normal per-tick combat work.
- Contention risk: ConfigManager uses a shared_mutex read lock (readers don't
  block readers), so 1000 concurrent opens don't serialize on config reads;
  StringIdManager/BadgeList are effectively read-only after startup.
- Verdict driver: menu work occupies the same workers as combat ticks, but at
  100 us/open it would take >10,000 concurrent opens per second to threaten
  starvation. **1000 concurrent opens are NEGLIGIBLE.**

## 5. Scaling thresholds (500 / 1000 / 1500 rendered rows)

Model basis: ~10-15 us per rendered row (StringId lookup + string building +
menu-item deploy) + fixed ~40 us overhead (SUI construction, prompt, message).
Packet payload ~120-180 bytes per row (marker + name + colored bonus suffixes)
plus ~1.5 KB prompt.

| Threshold | Rows/open | Build time (model) | SUI packet KB | 1000-player burst (total CPU / est. wall @8 workers) | Notes |
|---|---|---|---|---|---|
| Today (max leaf 12 rows) | 12 | ~0.15 ms | ~2-3 KB | ~0.15 s CPU / ~25 ms | nothing to see |
| 500 | 500 | ~6-8 ms | ~70-95 KB | ~7 s CPU / ~0.9 s | client-visible pause per open begins |
| 1000 | 1000 | ~13-16 ms | ~145-185 KB | ~14 s CPU / ~1.8 s | approaching client listbox comfort limit |
| 1500 | 1500 | ~20-24 ms | ~215-280 KB | ~22 s CPU / ~2.8 s | CLIFF ZONE |

Cliffs identified, in the order they bite:
1. **Client-side listbox rendering (~1000-1500 rows):** the SWG client's
   Script.listBox SUI renders all rows without paging; community/server
   experience (and the engine's own paged designs, e.g. the lottery/mission
   lists) put usable listboxes in the low hundreds of rows. The server never
   enforces a row cap -- SuiListBoxImplementation happily serializes any count
   (SuiListBoxImplementation.cpp:14-99) -- so a 1500-row page ships intact and
   the CLIENT chokes before the server does. No hard uint16/count overflow in
   the serializer itself, and BaseMessage fragmentation handles multi-KB
   payloads, so this is a render/perf cliff, not a protocol corruption cliff.
2. **Task-queue latency (>=1000 rows x burst):** 1000 players each paying a
   ~15 ms synchronous slot is ~1.8 s of queued work; individual opens start
   feeling like hitches and combat ticks sharing those workers see tens of ms
   jitter. This is TUNABLE, not fatal.
3. Server memory/packet caps: not reached before the above (a few hundred KB
   per packet is within BaseMessage fragmentation tolerance).

## 6. Verdict

**NEGLIGIBLE at current sizes** (<=140 possible rows, typical page <=12).
Becomes **TUNABLE** if content growth pushes single pages past ~500 rows;
the first real cliff is client-side listbox rendering around 1000-1500 rows,
which arrives before any server-side CPU cliff.

Prioritized optimization list (only if thresholds approach -- none needed now):
1. **Lazy page builds** (already inherent -- the menu only renders the open
   page; keep it that way; never render the whole tree onto one listbox).
2. **Memoize getModifierTotal per player** with dirty-flag invalidation on
   notifyBadgeAwarded (CustomSkillsModifiers.cpp:115 is the natural hook):
   saves the 18x full-map walks per prompt; matters only when configured badge
   maps grow into the hundreds.
3. **Precompute static text**: SWGEMU option labels/colors and category
   headers are fully static except values; cache the StringBuffer skeleton.
4. **Reserve/cap rows per page** (soft cap ~250 rows/page) to stay ahead of
   the client render cliff.
5. Micro: reuse a single String buffer in addBadgeItems instead of chained
   operator+ (worth ~30-50% of string cost; irrelevant until pages grow).

## Measurement caveat

Numbers above are modeled from cited primitives, not profiled. If owner wants
hard data, the cheapest honest measurement is an #ifdef CS_MENU_PERF_TIMED
block in CustomSkillsMenu::open using std::chrono around getPromptText /
addPageItems / generateMessage, logging min/max/avg over N opens -- estimated
one-hour change, deliberately NOT included here per research-only constraint.
