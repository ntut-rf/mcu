CFLAGS 	+= -std=gnu11 -ffreestanding -ffunction-sections -fdata-sections -Wall -Winline -fdiagnostics-color=always -Os -flto -ffat-lto-objects -g3 -I.
LDFLAGS += -Wl,--gc-sections -nostartfiles -lgcc

ifeq ($(DEVICE),STM32F103RB)
	include devices/stm32f103xx/stm32f103rb.mk
endif
ifeq ($(DEVICE),MSP432)
	include devices/msp432/msp432.mk
endif

.PHONY: elf
elf: build/$(EXE_NAME).elf

OBJ := $(patsubst %.c, build/%.c.o, $(wildcard $(EXE_NAME)/*.c)) build/startup.S.o build/isr.s.o

$(info OBJ: $(OBJ))

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
