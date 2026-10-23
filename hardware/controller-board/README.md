# hardware/controller-board

KiCad project for the TigerBass 2 controller PCB: the microcontroller, the external ADC and its front end, the string drive, USB, power, and the connectors to everything else in the instrument.

**Status: schematic started (KiCad 9).** The ADC and pickup front end are drawn on one flat sheet. There is no layout yet. See the [schematic review](../../docs/analysis/controller-board-schematic-review.md) for what is missing. The KiCad project goes in this folder, with the project file, root schematic and board file sharing one name, for example `controller-board.kicad_pro`, `controller-board.kicad_sch`, `controller-board.kicad_pcb`. CI finds projects by their `.kicad_pro` file.

## Before starting the schematic

These decisions shape the board and are still open. Each has a record in [`docs/decisions/`](../../docs/decisions/).

- [Shared-fret ghosting](../../docs/decisions/0001-shared-fret-ghosting.md)
- [External ADC](../../docs/decisions/0002-external-adc.md)
- [Amplitude front end](../../docs/decisions/0003-amplitude-front-end.md)
- [Microcontroller](../../docs/decisions/0004-microcontroller.md)
- [Battery and power tree](../../docs/decisions/0009-battery-and-power-tree.md)
- [Board partition and connectors](../../docs/decisions/0010-board-partition-and-connectors.md)

## Conventions

- One hierarchical sheet per function (power, MCU, ADC front end, string drive, USB, MIDI out, connectors). Last year's project used the same approach with `Power`, `midi_out` and `amp_detection` sheets.
- Name nets for what they carry: `FRET_07`, `STRING_DRV_2`, `PICKUP_3_RAW`. Avoid net names that are only pin names.
- Add a test point for every power rail and for each signal named in the bring-up checklist.
- Board revisions are letters: `revA`, `revB`. Put the revision in the title block and on the silkscreen.

## Who edits here

The EE and ECE members doing schematic capture and layout. Check the issue for the sheet you want to change before you open it.
