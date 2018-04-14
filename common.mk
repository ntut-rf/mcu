CFLAGS 	+= -std=gnu11 -ffreestanding -ffunction-sections -fdata-sections -Wall -Winline -fdiagnostics-color=always -Os -flto -ffat-lto-objects -g3 -I.
LDFLAGS += -Wl,--gc-sections -nostartfiles -lgcc

include devices/stm32f103xx/stm32f103rb.mk

.PHONY: elf
elf: build/$(EXE_NAME).elf

build/%.o: %/*.c
	mkdir -p build/
	arm-none-eabi-gcc $(CFLAGS) -c $^ -o $@

build/%.elf: build/%.o
	arm-none-eabi-gcc $(CFLAGS) $(LDFLAGS) $< -o $@

.PHONY: clean
clean:
	rm -rf build/

.PHONY: flash
flash: build/$(EXE_NAME).elf
	openocd \
		-s "/usr/share/openocd/scripts" \
		$(OPENOCD_CFG) \
	    -c "init" \
		-c "arm semihosting enable" \
	    -c "reset halt" \
	    -c "poll" \
		-c "$(OPENOCD_UPLOAD_CMD) $<" \
	    -c "reset" \
	    -c "shutdown"
