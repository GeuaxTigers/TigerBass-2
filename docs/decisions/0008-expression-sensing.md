# 0008: Scroll wheel and whammy bar sensing

| | |
| --- | --- |
| Status | Open |
| Owner | `TBD(team)`. Needs one electrical and one mechanical owner |
| Needed by | Before the body design fixes where the controls sit |
| Requirements | Q4, TB2-02, TB2-03 |
| Analysis | None planned yet |

## The question

How does the instrument sense the position of the scroll wheel and the whammy bar, and which MIDI messages do they send?

## Why it matters

These two controls are new this year and are as much mechanical as electrical. The sensor type decides the mechanism the mechanical engineers build, the signals the controller board has to read, and how the control feels. They are also the most visible new features at a demo.

Tiger Bass 1 had five potentiometers read by the microcontroller's ADC: octave, modulation, pitch bend, MIDI channel and threshold.

## Start with what each control is

**Scroll wheel.** Does it spring back to a rest position, or stay where it is left? A keyboard's modulation wheel stays where it is left. Is it endless, or does it have end stops?

**Whammy bar.** Is it a real vibrato bridge that changes the strings' pitch, with a sensor added, or a lever that only sends MIDI and leaves the strings alone? A real vibrato bridge changes the acoustic bass sound too, and moves the bridge the string drive wires and pickups attach to. A MIDI-only lever is much simpler. Does it bend down only, or both ways around a centre?

`TBD(team)` for all of these. The sensor follows from the answers.

## Sensor options

These apply to either control.

### A. Potentiometer

A voltage divider read by an ADC. Simple, absolute, cheap. It has a wiper that wears, limited rotation, and it needs a mechanical link to the control.

### B. Hall-effect sensor and magnet

A magnet on the moving part and an analog Hall sensor on the board or a small daughterboard. No contact and no wear, and the sensor can be sealed inside the body. Output is not linear with position, so it needs calibration, and stray fields from the bass's own pickup magnets have to be checked.

### C. Incremental rotary encoder

Counts steps in each direction. Endless rotation, digital, no ADC needed. It reports movement, not position, so it has no memory of where it is at power-up. Suits an endless scroll wheel. Does not suit a spring-return lever.

### D. Strain gauge or force sensor

Senses how hard the bar is pushed instead of how far it moves. No moving parts. Needs an amplifier, drifts with temperature, and gives the player no travel to feel.

## MIDI mapping

- **Pitch bend** is its own 14-bit message, with a centre value meaning no bend. How many semitones full deflection means is set at the receiving synth, commonly two semitones each way by default. So "range" here means how much of the 14-bit span the bar's travel covers.
- **Modulation** is normally Control Change 1, 7 bits.
- Decide whether "vibrato" means sending modulation and letting the synth produce the vibrato, or the instrument itself sending a wobbling pitch bend.

## How to choose

1. The feel the sponsor wants from each control.
2. Returns reliably to the same rest value, so pitch does not sit slightly bent (TB2-03).
3. Resolution fine enough that a bend sounds smooth and not stepped.
4. Survives being played: no wear-out, nothing that snags.
5. Fits the body (TB2-05) and can be built with the tools the mechanical engineers have.
6. Does not pick up interference from, or cause interference in, the pickups.
7. Electrical cost: ADC channels, pins, supply.

## What we need to find out first

1. **Ask the sponsor** what each control should feel like, with reference to an instrument or controller the sponsor already knows.
2. **Decide whether the whammy bar moves the strings.**
3. **Sketch each mechanism** with its travel and where the sensor sits.
4. **Bench-test a candidate sensor** next to the bass's magnetic pickup, for interference in both directions.
5. **Decide whether these go through the external ADC** ([0002](0002-external-adc.md)) or the microcontroller's own.
6. **Decide what happens to last year's other controls:** octave, MIDI channel and threshold.

## Decision

_Not decided._

## Consequences

_Fill in when decided._
