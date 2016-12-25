#modify this to fit the prefix of the gcc toolchain binaries. i.e.
#if gcc is installed as "sh4-none-elf-gcc", then PREFIX should be sh4-none-elf
PREFIX=sh-elf

#DBGFLAGS=-gdwarf-2

#possible choices : SH7058 SH7055_18
#try "make BUILDWHAT=SH7055_18" to override this default.
BUILDWHAT ?= SH7058

# Specify compiler to be used
CC = $(PREFIX)-gcc

# Specify Assembler to be used
#AS = $(PREFIX)-as
AS   = $(PREFIX)-gcc -x assembler-with-cpp

CP   = $(PREFIX)-objcopy
SIZE = $(PREFIX)-size
HEX  = $(CP) -O ihex
BIN  = $(CP) -O binary -S

# Specify linker to be used
LD = $(PREFIX)-ld

# Specify CPU flag
CPU = -m2 -mb

# Common compiler flags
#OPT = -Os
OPT = -Os -ffunction-sections



PROJECT = npkern

#-specs=nano.specs

ASFLAGS = $(CPU) $(DBGFLAGS) -nostartfiles -Wa,-amhls=$(<:.s=.lst) $(E_ASFLAGS)
CPFLAGS = $(CPU) $(DBGFLAGS) $(OPT) -fomit-frame-pointer -std=gnu99 -Wall -Wextra -Wstrict-prototypes \
	-fstack-usage -fverbose-asm -Wa,-ahlms=$(<:.c=.lst) $(E_CFLAGS)

LDFLAGS = $(CPU) -nostartfiles -T$(LDSCRIPT) -Wl,-Map=$(PROJECT).map,--cref,--gc-sections

LDSCRIPT = lkr_705x_180nm.ld


ASRC = start_705x.s
SRC = cmd_parser.c eep_funcs.c main.c crc.c
SRC += platf_705x_180nm.c platf_705x.c


OBJS  = $(ASRC:.s=.o) $(SRC:.c=.o)

all: $(OBJS) $(PROJECT).elf $(PROJECT).hex $(PROJECT).bin
	$(SIZE) $(PROJECT).elf


%.o: %.c
	$(CC) -c $(CPFLAGS) -D $(BUILDWHAT) -I . $< -o $@

%.o: %.s
	$(AS) -c $(ASFLAGS) $< -o $@

%elf: $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $@

%hex: %elf
	$(HEX) $< $@

%bin: %elf
	$(BIN)  $< $@

.PHONY : clean
clean:
	-rm -f $(OBJS)
	-rm -f $(SRC:.c=.su)
	-rm -f  $(PROJECT).elf
	-rm -f  $(PROJECT).map
	-rm -f  $(PROJECT).hex
	-rm -f  $(PROJECT).bin
	-rm -f  $(SRC:.c=.c.bak)
	-rm -f  $(SRC:.c=.lst)
	-rm -f  $(ASRC:.s=.s.bak)
	-rm -f  $(ASRC:.s=.lst)
