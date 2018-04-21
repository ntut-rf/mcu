BINARY			:= semihosting
DEVICE 			:= STM32F103RB
OPENOCD_FILE 	:= board/st_nucleo_f103rb.cfg
OPENCM3_DIR 	:= ../libopencm3
OBJS			+= build/semihosting.o
LDFLAGS			+= --specs=nano.specs
LDLIBS			+= -lrdimon
DEFS			+= -DENABLE_SEMIHOSTING=1
include ../libopencm3.mk
