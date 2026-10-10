# Requirements

What TigerBass 2 must do, one requirement per row. Each has an ID so that decisions, tests and results can refer to it.

**This table is a starting point, not an agreed specification.** It was assembled from last year's design report and this year's project goals. The team, sponsor and advisor have not signed off on it. Every `TBD(team)` is a number or a choice nobody has made yet.

## How to read the table

| Column | Meaning |
| --- | --- |
| ID | `F`, `Q` and `M` are last year's functions, qualitative constraints and measurable specifications, with their original numbers. `TB2-` are this year's additions. `DC-` are design constraints: choices already made about how, not what |
| Value | The number we are held to. `TBD(team)` if not set |
| Source | Where the requirement comes from |
| Verify by | **Test**: measure it. **Analysis**: calculate or simulate it. **Inspection**: look at it. **Demonstration**: show it working |
| Status | **Carried over**: unchanged from last year. **Under review**: last year's version probably does not apply as written. **New**: from this year's goals. **Proposed**: implied by the goals but not stated by anyone yet, so the team needs to accept, change or drop it |

Sources:

- **FDPR** is the Project 68 Final Design Project Report, 2024–25. It is not in this repository yet; section numbers refer to that report. What we extracted from it is in [`legacy/tigerbass1-notes.md`](legacy/tigerbass1-notes.md).
- **Goals** are the project goals in the [README](../README.md#what-we-are-building).

## Functions

What the instrument does. From FDPR section III.C, Table "Required Functions".

| ID | Requirement | Value | Source | Verify by | Status |
| --- | --- | --- | --- | --- | --- |
| F1 | Detect the signals from the strings and fretboard that identify which note is being fretted | Every fret position on every string | FDPR III.C | Test | Carried over |
| F2 | Measure how hard each string was plucked, per string | Velocity range `TBD(team)` | FDPR III.C | Test | Carried over |
| F3 | Convert the analog pickup signals to digital | Resolution and rate `TBD(team)`, see [decision 0002](decisions/0002-external-adc.md) | FDPR III.C | Analysis, test | Carried over |
| F4 | Build MIDI messages from the detected note and amplitude and send them to a synthesizer or computer | Note On, Note Off with velocity | FDPR III.C | Test | Carried over |
| F5 | Run from a battery | FDPR: "the 9 V battery already in the guitar". This year: see TB2-04 | FDPR III.C | Test | Under review |
| F6 | Still work as a normal bass when the MIDI side is not in use | Audio output unaffected with the controller off | FDPR III.C | Demonstration | Carried over |

## Qualitative constraints

How well it does it. From FDPR section III.D. The weights are last year's and came from the sponsor's priorities.

| ID | Requirement | Weight | Source | Verify by | Status |
| --- | --- | --- | --- | --- | --- |
| Q1 | Note detection is accurate, with no ghost notes | 0.30 | FDPR III.D | Test | Carried over |
| Q2 | Common bass playing techniques are detected and reproduced | 0.30 | FDPR III.D | Demonstration | Carried over. Which techniques is `TBD(team)`, see TB2-10 |
| Q3 | The electronics are safe for the player and for the components | 0.20 | FDPR III.D | Analysis, test | Carried over |
| Q4 | Basic MIDI controls are provided. FDPR: pitch wheel, mod wheel, octave control | 0.10 | FDPR III.D | Demonstration | Under review, see TB2-02 and TB2-03 |
| Q5 | It looks and feels like a normal string instrument | 0.10 | FDPR III.D | Inspection | Carried over, see TB2-05 |

## Measurable specifications

From FDPR section III.E.

| ID | Requirement | Value | Source | Verify by | Status |
| --- | --- | --- | --- | --- | --- |
| M1 | Budget | $1000 | FDPR III.E | Inspection of the BOM and orders | Carried over, see TB2-06 |
| M2 | Latency from note detected to MIDI packet sent | 8 ms maximum | FDPR III.E | Test | Under review, see TB2-01 |
| M3 | I/O the controller must handle | About 50 ports | FDPR III.E | Analysis | Under review. With 20 shared fret lines the count drops. See [decision 0004](decisions/0004-microcontroller.md) |
| M4 | Minimum signal amplitude that counts as a note | "x dB". Never given a value in the FDPR | FDPR III.E | Test | Under review. `TBD(team)` |
| M5 | Supply voltage | 9 V | FDPR III.E | Test | Under review, see [decision 0009](decisions/0009-battery-and-power-tree.md) |

## This year's goals

| ID | Requirement | Value | Source | Verify by | Status |
| --- | --- | --- | --- | --- | --- |
| TB2-01 | Note-on latency is lower than Tiger Bass 1 | `TBD(team)`. Last year's target was 8 ms and its predicted worst case was 4.84 ms. No measured figure for the finished instrument is in our notes. See [decision 0005](decisions/0005-latency-target.md) | Goals | Test | New |
| TB2-02 | A scroll wheel controls vibrato / modulation | MIDI message and range `TBD(team)`, see [decision 0008](decisions/0008-expression-sensing.md) | Goals | Demonstration | New |
| TB2-03 | A whammy bar controls pitch bend | Range and resolution `TBD(team)`, see [decision 0008](decisions/0008-expression-sensing.md) | Goals | Demonstration | New |
| TB2-04 | The battery is integrated into the instrument | Run time `TBD(team)`. Last year's estimate was about 3 hours. See [decision 0009](decisions/0009-battery-and-power-tree.md) | Goals | Test | New |
| TB2-05 | The electronics are designed into the body: a sleeker instrument than Tiger Bass 1 | What "sleeker" means in measurable terms is `TBD(team)`: for example no exposed wiring, a closed cavity, a maximum added weight | Goals | Inspection | New |
| TB2-06 | Budget | $1000, preliminary | Goals | Inspection of the BOM and orders | New |

## Design constraints

Choices already made about how the instrument is built. They are listed here so tests and decisions can refer to them.

| ID | Constraint | Source | Status |
| --- | --- | --- | --- |
| DC-01 | Fret sensing uses 20 full-width frets shared by all four strings, replacing 80 per-string fret segments | Team design direction | Direction chosen. Depends on [decision 0001](decisions/0001-shared-fret-ghosting.md) |
| DC-02 | The pickup channels are measured by an ADC separate from the microcontroller | Team design direction | Direction chosen. Part is [decision 0002](decisions/0002-external-adc.md) |
| DC-03 | The controller board is a new design that reuses parts of the Tiger Bass 1 design | Team design direction | Direction chosen |
| DC-04 | Four strings, standard bass tuning E1 A1 D2 G2 | Tiger Bass 1 as built | `TBD(team)`: confirm alternate tunings are out of scope |

## Proposed

Nobody has stated these, but the goals imply them or a test plan will need them. For each one the team should accept it and give it a value, change it, or delete it.

| ID | Requirement | Value | Why it is proposed | Verify by |
| --- | --- | --- | --- | --- |
| TB2-07 | Any combination of the four strings can sound at once, each reported as its own note | 4 simultaneous notes | Last year tested chords. The shared-fret change puts this at risk ([decision 0001](decisions/0001-shared-fret-ghosting.md)) | Test |
| TB2-08 | The reported note is the fretted note | Wrong-note rate `TBD(team)` over a defined test sequence | Makes Q1 testable | Test |
| TB2-09 | Every Note On is followed by a Note Off for the same note. No stuck notes | 0 stuck notes over a defined test sequence | A stuck note is the failure a player notices most | Test |
| TB2-10 | A defined list of playing techniques is supported | List `TBD(team)`. Candidates: open strings, hammer-on, pull-off, slide, muted notes, slap | Makes Q2 testable | Demonstration |
| TB2-11 | The instrument appears to a computer as a standard USB MIDI device with no driver to install | Operating systems `TBD(team)` | Last year sent MIDI through a HID-class call. See [decision 0006](decisions/0006-usb.md) | Demonstration |
| TB2-12 | Current through the player from an energised string stays below the level a person can feel | Limit `TBD(team)`. Last year calculated 0.016 mA worst case at 3.3 V and cited 3 mA as the threshold of sensation | Makes Q3 testable. The limit needs a cited source | Analysis, test |
| TB2-13 | The battery is protected against over-charge, over-discharge, short circuit and over-temperature | Per the cell's datasheet | Makes Q3 testable for an integrated battery | Inspection, test |
| TB2-14 | Velocity tracks how hard the string is plucked | Number of distinguishable levels `TBD(team)` | Makes F2 testable | Test |

## Open questions about the requirements themselves

1. Who approves this table: the team, the sponsor, the advisor, or all three?
2. M2 measured latency from "note detected" to "packet sent". A player feels pluck to sound, which also includes the time for the pickup signal to rise and whatever the computer adds. Which interval is TB2-01 about, and where are its two endpoints measured?
3. F6 says the bass still works as a normal bass. Does the normal audio signal pass through the controller board at all, or is it a separate path from the magnetic pickup to the output jack?
4. Is DIN MIDI out a requirement or a convenience? See [decision 0007](decisions/0007-din-midi-out.md).
5. Last year's controls included octave, MIDI channel and threshold. Which of those stay?
