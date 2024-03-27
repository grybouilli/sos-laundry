#include "uart_utils.h"
#include "ring_buffer.h"
#include <avr/interrupt.h>
#include <avr/io.h>
#include <string.h>

struct ring_buffer receiver;
struct ring_buffer sender;

uint16_t commands = 0;

void setup_bt_com(uint32_t br) {

    uint16_t ubrr = UBRR(br);

    UBRR0H = (unsigned char)(ubrr >> 8);
    UBRR0L = (unsigned char)ubrr;

    UCSR0A = (1 << U2X0); // asynchronous double speed mode

    UCSR0B = (1 << RXCIE0) | (1 << TXEN0) | (1 << RXEN0);

    // Set one stop bit
    UCSR0C = (0 << USBS0);

    UCSR0C |= (1 << UCSZ00) | (1 << UCSZ01);

    rb_init(&receiver);
    rb_init(&sender);
}

void send_byte(uint8_t c) { rb_put(&sender, c); }

void send_message(char *msg) {
    uint8_t l = strlen(msg);
    for (uint8_t c = 0; c < l; ++c)
        send_byte(msg[c]);
}

void send_encoded_message(char * msg)
{
    char encoded_msg [RB_SIZE];
    memset( encoded_msg, '\0', sizeof(char)*RB_SIZE );
    send_message(encode_message(encoded_msg, msg));
}

char * encode_message(char * dst, char * msg)
{
    strcat(dst, msg);
    strcat(dst, END_CHAR_STR);
    return dst;
}

char * decode_message(char * dst, char * msg)
{
    uint8_t l = strlen(msg);
    strncpy(dst, msg, l-1);
    dst[l] = '\0';
    return dst;
}

void send_pending_messsages() {
    while (!rb_is_empty(&sender)) {
        while (!(UCSR0A & (1 << UDRE0)))
            ;
        UDR0 = rb_read(&sender);
    }
}

void read_pending_messages(void (*handler)(char *)) {
    cli();
    char buffer[RB_SIZE];
    memset(buffer, '\0', RB_SIZE);

    uint8_t c = 0;
    while (commands) {
        buffer[c++] = rb_read(&receiver);
        if(buffer[c-1] == END_CHAR)
        {
            handler(buffer);
            commands--;
            c = 0;
            memset(buffer, '\0', RB_SIZE);
        }
    }

    sei();
}

ISR(USART_RX_vect) { 
    char c = UDR0;
    rb_put(&receiver, c); 
    if(c == END_CHAR)
    {
        commands++;
    }
}
