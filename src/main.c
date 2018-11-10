#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include "uart.h"
#include "hmi.h"

#define BLINK_DELAY_MS 200

/* Initialize error console as stderr in UART1 and print user code info */
static inline void init_errcon(void)
{
    simple_uart1_init();
    stderr = &simple_uart1_out;
    fprintf_P(stderr, PSTR(VER_FV));
    fprintf_P(stderr, PSTR(VER_LIBC));
}

/* Initialize UART0 IO */
static inline void init_uartio(void)
{
    simple_uart0_init();
    stdout = &simple_uart0_io;
    stdin = &simple_uart0_io;
    fprintf_P(stdout, PSTR(STUDENT_NAME));
}

static inline void month_lookup(void)
{
    char letterOfMonth = 0;
    fprintf_P(stdout, PSTR(INPUT_ASK_LETTER));
    if (scanf("%c", &letterOfMonth)) {
        printf("%c\n", letterOfMonth);
    }
    
}

static inline void initLeds(void)
{
    /* Set port A pin 1, 3, 5 and port B pin 7 for output */
    DDRA |= _BV(PA1);
    DDRA |= _BV(PA3);
    DDRA |= _BV(PA5);
    DDRB |= _BV(DDB7);
    /* Set port B pin 7 low to turn Arduino Mega yellow LED off */
    PORTB &= ~_BV(DDB7);
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
        blinkLed(PA1);
        /* Blink green LED */
        blinkLed(PA3);
        /* Blink blue LED */
        blinkLed(PA5);
        /* Run function to read user input and find the month */
        month_lookup();
    }
}