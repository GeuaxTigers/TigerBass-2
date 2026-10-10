# 0001: Shared-fret ghosting

| | |
| --- | --- |
| Status | Open |
| Owner | `TBD(team)` |
| Needed by | Before fret board layout starts |
| Requirements | Q1, TB2-07, TB2-08, DC-01 |
| Analysis | Planned: `docs/analysis/shared-fret-ghosting.md` (Prompt 3) |

## The question

With 20 full-width frets shared by all four strings, can the scan always tell which fret each string is on, and if not, what do we add so that it can?

## Why it matters

The plan to go from 80 fret segments to 20 full frets is the largest change from Tiger Bass 1. It simplifies the neck, the cabling and the controller board. It also removes something the old design relied on without saying so: the fret segments under one string were electrically separate from the segments under the others.

A full-width metal fret is a conductor that all four strings can touch. Two things follow.

**Strings fretted at the same fret are shorted together through that fret.** On its own this is harmless: each string still reads the right fret.

**A string that touches two frets bridges them.** Last year's analysis assumed a fretted string contacts the fret on each side of the finger. If that is right, take the A string fretted at fret 5, touching frets 4 and 5, and the D string fretted at fret 6, touching frets 5 and 6. Drive the A string high. Frets 4 and 5 go high, as intended. But the D string also touches fret 5, so it goes high too, and carries the signal to fret 6. The scan sees frets 4, 5 and 6 high, takes the one closest to the bridge, and reports the A string at fret 6. That is one semitone wrong.

The D string does not need to be sounding for this to happen. A finger resting on the next string, one fret along, is enough.

By symmetry, driving the D string gives the same three frets. Once two strings are joined through a shared fret they are one electrical node, and a logic-level scan cannot tell which string is on which fret.

How often this matters depends on a fact we do not have: whether a fretted string on this instrument really touches the fret behind the finger. If it only touches the fret in front, two strings share a fret only when they are fretted at the same position, and the problem above does not arise.

## Options

### A. Plain shared-fret scan, and accept the failing fingerings

Twenty fret lines, the same scan as last year. Simplest hardware. Whether it is acceptable depends on how common the failing fingerings are, which the analysis and a bench test have to answer.

### B. Track the fret state over time

Scan continuously instead of only after a pluck, and remember each string's last unambiguous position. When a second finger lands and joins two strings, the firmware already knows where the first one was, so the new fret must belong to the other string. No extra hardware. It fails when two fingers land within the same scan, and it needs a rule for what to do when it loses track.

### C. Use a second source of information to break the tie

When the scan reads a run of three or more adjacent frets, the firmware knows it is in an ambiguous state. It could then use something else to decide, such as the pitch of the plucked string taken from its pickup channel. Estimating pitch takes at least a cycle or two of the note, which on a low bass string is tens of milliseconds, so this trades latency for correctness in the ambiguous case only.

### D. Keep the frets segmented, and multiplex them in the neck

Keep per-string fret segments, which cannot ghost, but put shift registers or multiplexers on the fret board so that 80 contacts leave the neck on a few wires instead of 80. This keeps the cabling benefit and gives up the mechanical one: split frets are harder to make and feel different to play. It puts active parts in the neck.

### E. Segment only some frets

Full-width frets where the failing fingerings are rare, segmented frets where they are common. A compromise in both directions. Needs the analysis to say which frets, if any, that would be.

### F. Measure something other than a logic level

For example, pass a known current along the driven string and read where along its length a fret picks off the voltage. This is a different sensing principle with its own problems and should only be explored if A to E all fail.

## How to choose

1. Note accuracy (Q1, TB2-08): how many realistic fingerings are read wrongly.
2. Polyphony (TB2-07): whether chords still work.
3. Latency (TB2-01): what the fix adds to the scan.
4. Effect on the neck: parts in the neck, how the frets are made, how it feels to play.
5. Wires between neck and body.
6. Firmware complexity, and how the fix fails when it fails.

## What we need to find out first

1. **Does a fretted string touch the fret behind the finger?** On the real neck, with a multimeter on continuity between the string and each fret, at several positions, with light and heavy finger pressure, for each string. This one measurement decides how serious the problem is.
2. **Which fingerings fail, and how common are they?** The simulator in Prompt 3 enumerates them. Someone who plays bass should then judge how often they come up.
3. **How are the strings isolated from each other on Tiger Bass 1?** On an ordinary bass the strings are joined through the metal bridge and tied to audio ground. For the scan to work each string must be electrically separate at the bridge, the nut and the tuners. Find out how last year's instrument does this and whether the new bridge will.
4. **What does the scan actually read during a ghosting case on the bench?** Two strings, one fret apart, a scope or logic analyser on the fret lines.

## Decision

_Not decided._

## Consequences

_Fill in when decided._
