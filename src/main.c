#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <stdbool.h>
#include <time.h>
#include "hmi.h"
#include "init.h"
#include "print_helper.h"
#include "../lib/hd44780_111/hd44780.h"
#include "../lib/andygock_avr_uart/uart.h"


static inline void month_lookup(void)
{
    char letterOfMonth = 0;
    uart0_puts_p(input_ask_letter);
    

    if (uart0_getc()) {
        uart0_puts(&letterOfMonth);
        lcd_clr(LCD_ROW_2_START, LCD_VISIBLE_COLS);
        lcd_goto(LCD_ROW_2_START);
        bool input_match = false;

        for (uint8_t i = 0; i < NAME_MONTH_COUNT; i++) {
            if (!strncmp_P(&letterOfMonth, (PGM_P) pgm_read_word(&months_table[i]), 1)) {
                uart0_puts_p((PGM_P) pgm_read_word(&months_table[i]));
                uart0_puts("\n");
                lcd_puts_P((PGM_P) pgm_read_word(&months_table[i]));
                lcd_putc(' ');
                input_match = true;
            } else {
                if (!input_match) {
                    lcd_clr(LCD_ROW_2_START, LCD_VISIBLE_COLS);
                    lcd_goto(LCD_ROW_2_START);
                }
            }
        }
    }
}

static inline void heartbeat(void)
{
    static time_t prev_time;
    char ascii_buf[11] = {0x00};
    time_t now = time(NULL);
    if (prev_time != now) {
        //Print uptime to uart1
        ltoa(now, ascii_buf, 10);
        uart1_puts_p(PSTR("Uptime: "));
        uart1_puts(ascii_buf);
        uart1_puts_p(PSTR(" s.\r\n"));
        //Toggle LED
        PORTA ^= _BV(LED_GREEN);
        prev_time = now;
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
    init_uart();
    init_sys_timer();
    init_lcd();

    while (1) {
        heartbeat();
        /* Run function to read user input and find the month */
        //month_lookup();
    }
}

/* System timer ISR */
ISR(TIMER1_COMPA_vect)
{
    system_tick();
}