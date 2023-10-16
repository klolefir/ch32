ELF2BIN = riscv-none-embed-objcopy
CC = riscv-none-embed-gcc

USR = $(USER)
V307_PATH = /home/$(USR)/projects/ch32/v307
PRJ_PATH = $(V307_PATH)/snippets/test
VENDOR_PATH = $(V307_PATH)/vendor
CORE_PATH = $(VENDOR_PATH)/core
DEBUG_PATH = $(VENDOR_PATH)/debug
V307LIB_PATH = $(VENDOR_PATH)/ch32v307
PERLIB_PATH = $(V307_PATH)/perlib
OPTIMIZATION = -Os

#CFLAGS = -Wall -g -mcpu=cortex-m4 -mthumb $(OPTIMIZATION)
CFLAGS = -Wall -g $(OPTIMIZATION)

MACRO = -D STM32F407xx
MAIN_INC = 		-I ./
PERLIB_INC =	-I $(PERLIB_PATH)
VENDOR_INC = 	-I $(CORE_PATH) \
				-I $(V307LIB_PATH)/inc
DEBUG_INC = 	-I $(DEBUG_PATH)
#PERLIB_INC = 	-I $(PERLIB_PATH)

INC = 	$(MAIN_INC) \
		$(DEBUG_INC) \
		$(VENDOR_INC) \
		$(PERLIB_INC)

CPPFLAGS = $(MACRO) $(INC)

LINKER = $(V307_PATH)/general/link.ld
LDFLAGS = -Wall -g -nostdlib -T $(LINKER)

MAIN_SRC 	= 	$(PRJ_PATH)/main.c $(PRJ_PATH)/ch32v30x_it.c
DEBUG_SRC	= 	$(DEBUG_PATH)/debug.c
PERLIB_SRC	=	$(PERLIB_PATH)/rcc.c
SYSTEM_SRC 	= 	$(V307LIB_PATH)/src/system_ch32v30x.c
STARTUP_SRC = 	$(V307_PATH)/general/startup.s
SRC = 	$(MAIN_SRC) \
		$(SYSTEM_SRC) \
		$(STARTUP_SRC) \
		$(PERLIB_SRC)

ELF = firmware.elf
BIN = firmware.bin

OPENOCD 		= /usr/local/bin/openocd-riscv
OPENOCD_INT 	= /home/klolefir/soft/wch/OpenOCD/bin/wch-riscv.cfg
OPENOCD_PORT 	= 3333
GDB				= gdb-multiarch

all: $(BIN)

$(ELF): $(SRC)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $^ -o $@

$(BIN): $(ELF)
	$(ELF2BIN) -O binary $< $@

openocd:
	$(OPENOCD) -f $(OPENOCD_INT)

ocdflash:
	$(OPENOCD) -f $(OPENOCD_INT) -c init -c halt  -c "program $(BIN)"  -c exit

gdbflash: $(BIN)
	$(GDB) -x $(GDB_CONF)

debug: $(ELF)
	$(GDB) -iex "tar ext :$(OPENOCD_PORT)" $^


clean:
	rm $(ELF) $(BIN)
