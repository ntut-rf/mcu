CFLAGS 	+= -std=gnu11 -ffreestanding -ffunction-sections -fdata-sections -Wall -Winline -fdiagnostics-color=always -Os -flto -ffat-lto-objects -g3
LDFLAGS += -Wl,--gc-sections -nostartfiles -lgcc
EXE = build/bare.elf

include devices/stm32f103rb.mk

.PHONY: all
all: $(EXE)

build/%.o: %.c
	mkdir -p build/
	arm-none-eabi-gcc $(CFLAGS) -c $^ -o $@

build/%.elf: build/%.o
	arm-none-eabi-gcc $(CFLAGS) $(LDFLAGS) $< -o $@

clean:
	rm -rf build/

flash: $(EXE)
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
