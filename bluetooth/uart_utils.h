#ifndef __UART_UTILS_H__
#define __UART_UTILS_H__

#include <avr/io.h>

#define BT_BAUD 9600
#define FOSC 16000000 // Clock Speed
#define UBRR(BAUD) ((FOSC / (8 * BAUD)) - 1)

#ifndef END_CHAR
#define END_CHAR '\n'
#endif

#define END_CHAR_STR (char []){END_CHAR, '\0'}


void setup_bt_com(uint32_t br);
void send_byte(uint8_t c);
void send_message(char *msg);
void send_encoded_message(char * msg);
char * encode_message(char* dst, char * msg);
char * decode_message(char* dst, char * msg);
void send_pending_messsages();
void read_pending_messages(void (*)(char *));

#endif
