# Custom Skills Menu Design

## Main page

The `/customskills` command opens the character's Custom Skills menu. Until
additional systems are implemented, the main page contains the character's
acquired badge bonuses and a single `Badges` category:

```text
Custom Skills

Accumulated Bonuses
+7% Critical Damage
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

Only the marker is colored. Omit the suffix for badges without configured
bonuses and list multiple bonuses in stable modifier display order.

Multiple Custom Skills windows remain supported intentionally.
