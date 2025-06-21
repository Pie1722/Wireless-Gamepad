MCU = atmega328p
F_CPU = 16000000UL
CC = avr-gcc
OBJCOPY = avr-objcopy

CFLAGS = -mmcu=$(MCU) -DF_CPU=$(F_CPU) -Os -Wall
LDFLAGS = -mmcu=$(MCU)

# List of source files
SRC = main.c spi.c nrf.c serial.c
OBJ = $(SRC:.c=.o)

TARGET = main

all: $(TARGET).hex

$(TARGET).elf: $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET).hex: $(TARGET).elf
	$(OBJCOPY) -O ihex -R .eeprom $< $@

upload:
	avrdude -v -p m328p -c arduino -P /dev/ttyUSB0 -b 115200 -U flash:w:$(TARGET).hex:i

clean:
	rm -f *.o *.elf *.hex
