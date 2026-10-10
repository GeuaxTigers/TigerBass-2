# firmware/bsp

The board support package: the STM32CubeIDE project, the code STM32CubeMX generates, and the glue that implements the interfaces declared in [`app/`](../app/) on the real board.

**Status: not started.** It depends on the [microcontroller](../../docs/decisions/0004-microcontroller.md) and [external ADC](../../docs/decisions/0002-external-adc.md) decisions.

## What goes here

- The CubeIDE project files: `.project`, `.cproject`, the `.ioc` file. These must be committed or the project will not open.
- CubeMX-generated sources: `Core/`, `Drivers/`, the USB middleware.
- Our implementations of the `app/` hardware interfaces: string drive, fret-line read, ADC driver, microsecond timer, MIDI transmit queue.
- Interrupt handlers. Keep them short: capture the data, set a flag, return.

## Editing generated files

CubeMX regenerates its files whenever someone changes the `.ioc` and clicks Generate. It keeps only what is between these markers:

```c
/* USER CODE BEGIN 2 */
your code here survives
/* USER CODE END 2 */
```

Anything you write outside a `USER CODE` block in a generated file is erased the next time someone regenerates. Put as little as possible in generated files: call into your own source files from inside a `USER CODE` block.

## What does not belong here

Decisions. If a function here chooses a note, computes a velocity or decides when to send Note Off, it belongs in `app/` where it can be tested.

## Who edits here

One person at a time changes the `.ioc` file, for the same reason as KiCad files: two people's CubeMX changes do not merge. Say so on the issue first.
