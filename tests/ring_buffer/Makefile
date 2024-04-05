BLUETOOTH_DIR = ../../bluetooth
CC := gcc                                                                                                                     
# https://gcc.gnu.org/bugzilla//show_bug.cgi?id=105523                                                                            
CFLAGS := -DUSING_GCC -Os -Wall -I$(BLUETOOTH_DIR) #--param=min-pagesize=0 

all : run 

run: test_rb.o $(BLUETOOTH_DIR)/ring_buffer.o
	$(CC) $(CFLAGS) -o test test_rb.o $(BLUETOOTH_DIR)/ring_buffer.o 
	./test
                                                                                                                                  
%.bin: %.elf                                                                                                                      
	avr-objcopy -j .text -j .data -O binary $^ $@                                                                                   
                                                                                                                                  
%.o: %.c %.h $(BLUETOOTH_DIR)/%.c $(BLUETOOTH_DIR)/%.h
	$(CC) $(CFLAGS) -c -o $@ $<                                                                                                     
                                                                                                                                  
clean:                                                                                                                            
	rm -f *.elf *.bin *.o *.gch test $(BLUETOOTH_DIR)/*.o
