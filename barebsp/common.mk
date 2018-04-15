CFLAGS 	+= -std=gnu11 -ffreestanding -ffunction-sections -fdata-sections -Wall -Winline -fdiagnostics-color=always -O1 -flto -ffat-lto-objects -g3 -I.
LDFLAGS += -Wl,--gc-sections -nostartfiles -lgcc

$(info DEVICE: $(DEVICE))

ifeq ($(DEVICE),STM32F103RB)
	include barebsp/devices/stm32f103xx/stm32f103rb.mk
endif
ifeq ($(DEVICE),MSP432)
	include barebsp/devices/msp432/msp432.mk
endif

.PHONY: elf
elf: build/$(EXE_NAME).elf

OBJ := $(patsubst %.c, build/%.c.o, $(wildcard $(EXE_NAME)/*.c)) build/barebsp/startup.S.o build/barebsp/isr.s.o

build/%.c.o: %.c
	mkdir -p $(@D)
	arm-none-eabi-gcc $(CFLAGS) -c $< -o $@

build/%.S.o: %.S
	mkdir -p $(@D)
	arm-none-eabi-gcc $(CFLAGS) -c $< -o $@

build/%.s.o: %.s
	mkdir -p $(@D)
	arm-none-eabi-gcc $(CFLAGS) -c $< -o $@

build/%.elf: $(OBJ)
	arm-none-eabi-gcc $(CFLAGS) $(LDFLAGS) $^ -o $@

.PHONY: clean
clean:
	rm -rf build/

.PHONY: flash
flash: build/$(EXE_NAME).elf
	openocd \
		-s "/usr/share/openocd/scripts" \
		$(OPENOCD_CFG) \
	    -c "init" \
	    -c "reset halt" \
	    -c "poll" \
		-c "$(OPENOCD_WRITE_CMD) $<" \
	    -c "reset" \
	    -c "shutdown"
