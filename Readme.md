This is a bare minimal example for the Cortex-M boards STM32F103 Nucleo & MSP432 Launchpad.

## Prerequisites

* GNU ARM Toolchain
* GNU make
* OpenOCD

## Usage

To build the executable: (`bare.c` &rarr; `build/bare.elf`), open the console and
```shell
$ make
```

To flash the executable onto the board,
```shell
$ make flash
```
