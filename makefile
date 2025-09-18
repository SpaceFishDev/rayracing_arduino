MCU = atmega328p
F_CPU = 16000000UL

PROGRAMMER = arduino
PORT = /dev/ttyUSB0
BAUD = 9600

SRC_DIR = src
SRC_C = $(wildcard $(SRC_DIR)/*.c) $(wildcard $(SRC_DIR)/*/*.c)  
SRC_ASM = $(wildcard $(SRC_DIR)/*.s) $(wildcard $(SRC_DIR)/*/*.s)

OBJ_C = $(SRC_C:.c=.o)
OBJ_ASM = $(SRC_ASM:.s=.o)

TARGET = main
ELF = $(TARGET).elf
HEX = $(TARGET).hex

CFLAGS = -mmcu=$(MCU) -DF_CPU=$(F_CPU) -Os -fpermissive 

all: $(HEX)

%.o: %.c
	avr-gcc $(CFLAGS) -c $< -o $@

%.o: %.s
	avr-gcc $(CFLAGS) -c $< -o $@

$(ELF): $(OBJ_C) $(OBJ_ASM)
	avr-gcc $(CFLAGS) -o $(ELF) $(OBJ_C) $(OBJ_ASM)

$(HEX): $(ELF)
	avr-objcopy -O ihex -R .eeprom $(ELF) $(HEX)

flash: $(HEX)
	avrdude -v -p $(MCU) -c $(PROGRAMMER) -P $(PORT) -b 115200 -D -U flash:w:$(HEX):i

clean:
	rm -f $(OBJ_C) $(OBJ_ASM) $(ELF) $(HEX)