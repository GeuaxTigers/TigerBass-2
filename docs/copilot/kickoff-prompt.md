# Prompts for the Copilot agent

Copy-paste prompts for GitHub Copilot's cloud agent, which works on a branch in the background and opens a draft pull request when it finishes.

## Before the first prompt

1. Create the repository and commit the starter files: `README.md`, `.github/copilot-instructions.md`, `docs/legacy/tigerbass1-notes.md` and this file.
2. Put last year's work in `legacy/tigerbass1/`: the `ControllerPCB` KiCad project, the fret board project if it is separate, the STM32CubeIDE firmware project, and the FDPR as a PDF. Leave out `#auto_saved_files#`, `*-backups` folders, backup zips and `*.lck` files. The agent only knows what is in the repository. Without these files it works from the notes alone and Prompt 2 cannot run.
3. Open <https://github.com/copilot/agents>, choose this repository, and paste **Prompt 1**. You can also put a prompt in an issue and assign the issue to Copilot.
4. Review the pull request it opens as you would a teammate's. Merge it before starting the next prompt, so each task builds on the last.

Give it one prompt at a time. A prompt that asks for everything produces a pull request nobody can review.

---

## Prompt 1: repository foundation

```text
Set up the foundation of this repository so that eight students, most of them new
to Git, can start working in it next week. This task is structure, documentation
and automation only. Do not write firmware logic and do not create or edit any
KiCad file.

Read these three files completely before changing anything, and follow
.github/copilot-instructions.md throughout:
  - README.md
  - .github/copilot-instructions.md
  - docs/legacy/tigerbass1-notes.md
If legacy/tigerbass1/ exists, list its contents so you know what is there, but
do not analyse it in this task.

Deliver one pull request titled "ci: repository foundation" containing the
following.

1. DIRECTORY SKELETON
   Create every folder shown under "Repository layout" in README.md that does
   not exist yet. Put a short README.md in each top-level folder and in each
   folder under hardware/ and firmware/ saying what belongs there, what does
   not, and who typically edits it. No empty placeholder files.

2. GIT HYGIENE
   a. .gitignore covering:
      - KiCad: *-backups/, _autosave-*, #auto_saved_files#, *.lck, ~*.lck,
        *.kicad_prl, fp-info-cache, *-bak, *.bak
      - STM32CubeIDE build output and per-user settings: Debug/, Release/,
        .settings/, *.launch. Keep .project, .cproject and *.ioc tracked,
        because the project does not open without them.
      - LTspice output: *.raw, *.log, *.op.raw
      - CMake build folders, Python caches, and OS and editor files
      Add a one-line comment above each group saying why it is ignored.
   b. .gitattributes: normalise line endings to LF for text; mark KiCad source
      files as text with LF; mark *.step, *.stp, *.pdf, *.png, *.jpg as binary.
   c. .editorconfig for C, Python, Markdown and YAML.

3. CONTRIBUTING.md
   Expand the "Working agreements" in README.md into a guide a student who has
   never used Git can follow. Include:
   - First-time setup, with both the command line and GitHub Desktop.
   - The everyday loop: pull, branch, change, commit, push, open a pull
     request, respond to review, merge.
   - The rule that one person edits a given KiCad file at a time, how to claim
     a file on an issue, and what to do when two people changed the same file
     anyway (keep one version, redo the other change by hand in KiCad; explain
     why a text merge is not safe).
   - How to review a hardware pull request when you cannot see a diff: open
     the branch in KiCad, run ERC and DRC, compare the exported schematic PDF.
   - How fabrication releases work: tag, exported outputs, release assets.
   - A short section "What goes in your design notebook", mapping a merged
     pull request to a notebook entry.

4. GITHUB TEMPLATES
   - .github/pull_request_template.md with the six description sections from
     .github/copilot-instructions.md, plus a checklist: ERC and DRC run (for
     hardware), tests pass (for firmware), no editor leftovers committed, docs
     updated.
   - Issue forms in .github/ISSUE_TEMPLATE/: design decision, hardware change,
     firmware task, test result, bug.
   - .github/CODEOWNERS with the path patterns filled in and the owners left
     as comments reading TBD(team). Do not invent GitHub usernames.

5. REQUIREMENTS
   docs/requirements.md: one table, one requirement per row, with columns ID,
   requirement, value, source, how it will be verified (test, analysis,
   inspection or demonstration), status.
   - Seed it from the functions (F1 to F6), qualitative constraints (Q1 to Q5)
     and measurable specifications (M1 to M5) of last year's project. If the
     FDPR is in legacy/, take them from it and cite the section. If it is not,
     use what docs/legacy/tigerbass1-notes.md gives you and mark the rest
     TBD(team).
   - Add this year's goals from README.md as new requirements with IDs
     starting TB2-.
   - A requirement with no agreed number gets TBD(team) in the value column.
     Do not supply a number yourself.

6. ARCHITECTURE
   docs/architecture.md:
   - A Mermaid block diagram of the whole instrument: strings, frets, pickups,
     ADC, microcontroller, expression controls, USB, DIN MIDI, battery and
     regulation, and the boundary between the controller board, the fret board
     and off-board parts.
   - A table of every interface that crosses a board boundary: signal, count,
     direction, electrical level, connector. Use TBD(team) freely.
   - A firmware block diagram showing the planned split between firmware/app
     and firmware/bsp and the interfaces between them. Planned only. Do not
     implement it.

7. DECISION RECORDS
   docs/decisions/README.md explaining the format, a template, and one file
   per item in README.md's "Open decisions" list. Each file has status "Open"
   and sections: the question, why it matters, options, criteria for choosing,
   what we need to find out first. Fill in options and criteria from general
   engineering knowledge and from the legacy notes. Leave the decision blank.

8. CONTINUOUS INTEGRATION
   a. .github/workflows/ci.yml, triggered on pull requests and pushes to main:
      - Markdown lint and a broken-link check on internal links.
      - A firmware job that configures, builds and runs ctest, and is skipped
        when firmware/CMakeLists.txt does not exist.
      - A hardware job that finds every *.kicad_sch and *.kicad_pcb under
        hardware/, runs kicad-cli ERC and DRC on them, and uploads a schematic
        PDF and a BOM as artifacts. Skipped when there are none. Pin the KiCad
        version in one place at the top of the file with a comment that it
        must match the version the team installs.
      - A check that fails if a file matching the KiCad backup, autosave or
        lock patterns is committed.
   b. .github/workflows/copilot-setup-steps.yml with a single job named
      exactly copilot-setup-steps that installs what you need to check your
      own work in later tasks: cmake, ninja, gcc, clang-format, cppcheck,
      gcc-arm-none-eabi and python3.

9. tools/README.md listing the scripts planned for this folder (note map
   generator, latency budget, BOM summary) with one line each. Do not write
   the scripts.

DONE WHEN
- Every workflow file passes a YAML syntax check and ci.yml runs green on the
  pull request.
- No file under legacy/ is modified and no KiCad file is created or modified.
- Every TBD(team) you wrote appears under "Questions for the team" in the pull
  request description.
- The pull request description follows the six-section format in
  .github/copilot-instructions.md.

If you find something in the three files you read that is wrong or
contradictory, do not fix it silently. Note it in the pull request.
```

