# hardware/fret-board

KiCad project for the PCB that sits in the neck under the frets and carries each fret's signal back to the controller board.

**Status: not started.** Last year's fret board carried 80 lines, one for each string segment of each fret. The plan for this year is 20 full-width frets shared by all four strings, so 20 lines.

## Before starting

- [Shared-fret ghosting](../../docs/decisions/0001-shared-fret-ghosting.md) decides whether 20 plain fret lines are enough or whether the fret board needs more on it.
- [Board partition and connectors](../../docs/decisions/0010-board-partition-and-connectors.md) decides the connector and cable between this board and the controller.

## This board is mechanical as much as electrical

Its outline, thickness, fret contact positions and connector location are set by the neck. Agree those with the mechanical engineers before layout, and keep the agreed dimensions in a drawing under [`mechanical/`](../../mechanical/). Fret spacing is not uniform: each fret is closer to the next as you go up the neck.

## Who edits here

Whoever owns the fret board, working with the MEs on the outline.
