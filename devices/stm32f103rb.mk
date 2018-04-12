CFLAGS 				+= -mcpu=cortex-m3 -mthumb
LDFLAGS 			+= -T devices/stm32f103rb.ld
OPENOCD_CFG 		= -f board/st_nucleo_f103rb.cfg
OPENOCD_UPLOAD_CMD 	= flash write_image erase
