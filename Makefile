#This can be changed at compile time as required by
#the gcc toolchain binaries, i.e. if gcc is installed as
#"sh4-none-elf-gcc", then run "make PREFIX=sh4-none-elf ..."
PREFIX ?= sh-elf

#DBGFLAGS=-gdwarf-2

#possible choices : SH7058 SH7055_18 SH7055_35 SH7051
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


PROJBASE = npk
PROJECT = $(PROJBASE)_$(BUILDWHAT)

#-specs=nano.specs

ASFLAGS = $(CPU) $(DBGFLAGS) -nostartfiles -Wa,-amhls=$(<:.s=.lst) $(E_ASFLAGS)
CPFLAGS = $(CPU) $(DBGFLAGS) $(OPT) -fomit-frame-pointer -std=gnu99 -Wall -Wextra -Wstrict-prototypes \
	-fstack-usage -fverbose-asm -Wa,-ahlms=$(<:.c=.lst) $(E_CFLAGS)

LDFLAGS = $(CPU) -nostartfiles -T$(LDSCRIPT) -Wl,-Map=$(PROJECT).map,--cref,--gc-sections


ASRC = start_705x.s

SRC = cmd_parser.c eep_funcs.c main.c crc.c

ifeq ($(BUILDWHAT), SH7051)
	SRC += platf_7050h.c pl_flash_7051.c
else ifeq ($(BUILDWHAT), SH7055_35)
	SRC += platf_7055p.c pl_flash_7055_350nm.c
else
	#new 7055 or 7058, 180nm
	SRC += platf_7055p.c pl_flash_705x_180nm.c
endif


ifeq ($(BUILDWHAT), SH7051)
	LDSCRIPT = lkr_7051.ld
else
	LDSCRIPT = lkr_7055_7058.ld
endif

OBJS  = $(ASRC:.s=.o) $(SRC:.c=.o)

all: npk_commit.h $(OBJS) $(PROJECT).elf $(PROJECT).bin
	$(SIZE) $(PROJECT).elf

%.o: %.c
	$(CC) -c $(CPFLAGS) -D $(BUILDWHAT) -D PLATF=\"$(BUILDWHAT)\" -I . $< -o $@

%.o: %.s
	$(AS) -c $(ASFLAGS) $< -o $@

%elf: $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $@

%hex: %elf
	$(HEX) $< $@

%bin: %elf
	$(BIN)  $< $@

npk_commit.h:
	git log -n 1 --format=format:"#define NPK_COMMIT \"%h\"%n" HEAD > $@

.PHONY : clean
clean:
	-rm -f $(OBJS)
	-rm -f $(SRC:.c=.su)
	-rm -f  $(PROJBASE)_*.elf
	-rm -f  $(PROJBASE)_*.map
	-rm -f  $(PROJBASE)_*.bin
	-rm -f  $(SRC:.c=.c.bak)
	-rm -f  $(SRC:.c=.lst)
	-rm -f  $(ASRC:.s=.s.bak)
	-rm -f  $(ASRC:.s=.lst)
