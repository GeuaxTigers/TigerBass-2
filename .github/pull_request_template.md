<!--
Title: an area prefix and a plain sentence, for example
  hw: add input buffer to pickup channel 1
Prefixes: hw, fw, mech, docs, tools, ci
-->

## What changed

<!-- Two or three sentences a teammate can read without opening the diff. -->

## Why

<!-- The problem this solves. Link the issue (Closes #12) and any decision record. -->

## How it was checked

<!-- Evidence, not a claim. Paste command output, ERC/DRC counts, measurements, screenshots. -->

## Assumptions

<!-- Anything you took as true without confirming it. Write "None" if none. -->

## Questions for the team

<!-- Open questions, including every TBD(team) this adds. Write "None" if none. -->

## Not done

<!-- Anything in scope that you left out, and why. Write "None" if none. -->

## Checklist

- [ ] The changed-files list contains only what I meant to change
- [ ] No KiCad backups, autosaves or lock files, and no build output
- [ ] Documentation is updated where this changes how something works

**Hardware** (delete this section if not hardware)

- [ ] I claimed the KiCad file on the issue before editing
- [ ] ERC run: 0 errors, or each remaining one explained above
- [ ] DRC run: 0 errors, or each remaining one explained above
- [ ] Board updated from schematic, so the two agree
- [ ] New parts have a manufacturer part number and a datasheet link
- [ ] New footprints checked against the datasheet land pattern (page cited above)

**Firmware** (delete this section if not firmware)

- [ ] Host tests pass (`ctest` output above)
- [ ] New behaviour has a test
- [ ] Nothing added to the scan path waits or blocks
- [ ] If it ran on the board: what was measured is recorded above

**Mechanical** (delete this section if not mechanical)

- [ ] STEP export and drawing updated together
- [ ] Dimensions shared with the electronics still match the current board outline
