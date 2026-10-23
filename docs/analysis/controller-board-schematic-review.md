# Controller board schematic review, 23 Oct 2026

A review of `hardware/controller-board/controller-board/controller-board.kicad_sch` as committed on 18 Oct 2026 (`f08f452`), compared with Tiger Bass 1 and with the open decisions. It feeds [0002](../decisions/0002-external-adc.md), [0003](../decisions/0003-amplitude-front-end.md) and [0009](../decisions/0009-battery-and-power-tree.md). Like every analysis document here, it lists findings and options. It does not settle any decision.

## What is on the sheet

One flat sheet containing the pickup front end and the external ADC. It implements option B of [0003](../decisions/0003-amplitude-front-end.md): buffer and bias, then digitise the raw waveform.

| Ref | Part | Job |
| --- | --- | --- |
| U3 | ADS131M04 (TSSOP-20) | 4-channel simultaneous-sampling delta-sigma ADC |
| U1 | TLV9064, units A–D | One unity-gain buffer per pickup channel |
| U2 | TLV9064, unit A | Buffers the 1.65 V mid-rail bias, `1V65Bias` |
| U2 | TLV9064, units B–D | Placed, not connected |
| R9, R10, C1, C2 | 10 kΩ, 10 kΩ, 10 µF, 100 nF | `3V3A` / 2 divider into U2A |
| R1–R4, R11–R14 | 100 kΩ series, 1 MΩ to bias | Pickup input network, one pair per channel |
| R5–R8, R15–R18, C3–C6 | 1 kΩ, 1 kΩ, 10 nF | Differential RC between each buffer and the ADC pair |
| C7 | 220 nF | ADS131M04 `CAP` pin |

Signal path, channel 1 (the other three are the same): pickup → R1 100 kΩ → U1A + input, with R11 1 MΩ from that node to `1V65Bias` → U1A follower → R5 1 kΩ → `AIN0P`. `AIN0N` goes through R15 1 kΩ to `1V65Bias`. C3 10 nF sits across `AIN0P`–`AIN0N`.

## Method

KiCad 9 is not installed where this review was done. The netlist was rebuilt from the schematic file directly: symbol pin positions from each library symbol, transformed by the symbol's position and rotation, then joined through wire end points, junctions and labels. Every one of the 21 pins it found unconnected matched the parts listed under "Not drawn yet", which checks the method. Please confirm the findings with ERC in KiCad 9.

## Calculations

| Quantity | Value | Working |
| --- | --- | --- |
| Bias voltage | 1.65 V | 3.3 V × 10 k / (10 k + 10 k) |
| Bias filter corner | ≈ 3.2 Hz | 5 kΩ Thevenin × 10.1 µF |
| Input attenuation | 0.909 | 1 M / (100 k + 1 M) |
| Load seen by the pickup | 1.1 MΩ to 1.65 V | R1 + R11 |
| Anti-alias corner, differential | ≈ 8.0 kHz | 1 / (2π × 2 kΩ × 10 nF) |
| ADC full scale, gain 1 | ±1.2 V differential | Internal 1.2 V reference |
| Usable swing at `AINxP` | 0.45 V to 2.85 V | 1.65 V ± 1.2 V. TLV9064 is rail-to-rail on 3.3 V |
| Gain error from the 1 kΩ series resistors | ≈ 0.3 % per leg | 1 kΩ against 330 kΩ ADC input impedance at gain 1. Equal on both legs, so it barely matters |
| Input current, 50 V spike on the pickup | ≈ 0.5 mA | Through R1 100 kΩ into the op-amp input clamp. Check against the TLV9064 input current limit |

## Findings

### Will not work as drawn

1. **DC bias of the input depends on how the pickup is referenced.** There is no coupling capacitor. If the pickup's return is ground, the U1 input sits at 1.65 V × 100 k / 1.1 M ≈ 0.15 V, not mid-rail, and negative half-cycles clip at the op-amp's lower rail. The pickup's return has to go to `1V65Bias`, or a series capacitor has to be added before R1.
2. **ADS131M04 `DVDD` (pin 20) is not connected.** The ADC will not run without it.
3. **ADS131M04 `CLKIN` (pin 17) is not connected.** The part has no internal oscillator and needs an external clock, nominally 8.192 MHz. It can come from an MCU timer output or a separate oscillator. The choice depends on [0004](../decisions/0004-microcontroller.md).

