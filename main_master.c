/**
 * @file main_master.c
 * @author grybouilli (grybouilli@outlook.fr)
 * @brief Master side of the project. Detects end of washing cycle and communicates it to the slave. Plays sound when done.
 * @version 0.1
 * @date 2024-04-07
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <bluetooth/uart_utils.h>
#include <bluetooth/ring_buffer.h>
#include <string.h>

#define START_BUTTON PD3
#define LED_DETECT PD2

void setup()
{
    // Setup washing machine start button as input pull-up
    DDRD &= ~(1 << START_BUTTON);
    PORTD |= (1 << START_BUTTON);

    // Led detection is done with simple input-high detection
    DDRD    &= ~(1 << LED_DETECT);
    PORTD   &= ~(1 << LED_DETECT);

    setup_bt_com(9600);

}

/**
 * @brief  Boolean to test BT connexion establishment
 * 
 */
volatile uint8_t connect = 0; 

/**
 * @brief Handle for bluetooth message reception 
 * @param msg The received message to handle
 */
void master_handler(char* msg)
{
    char decoded[RB_SIZE];
    memset( decoded, '\0', sizeof(char)*RB_SIZE );
    decode_message(decoded,msg);

    if(!strcmp(decoded, "connect")) // slave has received "connect" message => connexion is established
    {
        connect = 1;
    } 
}

/**
 * @brief Boolean to test end of washing cycle
 * 
 */
volatile uint8_t end = 0;

int main()
{
    setup();

    _delay_ms(2000);

    sei();

    while(!connect) // if connexion established, loop breaks
    {
        send_encoded_message("connect");        // check for connexion
        read_pending_messages(master_handler); 
        send_pending_messsages();
    }

    while(1)
    {
        if (!(PIND & (1 << START_BUTTON))) { // Start button was pressed

            _delay_ms(1000);

            // Tell display cycle is beginning
            send_encoded_message("begin");
            send_pending_messsages();
            end = 0;

            while(!end) // Loop as long as machine is washing
            {
                if ((PIND & (1 << LED_DETECT))) { // End LED is on, cycle is done
                    end = 1;
                }
            }
            // Tell display cycle is done
            send_encoded_message("end");
            send_pending_messsages();
            // Trigger audio ?
        }
    }
}