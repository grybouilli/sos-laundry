#include <bluetooth/uart_utils.h>
#include <bluetooth/ring_buffer.h>

#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>
#include <string.h>

#define BT_LED PD5
void slave_handler(char* msg)
{
    char decoded[RB_SIZE];
    memset( decoded, '\0', sizeof(char)*RB_SIZE );
    decode_message(decoded,msg);
    if(!strcmp(decoded,"begin")) 
    {
        // set leds to laundry on-going scheme
        PORTB |= (1 << PB5);
    }
    if(!strcmp(decoded, "end"))
    {
        // set leds to laundry done scheme
        PORTB &= ~(1 << PB5);
    } 

    if(!strcmp(decoded, "connect"))
    {
        PORTD |= (1 << BT_LED);
        send_encoded_message("connect");
    } 
}

void setup_slave()
{
    // set pin BT_LED as output low
    DDRD |= (1 << BT_LED);
    PORTD &= ~(1 << BT_LED);

    // set pin RGB_LED as output low
    DDRB |= (1 << PB5);
    PORTB &= ~(1 << PB5);

}

int main(void) 
{

    setup_slave();
    setup_bt_com(9600);

    _delay_ms(2000);


    sei();
    while (1) {
        read_pending_messages(slave_handler);
        send_pending_messsages();
    }
    return 0;
}