### Should fix before layout

1. **No decoupling at the op-amps.** Neither U1 nor U2 has a capacitor on V+. Add 100 nF at each.
2. **No decoupling at the ADC supply pins.** C1 and C2 filter the bias divider, not `AVDD`. `AVDD` and `DVDD` each need their own capacitors next to the pins. Take the values from the power-supply section of the ADS131M04 datasheet (typically 1 µF plus 100 nF per pin).
3. **U2 units B–D are floating.** Unused CMOS op-amps with floating inputs can oscillate and draw extra current. Choose one:
   - **Tie them off** as followers with + to `1V65Bias`.
   - **Use a single op-amp for the bias** (TLV9061) instead of a quad.
   - **Keep the quad and use the spare units as gain stages**, if the pickup level turns out too small. That decision waits on the pickup measurements in [0003](../decisions/0003-amplitude-front-end.md).
4. **Signal nets have no names.** The [controller-board README](../../hardware/controller-board/README.md) asks for names like `PICKUP_1_RAW`. Suggested labels: `PICKUP_n_RAW` at the input, `PICKUP_n_BUF` at each follower output, and `ADC_AINnP` / `ADC_AINnN` at the ADC.

### Depends on the pickup choice

1. **The 1.1 MΩ load is fine for a magnetic pickup but too low for a piezo.** A piezo element is a capacitor. With a typical 1 nF piezo, 1.1 MΩ forms a high-pass filter at 1 / (2π × 1.1 MΩ × 1 nF) ≈ 145 Hz, well above the low E at 41 Hz. A piezo needs about 10 MΩ or more, or a charge amplifier (option D in [0003](../decisions/0003-amplitude-front-end.md)). Last year's report, flowchart and budget disagree on which pickup was used. See [the legacy notes](../legacy/tigerbass1-notes.md).
2. **No gain.** The buffers are unity gain and the input network attenuates by 0.909. Whether that is enough depends on the pickup's output level. Last year's diode front end lost quiet plucks below a diode drop. This design keeps the whole waveform, which is the main improvement, but the quietest pluck still has to be measured.

### Not drawn yet (expected at this stage)

- The other ends of R1–R4, and the pickup connector. Last year's board used a 2×4 shrouded header with ground on the opposite row.
- The ADC's digital interface to the MCU: `SCLK`, `DIN`, `DOUT`, `~CS`, `~DRDY`, `~SYNC/~RESET`.
- The source of `3V3A`. Last year fed VDDA through a 120 Ω ferrite bead with its own decoupling. See [0009](../decisions/0009-battery-and-power-tree.md).
- Input protection. R1's 100 kΩ already limits current into the op-amp, so the series resistor may be all the protection a magnetic pickup needs. For a piezo, add clamp diodes to the rails after R1.

### Housekeeping

- Everything is on one flat sheet. The README's convention is one hierarchical sheet per function. Moving this to an `adc.kicad_sch` sub-sheet is easiest now, before the MCU and power sheets exist.
- The `AINxN` resistors are numbered out of order (R15, R7, R16, R18 for channels 0–3). Re-annotate before layout so the BOM reads cleanly.
- The file was saved by KiCad 9.0, and KiCad 8 cannot open it. In practice that settles the KiCad half of [0011](../decisions/0011-tool-versions.md) as KiCad 9. Record the decision so nobody saves from an older version.

## Latency note for 0005

The ADS131M04 data rate is set by its oversampling ratio. With an 8.192 MHz clock, the default OSR of 1024 gives 4 kSPS, and OSR 128 gives 32 kSPS. The digital filter takes about three output periods to settle after a step, roughly 0.75 ms at 4 kSPS or 0.1 ms at 32 kSPS. The analog 8 kHz RC is not the limit. Choose the OSR alongside [0005](../decisions/0005-latency-target.md). Check the filter settling figures in the datasheet before using them in the latency budget.

## What this review does not tell us

- Whether the pickup's signal level suits a unity-gain front end. Only a scope on the chosen pickup can say.
- ERC and DRC results. Run them in KiCad 9.
- Whether the TLV9064 noise and offset are good enough for the quietest pluck. That needs the measured signal level.
