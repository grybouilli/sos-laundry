FREQ_CPU := 16000000                                                                                                              
BAUDRATE := 115200                                                                                                                

BLUETOOTH_DIR = ../../bluetooth
CC := avr-gcc                                                                                                                     
# https://gcc.gnu.org/bugzilla//show_bug.cgi?id=105523                                                                            
CFLAGS := -Os -DF_CPU=$(FREQ_CPU) -mmcu=atmega328p -Wall -I$(BLUETOOTH_DIR) #--param=min-pagesize=0 
                                                                                                                                  
all:                                                                                                                              
	@echo "ATMega compiler"                                                                                                         
                                                                                                                                  
%.bin: %.elf                                                                                                                      
	avr-objcopy -j .text -j .data -O binary $^ $@                                                                                   
                                                                                                                                  
%.o: %.c %.h $(BLUETOOTH_DIR)/%.c $(BLUETOOTH_DIR)/%.h
	$(CC) $(CFLAGS) -c -o $@ $<                                                                                                     
                                                                                                                                  
main_master.elf: main_master.c $(BLUETOOTH_DIR)/uart_utils.o $(BLUETOOTH_DIR)/ring_buffer.o
	$(CC) $(CFLAGS) -o $@ $^   

main_slave.elf: main_slave.c $(BLUETOOTH_DIR)/uart_utils.o $(BLUETOOTH_DIR)/ring_buffer.o
	$(CC) $(CFLAGS) -o $@ $^                                                                                                        
                                                                                                                                  
clean:                                                                                                                            
	rm -f *.elf *.bin *.o *.gch                                                                                                     
                                                                                                                                  
install_master: main_master.bin                                                                                                                 
	avrdude -p atmega328p -P /dev/ttyACM0 -c arduino -V -U flash:w:$<:r                                                                         

install_slave: main_slave.bin                                                                                                                 
	avrdude -b 57600 -p atmega328p -P /dev/ttyUSB0 -c arduino -V -U flash:w:$<:r                                                                         
