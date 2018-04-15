# Be silent per default, but 'make V=1' will show all compiler calls.
ifneq ($(V),1)
Q		:= @
NULL	:= 2>/dev/null
endif

###############################################################################
# Executables

PREFIX		?= arm-none-eabi
CC			:= $(PREFIX)-gcc
CXX			:= $(PREFIX)-g++
LD			:= $(PREFIX)-gcc
AR			:= $(PREFIX)-ar
AS			:= $(PREFIX)-as
OBJCOPY		:= $(PREFIX)-objcopy
OBJDUMP		:= $(PREFIX)-objdump
GDB			:= $(PREFIX)-gdb
OPT			:= -Os
CSTD		?= -std=gnu11

###############################################################################
# Source files

#OBJS		+= build/$(BINARY).o
OPENCM3_SCRIPT_DIR 	= $(OPENCM3_DIR)/scripts

###############################################################################
# C flags

TGT_CFLAGS	+= $(OPT) $(CSTD) -g
TGT_CFLAGS	+= $(ARCH_FLAGS)
TGT_CFLAGS	+= -Wextra -Wshadow -Wimplicit-function-declaration
TGT_CFLAGS	+= -Wredundant-decls -Wmissing-prototypes -Wstrict-prototypes
TGT_CFLAGS	+= -fno-common -ffunction-sections -fdata-sections

###############################################################################
# C++ flags

TGT_CXXFLAGS	+= $(OPT) $(CXXSTD) -g
TGT_CXXFLAGS	+= $(ARCH_FLAGS)
TGT_CXXFLAGS	+= -Wextra -Wshadow -Wredundant-decls  -Weffc++
TGT_CXXFLAGS	+= -fno-common -ffunction-sections -fdata-sections

###############################################################################
# C & C++ preprocessor common flags

TGT_CPPFLAGS	+= -MD -MF build/$*.d
TGT_CPPFLAGS	+= -Wall -Wundef
TGT_CPPFLAGS	+= $(DEFS)

###############################################################################
# Linker flags

TGT_LDFLAGS		+= --static -nostartfiles
TGT_LDFLAGS		+= -T$(LDSCRIPT)
TGT_LDFLAGS		+= $(ARCH_FLAGS)
TGT_LDFLAGS		+= -Wl,-Map=$(*).map
TGT_LDFLAGS		+= -Wl,--gc-sections
ifeq ($(V),99)
TGT_LDFLAGS		+= -Wl,--print-gc-sections
endif

###############################################################################
# Used libraries
LDLIBS		+= -Wl,--start-group -lc -lgcc -lnosys -Wl,--end-group

###############################################################################
###############################################################################

.PHONY: all elf bin hex srec list clean
all: 	elf
elf: 	build/$(BINARY).elf
bin: 	build/$(BINARY).bin
hex: 	build/$(BINARY).hex
srec: 	build/$(BINARY).srec
list: 	build/$(BINARY).list
flash: 	build/$(BINARY).flash

# New style, assume device is provided, and we're generating the rest.
include $(OPENCM3_DIR)/mk/genlink-config.mk
include $(OPENCM3_DIR)/mk/genlink-rules.mk

%.bin: %.elf
	@printf "  OBJCOPY $(*).bin\n"
	$(Q)$(OBJCOPY) -Obinary $< $@

%.hex: %.elf
	@printf "  OBJCOPY $(*).hex\n"
	$(Q)$(OBJCOPY) -Oihex $< $(*).hex

%.srec: %.elf
	@printf "  OBJCOPY $(*).srec\n"
	$(Q)$(OBJCOPY) -Osrec $< $@

%.list: %.elf
	@printf "  OBJDUMP $(*).list\n"
	$(Q)$(OBJDUMP) -S $< > $@

%.elf %.map: $(OBJS) $(LDSCRIPT)
	@printf "  LD      $@\n"
	@mkdir -p build
	$(Q)$(LD) $(TGT_LDFLAGS) $(LDFLAGS) $(OBJS) $(LDLIBS) -o $@

-include $(OBJS:.o=.d)

build/%.o: %.c
	@printf "  CC      $@\n"
	@mkdir -p build/src
	$(Q)$(CC) $(TGT_CFLAGS) $(CFLAGS) $(TGT_CPPFLAGS) $(CPPFLAGS) -o $@ -c $<

build/%.o: %.cpp
	@printf "  CXX     $@\n"
	@mkdir -p build/src
	$(Q)$(CXX) $(TGT_CXXFLAGS) $(CXXFLAGS) $(TGT_CPPFLAGS) $(CPPFLAGS) -o $@ -c $<

clean:
	@printf "  CLEAN\n"
	$(Q)rm -rf generated.* build

%.flash: %.elf
	@printf "  FLASH   $<\n"
	$(Q)openocd \
		-s "/usr/share/openocd/scripts" \
		-f $(OPENOCD_FILE) \
		-c "init" \
		-c "arm semihosting enable" \
		-c "reset halt" \
		-c "poll" \
		-c "flash write_image erase $<" \
		-c "reset" \
		-c "shutdown" 2>/dev/null

mon:
	@openocd \
		-s "/usr/share/openocd/scripts" \
		-f "$(OPENOCD_FILE)" \
		-c "init" \
		-c "arm semihosting enable" \
		-c "reset run"

.PHONY: serial
serial: serial.0

.PHONY: serial.%
serial.%:
	@picocom -b 115200 /dev/ttyACM$*
