CFLAGS 				+= -mcpu=cortex-m4 -mthumb
LDFLAGS 			+= -T devices/msp432/msp432.ld
OPENOCD_CFG 		= -f interface/cmsis-dap.cfg -f target/ti_msp432p4xx.cfg
OPENOCD_UPLOAD_CMD 	= load_image
