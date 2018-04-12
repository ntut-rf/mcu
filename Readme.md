This is a bare minimal example for the Cortex-M boards STM32F103 Nucleo & MSP432 Launchpad.

## Prerequisites

* GNU ARM Toolchain
* GNU make
* OpenOCD

## Device choice

The default device chosen is STM32F1 Nucleo. To change to MSP432 Launchpad, edit `Makefile` and change `include devices/stm32f103rb.mk` to `include devices/msp432.mk`.

## Usage

To build the executable: (`bare.c` &rarr; `build/bare.elf`), open the console and
```shell
$ make
```

To flash the executable onto the board,
```shell
$ make flash
```
sudo may be required to flash the image onto MSP432. 
