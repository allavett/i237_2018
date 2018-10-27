#include <avr/io.h>
#include <util/delay.h>
#include <assert.h>
#include "uart.h"
#include "../lib/eriks_freemem/freemem.h"

#define __ASSERT_USE_STDERR
#define BLINK_DELAY_MS 20

/* Initialize error console as stderr in UART1 and print user code info */
static inline void init_errcon(void)
{
    simple_uart1_init();
    stderr = &simple_uart1_out;
    fprintf(stderr, "Version: %s built on: %s %s\n",
            FW_VERSION, __DATE__, __TIME__);
    fprintf(stderr, "avr-libc version: %s avr-gcc version: %s\n",
            __AVR_LIBC_VERSION_STRING__, __VERSION__);
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
    /* Test assert - REMOVE IN FUTURE LABS */
    char *array = NULL;
    uint32_t i = 1;
    /*
     * As sizeof(char) is guaranteed to be 1 it is left here only for better
     * understanding how malloc works. No need to add it in production.
     */
    array = malloc( i * sizeof(char));

    if (array == NULL) {
        // Always test if malloc succeeds
        assert(array != NULL); // Fire assert and print error to stderr
        return; // Exit from program anyway when assert is disabled with NDEBUG.
    }

    /* End test assert */

    while (1) {
        /* Blink red LED */
        blinkLed(PA1);
        /* Blink green LED */
        blinkLed(PA3);
        /* Blink blue LED */
        blinkLed(PA5);
        /* Test assert - REMOVE IN FUTURE LABS */
        /*
         * Increase memory allocated for array by 500 chars
         * until we have eaten it all and print space between stack and heap.
         * That is how assert works in run-time.
         */
        array = realloc( array, (i++ * 500) * sizeof(char));
        fprintf(stderr, "%d\n", freeMem());
        
        if (array == NULL) {
            // Always test if malloc succeeds
            assert(array != NULL); // Fire assert and print error to stderr
            return; // Exit from program anyway when assert is disabled with NDEBUG.
        }
        assert(array);
        /* End test assert */
    }
}