#include <avr/io.h>
#include <avr/pgmspace.h>
#include "init.h"
#include "hmi.h"
#include "print_helper.h"
#include "uart.h"
#include "../lib/hd44780_111/hd44780.h"

/* Initialize error console as stderr in UART1 and print user code info */
void init_errcon(void)
{
    simple_uart1_init();
    stderr = &simple_uart1_out;
    fprintf_P(stderr, PSTR(VER_FV));
    fprintf_P(stderr, PSTR(VER_LIBC));
}

/* Initialize UART0 IO */
void init_uartio(void)
{
    simple_uart0_init();
    stdout = &simple_uart0_io;
    stdin = &simple_uart0_io;
    fprintf_P(stdout, student_name);
    fprintf(stdout, "\n");
    print_banner_P(stdout, banner, BANNER_ROW_COUNT);
}

/* Initialize LEDs */
void initLeds(void)
{
    /* Set PINs for output */
    DDRA |= _BV(LED_RED);
    DDRA |= _BV(LED_GREEN);
    DDRA |= _BV(LED_BLUE);
    DDRB |= _BV(LED_YELLOW);
    /* Turn Arduino Mega yellow LED off */
    PORTB &= ~_BV(LED_YELLOW);
}

/* Initialize LCD */
void init_lcd(void)
{
    lcd_init();
    lcd_goto(LCD_ROW_1_START);
    lcd_puts_P(student_name);
}