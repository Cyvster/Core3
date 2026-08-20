# Custom Skills Menu Design

## Main page

The `/customskills` command opens the character's Custom Skills menu. The main
page contains the character's enabled, acquired modifier totals and a single
`Badges` category:

```text
Custom Skills

Stat Summary
+7% Critical Chance
+4 Melee Defense
+2% Movement Speed

Badges
```

The accumulated values are bonuses currently granted to the character. The
menu must never display or include the total bonuses that would be possible if
the character acquired every badge.

## Badge hierarchy

```text
Badges
  Milestone Badges
  Exploration
    Milestone Exploration
    Corellia
    Dantooine
    Dathomir
    Endor
    Lok
    Naboo
    Rori
    Talus
    Tatooine
    Yavin IV
  Profession
  Quest
  Event
```

Milestones use ascending threshold order. Exploration planets and badges
within a planet use alphabetical display-name order. Profession, quest, and
event groups use explicit stable display order where progression matters.
Badge CRC values and table indices must not control presentation order.

## Acquired totals

Every category page displays the accumulated bonuses granted by acquired
descendant badges. Aggregation is recursive: a planet totals its badges,
Exploration totals its milestones and planets, and Badges totals every badge
category. Unacquired badges never contribute to these totals, and no
`total possible` value is displayed.

Every category entry appends its acquired descendant-badge count. Counts are
recursive and character-specific, for example `Badges (20)`,
`Exploration (12)`, and `Rori (4)`. The possible badge count is not displayed.

Totals must come from the same modifier service used by gameplay so stacking,
caps, and rounding cannot disagree with the effective character bonuses.

## Badge rows

Badge pages continue to show both acquired and unacquired badges. Prefix each
row with only a colored ownership marker: a green capital `O` when acquired or
a red capital `X` when unacquired. Reset the color before the badge name; do
not include a legend or the words `Acquired` and `Not Acquired`.

Append the badge's configured bonuses after its display name:

```text
O  Ben Kenobi's Old Home  (+1% Critical Damage)
X  Pool Beneath Fort Tusken  (+1% Critical Damage)
```

The marker uses its ownership color and each configured modifier suffix uses
the module summary color. Omit the suffix for badges without configured bonuses
and list multiple bonuses in stable modifier display order.

Multiple Custom Skills windows remain supported intentionally.

## First effective modifier

By default, each of the twelve ground combat profession mastery badges grants
300 basis points (3.00%) of Critical Chance. The default maximum is 36.00%.
Successful critical attacks use a default 150.00% critical multiplier.
Critical hits temporarily produce a compact `(CRIT)` development label. Native
damage spam and its default color remain untouched. The label will be removed
after development because the stock client cannot append it to the damage line.
The shared modifier service supplies both combat and every menu summary.
The offense summary always lists base custom combat values such as Critical
Multiplier, even when the character has no acquired badge contribution. A
disabled mechanic is omitted from the active summary.

Modifier behavior is loaded once from `bin/scripts/customskills/config.lua`.
Every mechanic can be enabled independently and assigned a uniform per-badge
value and badge-key list without editing C++. Critical Chance additionally
configures its multiplier and development label. A server restart reloads the
configuration.

## Stock-client controls

The stock `Script.listBox` sends a server event when a player submits a button,
not when the selected row changes. Therefore OK opens the selected subpage,
Back returns one level, and only Cancel exits. Navigable category pages use an
OK button. Informational badge-list pages label that button Refresh; submitting
it or submitting without a valid selection reopens the current page.

Single-click navigation, right-click Back, and reliable double-click submission
require a client-side SUI script/template modification and are deferred to the
client-mod phase.
