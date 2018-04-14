CFLAGS 				+= -mcpu=cortex-m4 -mthumb
LDFLAGS 			+= -T devices/msp432/msp432.ld
OPENOCD_CFG 		= -f board/ti_msp432_launchpad.cfg
OPENOCD_WRITE_CMD	= flash write_image
