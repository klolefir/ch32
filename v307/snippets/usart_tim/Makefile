ELF2BIN = riscv-none-embed-objcopy
CC = riscv-none-embed-gcc

USR = $(USER)
V307_PATH = /home/$(USR)/projects/ch32/v307
PRJ_PATH = $(PWD)
VENDOR_PATH = $(V307_PATH)/vendor
CORE_PATH = $(VENDOR_PATH)/core
V307LIB_PATH = $(VENDOR_PATH)/ch32v307
PERLIB_PATH = $(V307_PATH)/perlib
KELIB_PATH = $(V307_PATH)/kelib
OPTIMIZATION = -O0

CFLAGS = -Wall -g -march=rv32imafc -mabi=ilp32f $(OPTIMIZATION)

MAIN_INC 	= 	-I ./
VENDOR_INC 	= 	-I $(CORE_PATH) \
				-I $(V307LIB_PATH)/inc
PERLIB_INC 	=  	-I $(PERLIB_PATH)
KELIB_INC 	=  	-I $(KELIB_PATH)

INC = 	$(MAIN_INC) \
		$(VENDOR_INC) \
		$(PERLIB_INC) \
		$(KELIB_INC)

MACRO =	-D CH32V30x_D8C

CPPFLAGS = $(MACRO) \
		   $(INC)

LINKER = $(V307_PATH)/general/link.ld
LDFLAGS = -Wall -g -nostdlib -T $(LINKER)

MAIN_SRC 	= 	$(PRJ_PATH)/main.c
SYSTEM_SRC 	= 	$(V307LIB_PATH)/src/system_ch32v30x.c
STARTUP_SRC = 	$(V307_PATH)/general/startup.s
PERLIB_SRC	=	$(PERLIB_PATH)/gpio.c \
				$(PERLIB_PATH)/rcc.c \
				$(PERLIB_PATH)/nvic.c \
				$(PERLIB_PATH)/usart.c \
				$(PERLIB_PATH)/tim.c \
				$(PERLIB_PATH)/tim_hd.c
KELIB_SRC	=	$(KELIB_PATH)/kestring.c
SRC = 	$(MAIN_SRC) \
		$(SYSTEM_SRC) \
		$(STARTUP_SRC) \
		$(PERLIB_SRC) \
		$(KELIB_SRC)

ELF = firmware.elf
BIN = firmware.bin

OPENOCD 		= /usr/local/bin/openocd-riscv
OPENOCD_INT 	= /home/klolefir/soft/wch/OpenOCD/bin/wch-riscv.cfg
OPENOCD_PORT 	= 3333

GDB 		= gdb-multiarch
GDB_CONF 	= $(V307_PATH)/general/gdb.conf

all: $(BIN)

$(ELF): $(SRC)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $^ -o $@

$(BIN): $(ELF)
	$(ELF2BIN) -O binary $< $@

openocd:
	$(OPENOCD) -f $(OPENOCD_INT)

ocdflash:
	$(OPENOCD) -f $(OPENOCD_INT) -c init -c halt  -c "program $(BIN)" -c wlink_reset_resume -c exit

gdbflash: $(BIN)
	$(GDB) -x $(GDB_CONF)

debug: $(ELF)
	$(GDB) -iex "tar ext :$(OPENOCD_PORT)" $^


clean:
	rm $(ELF) $(BIN)
