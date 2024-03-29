#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <bluetooth/uart_utils.h>

#define START_BUTTON PD3
#define LED_DETECT PD2

void setup()
{
    // Setup washing machine start button as input pull-up
    PORTD |= (1 << START_BUTTON);

    // Led detection is done with simple input high detection

    setup_bt_com(9600);

}

volatile uint8_t end = 0;

int main()
{
    setup();

    sei();

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