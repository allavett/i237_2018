#include <avr/io.h>
#include <avr/pgmspace.h>
#include "type_def.h"
#include "cli_microrl.h"
#include "init.h"
#include "hmi.h"
#include "print_helper.h"
#include "../lib/andygock_avr_uart/uart.h"
#include "../lib/hd44780_111/hd44780.h"
#include "../lib/helius_microrl/microrl.h"
#include "../lib/matejx_avr_lib/mfrc522.h"

#define UART_BAUD 9600

void init_rfid_reader(void)
{
    /* Init RFID-­?RC522 */
    MFRC522_init();
    PCD_Init();
}

void init_sys_timer(void)
{
    TCCR1A = 0;
    TCCR1B = 0;
    TCCR1B |= _BV(WGM12); // Turn on CTC (Clear Timer on Compare)
    TCCR1B |= _BV(CS12); // fCPU/256
    OCR1A = 62549; // Note that it is actually two registers OCR5AH and OCR5AL
    TIMSK1 |= _BV(OCIE1A); // Output Compare A Match Interrupt Enable
}

/* Initialize UART for outputting system information (uart0) and IO for user commands (uart1) */
void init_uart(void)
{
    uart0_init(UART_BAUD_SELECT(UART_BAUD, F_CPU));
    uart0_puts_p(console_started);
    uart0_puts_p(student_name);
    uart0_puts_p(PSTR("\r\n"));
    uart0_puts_p(cmd_intro);
    uart1_init(UART_BAUD_SELECT(UART_BAUD, F_CPU));
    uart1_puts_p(PSTR("System info.\r\n"));
    uart1_puts_p(ver_fw);
    uart1_puts_p(ver_libc);
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