/**
 * @file main_slave.c 
 * @author grybouilli (grybouilli@outlook.fr)
 * @brief 
 * @version 0.1
 * @date 2024-02-22
 * 
 * @copyright Copyright (c) 2024
 * 
 *  TEST FILE (Slave side)
 * 
 *  Slave handles led on/off switch which is done through receiving and dealing with master's messages.
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

#define LED PB1

void slave_handler(char* msg)
{
    char decoded[RB_SIZE];
    memset( decoded, '\0', sizeof(char)*RB_SIZE );
    decode_message(decoded,msg);
    if(!strcmp(decoded,"on")) 
    {
        PORTB |= (1 << LED);
    }
    if(!strcmp(decoded, "off"))
    {
        PORTB &= ~(1 << LED);
    } 
}

void setup_slave()
{
    // set pin LED as output low
    DDRB |= (1 << LED);

}

int main(void) 
{

    setup_slave();
    setup_bt_com(9600);

    PORTB |= (1 << LED);
    _delay_ms(2000);
    PORTB &= ~(1 << LED);


    sei();
    while (1) {
        read_pending_messages(slave_handler);
        send_pending_messsages();
    }
    return 0;
}