---

## Follow-up prompts

Run these one at a time after Prompt 1 is merged. The order below is the suggested order. Prompts 3, 4, 5 and 6 feed decisions the team has to make before layout starts.

### Prompt 2: Tiger Bass 1 teardown

Needs last year's files in `legacy/tigerbass1/`.

```text
Produce a complete written teardown of last year's controller board and firmware
from the files in legacy/tigerbass1/, and use it to correct
docs/legacy/tigerbass1-notes.md. Do not modify anything under legacy/.

1. Use kicad-cli to export, for each KiCad project in legacy/tigerbass1/: a
   schematic PDF, a netlist, and a BOM as CSV. Commit them under
   docs/legacy/exports/.
2. From the netlist, write docs/legacy/tigerbass1-pinmap.csv: one row per
   microcontroller pin, with pin number, port name, net name, function, and the
   connector and connector pin it reaches. Include the fret and string each
   fret-input pin corresponds to if the files make that determinable.
3. Write docs/legacy/tigerbass1-teardown.md, one section per schematic sheet:
   what the circuit does, every component with its value and part number, and
   how it connects to the rest. Include the fret board.
4. Walk through the firmware: the main loop step by step, every peripheral it
   configures and how, the ADC sampling scheme, the scan order, how velocity is
   computed, how USB-MIDI is implemented, and every delay or blocking wait with
   its file and line.
5. Go through each item under "Things that contradict each other" in
   docs/legacy/tigerbass1-notes.md and settle it from the source files where
   you can. Say which way it goes and cite the file and line. Add any new
   contradictions you find.
6. Update docs/legacy/tigerbass1-notes.md: correct what was wrong, and change
   each "Seen", "Report" and "Inferred" label to "Verified" with a file
   reference where you confirmed it.
7. End the teardown with two lists: "Worth carrying over" and "Worth changing",
   each item with a reason tied to something in the files. These are
   suggestions for the team, not decisions.

If a file is missing or will not parse, say which one and continue with the rest.
```

