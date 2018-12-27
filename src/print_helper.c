#include <avr/pgmspace.h>
#include <stdio.h>
#include <stdint.h>
#include "../lib/andygock_avr_uart/uart.h"

void print_banner_P(void (*puts_P_function)(const char*),
                    const char * const *banner,
                    const uint8_t rows)
{
    for (uint8_t row = 0; row < rows; row++) {
        puts_P_function((PGM_P) pgm_read_word(&(banner[row])));
        puts_P_function(PSTR("\r\n"));
    }

    puts_P_function(PSTR("\r\n"));
}

void print_bytes(const uint8_t *array, const size_t len)
{
    for (size_t i = 0; i < len; i++) {
        uart0_putc((array[i] >> 4) + ((array[i] >> 4) <= 9 ? 0x30 : 0x37));
        uart0_putc((array[i] & 0x0F) + ((array[i] & 0x0F) <= 9 ? 0x30 : 0x37));
    }
}