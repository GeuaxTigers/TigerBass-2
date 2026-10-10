# firmware/tests

Unit tests for [`app/`](../app/). They run on a PC with no board attached, and CI runs them on every pull request.

**Status: not started.**

## Planned layout

```text
tests/
├── fakes/        fake implementations of the app/ hardware interfaces
├── vendor/       the Unity test framework, with its licence
└── test_*.c      one test file per app/ module
```

## How a test works

A fake stands in for the hardware. A test tells the fake what the hardware will "see" over time: which fret lines read high during each scan, what each pickup channel samples. It then runs the firmware logic and checks every MIDI message that came out, and when.

## What to test

The cases that are hard to reproduce by playing:

- Two strings plucked in the same scan.
- A fret line that bounces between high and low.
- A pickup level sitting exactly at the threshold.
- The fretted note changing while the string is still ringing (hammer-on, pull-off, slide).
- A string released during the peak-detection window.
- The MIDI output queue being full.

And the invariant that matters most to a player: **no path leaves a note stuck on.** Every Note On is followed by a Note Off for the same note.

## Rules

- A failing test is information. Do not delete or disable one to get a green run. If the test is wrong, fix it and say why in the pull request.
- A bug found on the instrument gets a test that reproduces it before it gets a fix.
