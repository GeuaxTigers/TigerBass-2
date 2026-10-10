# mechanical

The instrument's physical design: the body and its electronics cavities, the neck and fret board fit, the mounts for the controller board and battery, and the scroll wheel and whammy bar mechanisms.

**Status: not started.** Where the native CAD files live is an [open decision](../docs/decisions/0012-mechanical-cad-location.md). Until it is settled, commit exports here.

## What to commit

| File type | Why |
| --- | --- |
| STEP (`.step`) of each part and of the full assembly | Anyone can open it, and KiCad can import it to check a board against the enclosure |
| PDF drawings with dimensions and tolerances | What gets machined or printed, and what the board outline is checked against |
| A short `README.md` per sub-assembly | What it is, what material, how it is made, what it mounts to |

Name files with a revision: `controller-mount-revA.step`. Git cannot show a diff of a CAD file, so the name and the commit message are the only record of what changed. Write the commit message accordingly: "Move USB cutout 3 mm toward the bridge to clear the strap button".

## The mechanical and electrical interface

These dimensions are shared with the electronics and need both sides to agree before either commits to them. Keep the agreed values in one drawing here and link it from the board READMEs.

- Controller board outline, mounting hole positions, maximum component height on each side.
- Fret board outline, thickness, and the position of each fret contact.
- Positions of every connector that passes through the body: USB, MIDI out, charging.
- Battery size and how it is retained and removed.
- Where the scroll wheel and whammy bar sensors sit, and their range of travel.
- Cable routes between the neck, the pickups and the controller board.

## Who edits here

The mechanical engineers. Electrical members: export the board as STEP from KiCad (File → Export → STEP) and hand it over whenever the outline or a tall component changes.
