This is a bare minimal example for the Cortex-M boards STM32F103 Nucleo & MSP432 Launchpad.

## Prerequisites

* GNU ARM Toolchain
* GNU make
* OpenOCD

**NOTE** For MSP432, upstream OpenOCD is unable to write the firmware into flash. [TI OpenOCD](https://github.com/seanstone/openocd-ti) is required for progamming the flash (You have to compile it from source). For Arch Linux, a PKGBUILD is avilable [here](https://github.com/seanstone/aur-openocd-ti).

## Device choice

The default device chosen is STM32F1 Nucleo. To change to MSP432 Launchpad, edit `Makefile` and change `include devices/stm32f103rb.mk` to `include devices/msp432.mk`.

## Usage

To build the executable, `cd` into the folder, open the console and
```shell
$ make
```

To flash the executable onto the board,
```shell
$ make flash
```
sudo may be required to flash the image onto MSP432 if udev rules are not set.