### Prompt 3: shared-fret ghosting analysis

```text
The team plans to replace last year's 80 fret segments (one per string per
fret) with 20 full-width frets shared by all four strings. Read the
"Shared-fret ghosting" item in README.md and the note-detection section of
docs/legacy/tigerbass1-notes.md. Work out when the shared-fret scan gives the
wrong answer, and what would prevent it. This is analysis for a team decision.
Do not pick the solution.

1. Write tools/fret_matrix_sim.py (Python, standard library only). Model four
   strings and N frets as an electrical network: each string driven high in
   turn through a series diode, the others held low through their diodes or
   left floating (model both), each fret line pulled down through a resistor,
   and each fretted string contacting either one fret or two adjacent frets
   (model both). Given which fret each string is pressed at, report which fret
   lines read high for each driven string, and what note a "fret closest to the
   bridge wins" rule would report.
2. Run it over every combination of up to four fretted strings. Report which
   combinations give a wrong note, grouped by pattern, and how musically common
   those patterns are on a bass (for example adjacent strings one fret apart).
3. Give tests for the simulator in tools/tests/, including the single-string
   cases, which must always be right.
4. Write docs/analysis/shared-fret-ghosting.md: the mechanism with a diagram,
   the results, and a comparison of ways to deal with it. Consider at least:
   leaving undriven strings floating instead of low; using the per-string
   pickup channels to restrict which strings are scanned; reading analog
   levels instead of logic levels; a resistor ladder along the frets read by
   the new ADC; and keeping segmented frets on only some frets. For each: what
   it fixes, what it does not, what hardware it needs, what it costs in scan
   time.
5. List the bench measurements a student should take on the real neck to
   confirm or kill the model, starting with whether a pressed string really
   touches the fret behind the finger.

State every modelling assumption and every component value you assumed.
```

### Prompt 4: portable firmware core

