export EXE_NAME = 01-blink
export DEVICE = STM32F103RB

.PHONY: default
default: elf

.PHONY: %
%:
	$(MAKE) -C.. -f barebsp/common.mk $*
