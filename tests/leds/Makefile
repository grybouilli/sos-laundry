FREQ_CPU := 16000000
BAUDRATE := 115200

LIGHT_DIR = ../../light_ws2812
CC := avr-gcc
# https://gcc.gnu.org/bugzilla//show_bug.cgi?id=105523
CFLAGS := -Os -DF_CPU=$(FREQ_CPU) -mmcu=atmega328p -Wall -I$(LIGHT_DIR) #--param=min-pagesize=0

all:
	@echo "ATMega compiler"

%.bin: %.elf
	avr-objcopy -j .text -j .data -O binary $^ $@

%.o: %.c %.h $(LIGHT_DIR)/%.c $(LIGHT_DIR)/%.h
	$(CC) $(CFLAGS) -c -o $@ $<

main.elf: main.c $(LIGHT_DIR)/Objects/light_ws2812.o
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f *.elf *.bin *.o *.gch $(LIGHT_DIR)/*.o

install: main.bin
	avrdude -p atmega328p -P /dev/ttyACM0 -c arduino -V -U flash:w:$<:r
