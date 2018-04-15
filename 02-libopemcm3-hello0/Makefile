BINARY			:= hello

#DEVICE 			:= LPC4320
OOCD_FILE 	:= board/ti_msp432_launchpad.cfg
LDSCRIPT 		= msp432.ld

LIBNAME		= opencm3_lpc43xx
DEFS		+= -DLPC43XX

FP_FLAGS	?= -mfloat-abi=hard -mfpu=fpv4-sp-d16
ARCH_FLAGS	= -mthumb -mcpu=cortex-m4 $(FP_FLAGS)

OPENCM3_DIR 	:= ../libopencm3

LDLIBS 			+= -lm
DEFS			+= -Iinclude

# OBJS			+= build/src/main.o

ENABLE_SEMIHOSTING ?= 1
ifeq ($(ENABLE_SEMIHOSTING),1)
LDFLAGS			+= --specs=nano.specs
LDLIBS			+= -lrdimon
DEFS			+= -DENABLE_SEMIHOSTING=1
endif

CFLAGS += -flto -fno-fat-lto-objects

include rules.mk
