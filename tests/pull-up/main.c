#include <avr/io.h>
#include <util/delay.h>

/* PULL-UP TEST
*
* In this test, we tried to light up an LED that is connected on PIN 9 by pushing a button or connecting the ground to PIN 7.
*
*/

int main()
{
    DDRB |= (1<<PB1);
    PORTB &= ~(1<<PB1);

    DDRD &= ~(1 <<PD7);
    PORTD |= (1 <<PD7);


    while(1)
    {
        if(!(PIND & (1<<PD7))) // if current detected
        {
            PORTB |= (1<<PB1);  // turn on
        } else
        {
            PORTB &= ~(1<<PB1); // turn off
        }
    }
}