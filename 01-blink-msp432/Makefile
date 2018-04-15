export EXE_NAME = 01-blink-msp432
export DEVICE = MSP432

.PHONY: default
default: elf

.PHONY: %
%:
	$(MAKE) -C.. -f barebsp/common.mk $*
