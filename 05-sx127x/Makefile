BINARY			:= sx127x
DEVICE 			:= STM32F103RB
OPENOCD_FILE 	:= board/st_nucleo_f103rb.cfg
OPENCM3_DIR 	:= ../libopencm3
OBJS			+= $(patsubst %.c, build/%.o, $(wildcard *.c))
CFLAGS			+= -I.
include ../libopencm3.mk
