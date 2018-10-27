#include <avr/io.h>
#include <util/delay.h>

#define BLINK_DELAY_MS 500

static inline void initLeds(void)
{
    /* Set port B pin 4, 6, 7 and port H pin 7 for output */
    DDRA |= _BV(PA1);
    DDRA |= _BV(PA3);
    DDRA |= _BV(PA5);
    DDRB |= _BV(DDB7);
    /* Set port B pin 7 low to turn Arduino Mega yellow LED off */
    PORTB &= ~_BV(DDB7);
}

static inline void blinkLed(const char led) {
    /* Set port A pin high to turn LED on */
    PORTA |= _BV(led);
     _delay_ms(BLINK_DELAY_MS);
     /* Set port A pin 0 low to turn red LED off */
     PORTA &= ~_BV(led);
     _delay_ms(BLINK_DELAY_MS);
    
}

void main (void)
{
    initLeds();

    while (1) {
        blinkLed(PA1);
        blinkLed(PA3);
        blinkLed(PA5);
    }
}