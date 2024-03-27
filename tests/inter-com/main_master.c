/**
 * @file main_master.c 
 * @author grybouilli (grybouilli@outlook.fr)
 * @brief 
 * @version 0.1
 * @date 2024-02-22
 * 
 * @copyright Copyright (c) 2024
 * 
 *  TEST FILE (Master side)
 * 
 *  Master handles the User Interaction which is done through connecting / disconnecting a jump wire between ground and PIN 7.
 * The communication protocole is as follows: 
 *  -   Master sends on / off when PIN 7 is triggered / open ;
 *  -   Slave responds on / off when slave's led is on / off, as long as acknowledgement wasn't received;
 *  -   Master sends ack when receiving on / off message from slave.
 */

#include "uart_utils.h"
#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>
#include <string.h>
#include "ring_buffer.h"

void master_handler(char* msg)
{
}

void check_pull_up()
{
    if(!(PIND & (1<<PD7))) // if current detected
    {
        send_encoded_message("on");
        PORTB |= (1 << PB5);
    } else
    {
        send_encoded_message("off");
        PORTB &= ~(1 << PB5);
    }
}

void setup_master()
{
    // set pin 7 as pull-up
    DDRD &= ~(1 << PD7);
    PORTD |= (1 << PD7);

    // set board led as output low
    DDRB |= (1 << PB5);
}

int main(void) 
{

    setup_master();
    setup_bt_com(9600);

    PORTB |= (1 << PB5);
    _delay_ms(2000);
    PORTB &= ~(1 << PB5);

    sei();
    while (1) {
        check_pull_up();
        read_pending_messages(master_handler);
        send_pending_messsages();
    }
    return 0;
}
