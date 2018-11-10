#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include "hmi.h"
#include "init.h"
#include "print_helper.h"

static inline void month_lookup(void)
{
    char letterOfMonth = 0;
    fprintf_P(stdout, input_ask_letter);

    if (scanf("%c", &letterOfMonth)) {
        printf("%c\n", letterOfMonth);

        for (uint8_t i = 0; i < NAME_MONTH_COUNT; i++) {
            if (!strncmp_P(&letterOfMonth, (PGM_P) pgm_read_word(&months_table[i]), 1)) {
                fprintf_P(stdout, (PGM_P) pgm_read_word(&months_table[i]));
                fprintf(stdout, "\n");
            }
        }
    }
}

static inline void blinkLed(const char led)
{
    /* Set port A pin high to turn LED on */
    PORTA |= _BV(led);
    _delay_ms(BLINK_DELAY_MS);
    /* Set port A pin low to turn LED off */
    PORTA &= ~_BV(led);
    _delay_ms(BLINK_DELAY_MS);
}

void main (void)
{
    initLeds();
    init_errcon();
    init_uartio();

    while (1) {
        /* Blink red LED */
        blinkLed(LED_RED);
        /* Blink green LED */
        blinkLed(LED_GREEN);
        /* Blink blue LED */
        blinkLed(LED_BLUE);
        /* Run function to read user input and find the month */
        month_lookup();
    }
}