MCU=atmega16
PROG=usbasp
PORT=/dev/tty4
BR=115200

TARGET=main
SRC=main.c
FLAGS= -mmcu=$(MCU) -Os -Wall
AVR_FLAGS= -c $(PROG) -p m16 -P $(PORT) -b $(BR)   
all: $(TARGET).hex

$(TARGET).hex: $(TARGET).elf
	avr-objcopy -O ihex -R .eeprom $< $@

$(TARGET).elf: $(SRC)
	avr-gcc $(FLAGS) -o $@ $<

flash:
	avrdude $(AVR_FLAGS) -U flash:w:$(TARGET).hex:i
upload: all flash
run: test
	./$<
test: test.c
	gcc -o $@ $<

compile:
	g++ -Wall main.cpp -o main
	./main
