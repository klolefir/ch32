ELF2BIN = riscv-none-embed-objcopy
CC = riscv-none-embed-gcc

USR = $(USER)
V307_PATH = /home/$(USR)/projects/ch32/v307
PRJ_PATH = $(V307_PATH)/snippets/web
VENDOR_PATH = $(V307_PATH)/vendor
CORE_PATH = $(VENDOR_PATH)/core
DEBUG_PATH = $(VENDOR_PATH)/debug
V307LIB_PATH = $(VENDOR_PATH)/ch32v307
#PERLIB_PATH = $(V307_PATH)/perlib
OPTIMIZATION = -O0

#CFLAGS = -Wall -g -mcpu=cortex-m4 -mthumb $(OPTIMIZATION)
CFLAGS = -Wall -g $(OPTIMIZATION)

MACRO = -D NOPRINTF
MAIN_INC = 		-I ./
VENDOR_INC = 	-I $(CORE_PATH) \
				-I $(V307LIB_PATH)/inc
DEBUG_INC = 	-I $(DEBUG_PATH)
#PERLIB_INC = 	-I $(PERLIB_PATH)

INC = 	$(MAIN_INC) \
		$(DEBUG_INC) \
		$(VENDOR_INC)

CPPFLAGS = $(MACRO) $(INC)

LINKER = $(V307_PATH)/general/link.ld
#LDFLAGS = -static -L. -lwchnet -T $(LINKER)
LDFLAGS = -T $(LINKER)

MAIN_SRC 	= 	$(PRJ_PATH)/main.c \
				$(PRJ_PATH)/ch32v30x_it.c \
				$(PRJ_PATH)/eth_driver.c \
				$(PRJ_PATH)/HTTPS.c \
				$(PRJ_PATH)/syscalls.c
DEBUG_SRC	= 	$(DEBUG_PATH)/debug.c
SYSTEM_SRC 	= 	$(V307LIB_PATH)/src/system_ch32v30x.c
STARTUP_SRC = 	$(V307_PATH)/general/startup.s
V307LIB_SRC	= 	$(V307LIB_PATH)/src/ch32v30x_gpio.c \
				$(V307LIB_PATH)/src/ch32v30x_rcc.c \
				$(V307LIB_PATH)/src/ch32v30x_usart.c \
				$(V307LIB_PATH)/src/ch32v30x_misc.c \
				$(V307LIB_PATH)/src/ch32v30x_eth.c
SRC = 	$(MAIN_SRC) \
		$(SYSTEM_SRC) \
		$(STARTUP_SRC) \
		$(DEBUG_SRC) \
		$(V307LIB_SRC)

ELF = firmware.elf
BIN = firmware.bin

OPENOCD 		= /usr/local/bin/openocd-riscv
OPENOCD_INT 	= /home/klolefir/soft/wch/OpenOCD/bin/wch-riscv.cfg
OPENOCD_PORT 	= 3333

GDB 		= gdb-multiarch
GDB_CONF 	= $(F407_PATH)/general/gdb.conf

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
