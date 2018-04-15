export EXE_NAME = 00-bare
export DEVICE = STM32F103RB

.PHONY: default
default: elf

.PHONY: %
%:
	$(MAKE) -C.. -f barebsp/common.mk $*
