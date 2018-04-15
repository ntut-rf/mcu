This is a bare minimal example for the Cortex-M boards STM32F103 Nucleo & MSP432 Launchpad.

## Prerequisites

* GNU ARM Toolchain
* GNU make
* OpenOCD

**NOTE** For MSP432, upstream OpenOCD is unable to write the firmware into flash. [TI OpenOCD](https://github.com/seanstone/openocd-ti) is required for progamming the flash (You have to compile it from source). For Arch Linux, a PKGBUILD is avilable [here](https://github.com/seanstone/aur-openocd-ti).

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

For the example "02-libopencm3-hello", it is required to build libopencm3 first,
```shell
$ git submodule init
$ git submodule update
$ cd libopencm3
$ make
```
