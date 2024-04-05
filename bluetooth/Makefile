FREQ_CPU := 16000000                                                                                                              
BAUDRATE := 115200                                                                                                                
                                                                                                                                  
CC := avr-gcc                                                                                                                     
# https://gcc.gnu.org/bugzilla//show_bug.cgi?id=105523                                                                            
CFLAGS := -Os -DF_CPU=$(FREQ_CPU) -mmcu=atmega328p -Wall #--param=min-pagesize=0 
                                                                                                                                  
all:                                                                                                                              
	@echo "ATMega compiler"                                                                                                         
                                                                                                                                  
%.bin: %.elf                                                                                                                      
	avr-objcopy -j .text -j .data -O binary $^ $@                                                                                   
                                                                                                                                  
%.o: %.c %.h                                                                                                                      
	$(CC) $(CFLAGS) -c -o $@ $<                                                                                                     
                                                                                                                                  
%.elf: main.c ring_buffer.o uart_utils.o
	$(CC) $(CFLAGS) -o $@ $^                                                                                                        
                                                                                                                                  
clean:                                                                                                                            
	rm -f *.elf *.bin *.o *.gch                                                                                                     