```text
Build the hardware-independent core of the firmware in firmware/app/, with unit
tests that run on a PC. Follow the "Firmware rules" in
.github/copilot-instructions.md exactly. Write no STM32-specific code and
include no vendor header.

Set up firmware/CMakeLists.txt so that
  cmake -S firmware -B build/host -DTB2_BUILD_TESTS=ON
  cmake --build build/host
  ctest --test-dir build/host --output-on-failure
builds app/ as a static library and runs the tests. Use the Unity test
framework, vendored into firmware/tests/vendor/ with its licence file.

Modules, each with a header, an implementation and a test file:

1. config: fret count, string count, tuning and default MIDI channel in one
   header. Twenty frets, four strings, tuning E1 A1 D2 G2.
2. note_map: MIDI note for a string and fret, computed from the tuning. Fret 0
   is the open string. Reject out-of-range input. Note in a comment that the
   legacy table's fret numbering is in question (docs/legacy/tigerbass1-notes.md)
   and that the team must confirm this mapping on the instrument.
3. midi_msg: build Note On, Note Off, Control Change and Pitch Bend as 3-byte
   MIDI messages and as 4-byte USB-MIDI event packets. Clamp data bytes to 7
   bits. Never emit a Note On with velocity 0.
4. hal interfaces (headers only): drive one string and release all strings;
   read the fret lines as a bitmask; read the latest pickup sample for a
   string; read an expression control; time in microseconds; queue a MIDI
   message without blocking, returning whether it was accepted.
5. fret_scan: given the fret-line bitmask read while one string is driven,
   return the fret closest to the bridge, or open. Add debounce so a fret must
   read the same for a configurable number of scans before it counts.
6. pluck_detect: per string, detect an onset when the pickup level crosses a
   threshold, track the peak over a configurable window, and report the peak
   when the window closes. Detect release when the level stays below a lower
   threshold for a configurable time. Use two thresholds so a signal sitting at
   one level cannot chatter.
7. velocity: map a peak level to MIDI velocity 1 to 127 through a configurable
   curve.
8. voice: one state machine per string tying the above together: idle, onset,
   sounding, released. Send Note On once per pluck. If the fret changes while
   the string is sounding (hammer-on, pull-off, slide), send Note Off for the
   old note and Note On for the new one. Send Note Off on release. Never leave
   a note stuck on: every path out of "sounding" sends Note Off.
9. expression: turn a control reading into Control Change 1 or Pitch Bend, with
   smoothing, a centre dead band for pitch bend, and no message unless the
   output value changed.

Provide fake implementations of the hal interfaces in firmware/tests/fakes/
that let a test script a sequence of fret masks and pickup samples over time
and capture every MIDI message with its timestamp.

Tests must cover at least: every string and fret through note_map; the byte
layout of every message type; two strings plucked in the same scan; a fret line
that bounces; a pickup level held exactly at the threshold; a note change
while sounding; release during the peak window; a full MIDI queue.

Every tunable number (thresholds, windows, debounce counts, curve) goes in
config with a comment saying it is a placeholder to be set from measurements.
Do not present any of them as tuned.
```

### Prompt 5: external ADC trade study

```text
The new controller board will use an ADC separate from the microcontroller to
measure the four per-string pickup channels. Write the study the team will use
to choose the part. Do not choose it.

Write docs/analysis/adc-trade-study.md:

1. What the ADC has to do, derived from docs/requirements.md and
   docs/legacy/tigerbass1-notes.md: four channels at minimum; fast enough to
   catch the first peak of a plucked string (the legacy notes give the timing
   last year's team measured); enough resolution to give a useful velocity
   range; an input range that suits the pickup and front end. Show the
   arithmetic for the minimum sample rate per channel and the minimum useful
   resolution, and state your assumptions. Where a requirement has no value
   yet, carry it as a variable instead of assuming one.
2. The architecture choices, each explained for a student who has only used a
   microcontroller's built-in ADC: simultaneous-sampling versus multiplexed;
   SAR versus delta-sigma, and what a delta-sigma filter's delay does to
   latency; SPI versus I2C versus parallel, and what bus speed each needs for
   the sample rate above; single-ended versus differential inputs; internal
   versus external reference; whether the expression controls should share
   this ADC or stay on the microcontroller.
3. What goes in front of it: buffering, anti-alias filtering, protection,
   biasing an AC pickup signal into a single-supply ADC. Compare against last
   year's passive clamp-and-diode network.
4. A comparison table of candidate parts, with a column for each criterion in
   section 1 and 2 plus package (the team hand-solders, so note anything
   without leads) and supply voltage. Include a part only if you can link its
   manufacturer datasheet and cite the page for each number. Do not give
   prices or stock; add empty columns for a student to fill in from a
   distributor.
5. What to prototype first, and what to measure on the bench to confirm the
   choice.

Also write tools/adc_budget.py: given channel count, sample rate, resolution
and bus type, print the bus clock needed, the time to read all channels once,
and the share of a latency budget that takes.
```

### Prompt 6: latency budget

```text
Write tools/latency_budget.py and docs/analysis/latency-budget.md.

The script takes a small config file listing each stage between a pluck and the
MIDI message leaving the instrument, each with a best case and worst case in
microseconds, and prints a table with totals and each stage's share. Stages:
time for the pickup signal to reach its first peak; ADC acquisition and
transfer; fret scan including settling time after a string is driven; debounce;
firmware processing; USB transfer; DIN MIDI transmission (three bytes at 31250
baud).

Commit two configs: one reconstructing last year's design from
docs/legacy/tigerbass1-notes.md, with each number's source in a comment, and
one template for this year with TBD values.

In the document: explain each stage in plain language; show which stages are
physics the team cannot change and which are design choices; correct last
year's USB figure using the full-speed frame timing noted in the legacy notes;
and describe how to measure the real end-to-end latency on the bench with an
oscilloscope, including what to trigger on and what to use as the "MIDI sent"
signal.
```

