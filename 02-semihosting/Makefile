BINARY			:= hello

DEVICE 			:= STM32F103C4T6
OPENOCD_FILE 	:= board/st_nucleo_f103rb.cfg

OPENCM3_DIR 	:= ../libopencm3

LDLIBS 			+= -lm
DEFS			+= -Iinclude

OBJS			+= build/src/main.o

ENABLE_SEMIHOSTING ?= 1
ifeq ($(ENABLE_SEMIHOSTING),1)
LDFLAGS			+= --specs=nano.specs
LDLIBS			+= -lrdimon
DEFS			+= -DENABLE_SEMIHOSTING=1
endif

CFLAGS += -flto -fno-fat-lto-objects

include ../libopencm3.mk
