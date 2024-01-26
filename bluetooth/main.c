#include <avr/io.h>
#include <util/delay.h>

#define BT_BAUD 38400
#define FOSC 16000000 // Clock Speed
#define BT_UBRR (FOSC / 16 / BT_BAUD - 1)

void setup_bt_com() {
    // HC05 Datasheet setup : Default Baud rate: 38400, Data bits:8, Stop
    // bit:1,Parity:No parity, Data control: has.
    //
    /*Set baud rate */
    UBRR0H = (unsigned char)(BT_UBRR >> 8);
    UBRR0L = (unsigned char)BT_UBRR;

    /* Enable receiver and transmitter */
    UCSR0B = (1 << RXEN0) | (1 << TXEN0);

    /* Set frame format: 8data, 1 stop bit */
    UCSR0C = (1 << UCSZ00) | (1 << UCSZ01) | (1 << UCSZ02);
}

int main(void) { return 0; }