### Prompt 7: microcontroller and pin budget

```text
Last year's board used a 144-pin STM32F439 because it needed 80 fret inputs.
This year the plan needs about 20. Write docs/analysis/mcu-pin-budget.md to
help the team decide whether to keep that part or move to a smaller one.

1. Count the pins and peripherals this year's design needs, from
   docs/architecture.md: fret inputs, string outputs, the bus to the external
   ADC, expression inputs, USB, DIN MIDI UART, debug, crystal, LEDs, battery
   monitoring, spares. Mark each count as firm or TBD.
2. List what staying on the STM32F439 gives (firmware and schematic reuse, a
   development board the lab already has) and what it costs (package size,
   board area, power, price).
3. List what the team should check in any alternative: USB device support
   without an external PHY, enough timers and SPI speed for the ADC, 5 V
   tolerant inputs where needed, package the team can solder, availability of
   a development board, support in STM32CubeIDE.
4. Do not recommend a specific alternative part unless you can link its
   datasheet and cite the pages that satisfy each check.

Also write tools/pin_budget.py that reads a CSV of signals and prints the
totals by type.
```

### Prompt 8: USB-MIDI

```text
Last year's firmware sent MIDI by calling ST's HID class functions with
modified descriptors. Write docs/analysis/usb-midi.md covering how to do it
properly.

1. Explain the USB-MIDI 1.0 class in plain language: the audio control and
   MIDI streaming interfaces, the bulk endpoints, the 4-byte event packet, and
   what a host needs to see to load its class driver with no installation on
   Windows, macOS, Linux, iOS and Android.
2. Write out a complete, commented device descriptor set for a one-port
   USB-MIDI device as a C array in firmware/app/usb_midi_desc.c, with a host
   test that checks every length field and total length against the actual
   array. Leave vendor ID, product ID and strings as clearly named
   placeholders with a comment on how a student project obtains usable IDs.
3. Compare the ways to implement the class on an STM32: adapting ST's device
   library, and using an open-source stack. For each: licence, effort, how
   sending works without blocking, and how it fits the queue interface in
   firmware/app/. If legacy/tigerbass1/ contains the firmware, describe
   exactly what last year's team changed in the HID class to make it work.
4. Explain what limits latency on full-speed USB and what, if anything, the
   device can do about it.
```

### Prompt 9: test plan

```text
Write the verification plan in docs/test/.

1. docs/test/test-plan.md: one test for every requirement in
   docs/requirements.md that is verified by test or demonstration. Each test
   has an ID, the requirement IDs it covers, equipment, setup, numbered steps,
   what to record, and the pass criterion. Where the requirement's value is
   TBD(team), write the criterion in terms of that requirement so it fills in
   when the value is set.
2. docs/test/bring-up-checklist.md for a new controller board, in order:
   visual inspection, resistance checks on every rail before power, first
   power-up on a current-limited supply with expected current, rail voltages,
   clock, programming over SWD, then each subsystem. For each step: what to
   measure, where, the expected value or TBD(team), and what to do if it is
   wrong.
3. docs/test/results/README.md and a results template, so every test run is
   recorded the same way: date, who, board revision, firmware commit,
   equipment, raw data location, result.
4. A traceability table: requirement to test to result.

Include the safety precautions from last year's report that still apply:
energised strings, lithium battery handling, soldering, ESD.
```

---

## Things the agent will not do well

- **Schematic capture and layout.** It cannot use KiCad's editors. It can check rules and export files once a student has drawn something.
- **Anything that needs a measurement.** It can say what to measure and build the script that analyses the data.
- **Choosing parts.** Its knowledge of what is in stock, what things cost and which parts are near end-of-life is unreliable. Use its trade studies to narrow the field, then check a distributor.
- **Tuning.** Every threshold and time constant it writes is a starting point until someone sets it on the real instrument.
