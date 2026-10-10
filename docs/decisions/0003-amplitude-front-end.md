# 0003: Amplitude front end

| | |
| --- | --- |
| Status | Open |
| Owner | `TBD(team)` |
| Needed by | Before the controller schematic's analog sheet |
| Requirements | F2, Q3, M4, TB2-14 |
| Analysis | Planned: `docs/analysis/adc-trade-study.md` section 3 (Prompt 5) |

## The question

What circuit sits between each pickup channel and the ADC?

## Why it matters

The pickup signal is AC, swings both ways around zero, varies widely in size between a soft and a hard pluck, and may come from a high-impedance source. The ADC wants a signal inside its input range, from a source that can drive it. The front end bridges the two, protects the ADC, and sets the quietest pluck the instrument can detect (M4).

## What Tiger Bass 1 did

As built, per string: a 1 kΩ series resistor, a clamp diode to ground, a clamp diode to a node at about 3 V, and a series diode into the ADC pin. It is passive, cheap and protects the pin.

Its limits: the series diode passes only the positive half of the signal and only the part above its forward voltage, so quiet plucks are lost. Nothing buffers the pickup, so the pickup has to drive the ADC's sampling capacitor through the resistor and diode.

Last year's report planned something different, an op-amp precision rectifier, which does not appear on the board. See [`../legacy/tigerbass1-notes.md`](../legacy/tigerbass1-notes.md).

## Options

### A. Keep the passive clamp and series diode

No new design work. Keeps the sensitivity floor.

### B. Buffer and bias, digitise the raw waveform

An op-amp buffer presents a high impedance to the pickup and a low one to the ADC. The signal is biased to the middle of the ADC range so both halves are captured. Rectifying, peak detection and thresholds are all done in firmware, where they can be changed without a new board. Needs an ADC fast enough to sample the waveform, and a clean mid-rail reference.

### C. Rectify and hold the peak in analog

A precision rectifier and a peak-hold or envelope stage, so the ADC reads a slowly changing level. The ADC can be slower. The time constants are fixed by components, and the hold has to be reset or allowed to decay between notes.

### D. Charge amplifier

If the pickups are piezo elements, they are capacitive sources and a charge amplifier is the standard way to read them: output is independent of cable capacitance and the low-frequency response is set by the feedback network. Only applies to piezo pickups.

### E. Analog comparator for onset, plus ADC for level

A comparator per string flags the instant the signal crosses a threshold and interrupts the microcontroller. The ADC is then used only to measure how hard the pluck was. Separates the fast decision (a string was plucked) from the accurate one (how hard).

## How to choose

1. The quietest pluck that must register (M4), and the loudest that must not clip.
2. Delay added between the pluck and a usable reading (TB2-01).
3. Protection: nothing the pickup can produce may exceed the ADC's absolute maximum ratings (Q3). Piezo elements can produce large spikes when struck.
4. Whether thresholds and time constants can be tuned after the board is built.
5. Parts count times four channels, and supply current.
6. What supply rails it needs: a single supply, or a negative rail too ([0009](0009-battery-and-power-tree.md)).

## What we need to find out first

1. **Which pickup, and its electrical character:** output impedance, signal size, and whether it has its own preamp. Datasheet first, then a scope on the real part.
2. **Scope captures of soft and hard plucks on every string**, through last year's front end and directly at the pickup, to see what the old circuit throws away.
3. **Crosstalk:** pluck one string and look at the other three channels. If a hard pluck on one string crosses the threshold on its neighbour, the front end or the firmware has to deal with it.
4. Simulate the candidate circuits in LTspice with the measured waveform as the source, in [`hardware/sim/`](../../hardware/sim/).

## Decision

_Not decided._

## Consequences

_Fill in when decided._
