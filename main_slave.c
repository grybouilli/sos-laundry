/**
 * @file main_slave.c
 * @author grybouilli (grybouilli@outlook.fr)
 * @brief Slave side of the project. Plays sound and drives RGB led when washing cycle is done.
 * @version 0.1
 * @date 2024-04-07
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include <bluetooth/uart_utils.h>
#include <bluetooth/ring_buffer.h>

#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>
#include <string.h>

#define BT_LED PD5 // Pin for the blue led signaling BT connexion establishment


/**
 * @brief Handle for bluetooth message reception 
 * @param msg The received message to handle
 */
void slave_handler(char* msg)
{
    char decoded[RB_SIZE];
    memset( decoded, '\0', sizeof(char)*RB_SIZE );
    decode_message(decoded,msg);
    if(!strcmp(decoded,"begin")) 
    {
        // set RGB leds to laundry on-going scheme
        PORTB |= (1 << PB5);
    }
    if(!strcmp(decoded, "end"))
    {
        // set RGB leds to laundry done scheme
        PORTB &= ~(1 << PB5);
    } 

    if(!strcmp(decoded, "connect")) // First sent message by master
    {
        PORTD |= (1 << BT_LED);          // turn on blue led to signal user the connexion is established
        send_encoded_message("connect"); // tell master the message has been received
    } 
}

/**
 * @brief Setup slave I/Os
 * 
 */
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