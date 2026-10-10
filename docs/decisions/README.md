# Design decisions

One file per decision. Each records the question, the options we looked at, what we chose, and why.

The point is that in March, when someone asks "why did we pick this ADC?", the answer is in a file with the evidence linked, instead of in the memory of whoever was at the meeting.

## Index

| No. | Decision | Status | Blocks |
| --- | --- | --- | --- |
| [0001](0001-shared-fret-ghosting.md) | Shared-fret ghosting | Open | Fret board, controller schematic, scan firmware |
| [0002](0002-external-adc.md) | External ADC | Open | Controller schematic, ADC driver |
| [0003](0003-amplitude-front-end.md) | Amplitude front end | Open | Controller schematic |
| [0004](0004-microcontroller.md) | Microcontroller | Open | Controller schematic, all of `firmware/bsp` |
| [0005](0005-latency-target.md) | Latency target | Open | Test plan, ADC and scan design |
| [0006](0006-usb.md) | USB connector and stack | Open | Controller schematic, enclosure cutout, USB firmware |
| [0007](0007-din-midi-out.md) | DIN MIDI out | Open | Controller schematic, enclosure cutout |
| [0008](0008-expression-sensing.md) | Scroll wheel and whammy bar sensing | Open | Mechanisms, controller schematic |
| [0009](0009-battery-and-power-tree.md) | Battery and power tree | Open | Controller schematic, battery mount |
| [0010](0010-board-partition-and-connectors.md) | Board partition and connectors | Open | Both board outlines, cabling |
| [0011](0011-tool-versions.md) | KiCad and CubeIDE versions | Open | Everyone's setup |
| [0012](0012-mechanical-cad-location.md) | Where mechanical CAD lives | Open | Mechanical workflow |
| [0013](0013-licence.md) | Licence | Open | Making the repo public |

Suggested order: 0011 first, because it costs nothing and unblocks everyone. Then 0001, because it decides whether the fret board plan stands. Then 0005, 0002, 0003 and 0004 together, because each constrains the others.

## Status values

| Status | Meaning |
| --- | --- |
| Open | Not decided. The file lists options and what we need to learn |
| Proposed | Someone has written a recommendation and it is waiting for the team |
| Decided | Chosen. The file says what and why |
| Superseded | Replaced by a later decision, which it links to |

## How to use these

**To work on an open decision:** do the measurements or analysis listed under "What we need to find out first", put the results in [`docs/analysis/`](../analysis/) or [`docs/test/`](../test/), and link them from the decision file.

**To record a decision:** in one pull request, fill in the "Decision" and "Consequences" sections, set the status to Decided with the date, update the index above, and update the rows it settles in [`docs/requirements.md`](../requirements.md) and [`docs/architecture.md`](../architecture.md).

**To change a decision later:** do not rewrite the old file. Add a new one that says what changed and why, and mark the old one Superseded. The record of having changed our minds, and the reason, is worth keeping.

**To add a decision:** copy [`0000-template.md`](0000-template.md), take the next number, and add it to the index.

The options and criteria in the open files were written from general engineering knowledge and from the notes on Tiger Bass 1, to give the team a starting point. They are not recommendations. Add options, strike ones that do not apply, and reorder the criteria.
