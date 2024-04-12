#include <Light_WS2812/light_ws2812.h>
#include <Light_WS2812/ws2812_config.h>
#include <util/delay.h>


#define PIX_NB 6

volatile struct cRGB leds[PIX_NB];

volatile uint8_t current_state = 0; // 0 is red, 1 is green

void change_state()
{
    if(current_state)
    {
        leds[0].r = 0; leds[0].g = 0;   
        leds[1].r = 0; leds[1].g = 0;   
        leds[2].r = 0; leds[2].g = 0;   
        leds[3].r = 0; leds[3].g = 255; 
        leds[4].r = 0; leds[4].g = 255; 
        leds[5].r = 0; leds[5].g = 255; 
    }
    else
    {
        leds[0].r = 255; leds[0].g = 0; 
        leds[1].r = 255; leds[1].g = 0; 
        leds[2].r = 255; leds[2].g = 0; 
        leds[3].r = 0;   leds[3].g = 0; 
        leds[4].r = 0;   leds[4].g = 0; 
        leds[5].r = 0;   leds[5].g = 0; 
    }

    current_state = !current_state;

}
int main()
{
    // set led pin as output
	DDRB|=_BV(ws2812_pin);

    DDRD &= ~(1 << PD4);
    PORTD = (1 << PD4);

    leds[0].r = 255; leds[0].g = 0; leds[0].b = 0;
    leds[1].r = 255; leds[1].g = 0; leds[1].b = 0;
    leds[2].r = 255; leds[2].g = 0; leds[2].b = 0;
    leds[3].r = 0;   leds[3].g = 0; leds[3].b = 0;
    leds[4].r = 0;   leds[4].g = 0; leds[4].b = 0;
    leds[5].r = 0;   leds[5].g = 0; leds[5].b = 0;

    ws2812_sendarray((uint8_t *)leds, PIX_NB*3);

    while(1)
    {
        if (!(PIND & (1 << PD4))) { // Button was pressed
            change_state();
            _delay_ms(100);
        }
        _delay_ms(10);
        ws2812_sendarray((uint8_t *)leds, PIX_NB*3);
    }
}