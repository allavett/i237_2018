#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <avr/pgmspace.h>
#include <stdbool.h>
#include "../lib/hd44780_111/hd44780.h"
#include "../lib/andygock_avr_uart/uart.h"
#include "../lib/helius_microrl/microrl.h"
#include "hmi.h"
#include "cli_microrl.h"
#include "print_helper.h"

#define NUM_ELEMS(x)        (sizeof(x) / sizeof((x)[0]))

void cli_print_help(const char *const *argv);
void cli_example(const char *const *argv);
void cli_print_ver(const char *const *argv);
void cli_print_banner(const char *const *argv);
void cli_handle_month(const char *const *argv);
void cli_print_cmd_error(void);
void cli_print_cmd_arg_error(void);

typedef struct cli_cmd {
    PGM_P cmd;
    PGM_P help;
    void (*func_p)();
    const uint8_t func_argc;
} cli_cmd_t;



const cli_cmd_t cli_cmds[] = {
    {help_cmd, help_help, cli_print_help, 0},
    {ver_cmd, ver_help, cli_print_ver, 0},
    {example_cmd, example_help, cli_example, 3},
    {banner_cmd, banner_help, cli_print_banner, 0},
    {month_cmd, month_help, cli_handle_month, 1},
};

void cli_print_help(const char *const *argv)
{
    (void) argv;
    uart0_puts_p(help);

    for (uint8_t i = 0; i < NUM_ELEMS(cli_cmds); i++) {
        uart0_puts_p(cli_cmds[i].cmd);
        uart0_puts_p(PSTR(" : "));
        uart0_puts_p(cli_cmds[i].help);
        uart0_puts_p(PSTR("\r\n"));
    }
}

void cli_example(const char *const *argv)
{
    uart0_puts_p(example_print);

    for (uint8_t i = 1; i < 4; i++) {
        uart0_puts(argv[i]);
        uart0_puts_p(PSTR("\r\n"));
    }
}

void cli_print_ver(const char *const *argv)
{
    (void) argv;
    uart0_puts_p(ver_fw);
    uart0_puts_p(ver_libc);
}

void cli_print_banner(const char *const *argv)
{
    (void) argv;
    print_banner_P(uart0_puts_p, banner, BANNER_ROW_COUNT);
}

void cli_handle_month(const char *const *argv)
{
    lcd_clr(LCD_ROW_2_START, LCD_ROW_2_LAST_COL - LCD_ROW_2_START);
    lcd_goto(LCD_ROW_2_START);
    bool input_match = false;

    for (uint8_t i = 0; i < NAME_MONTH_COUNT; i++) {
        if (!strncmp_P(argv[1], (PGM_P) pgm_read_word(&months_table[i]),
                       strlen(argv[1]))) {
            uart0_puts_p((PGM_P) pgm_read_word(&months_table[i]));
            uart0_puts_p(PSTR("\r\n"));
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

void cli_print_cmd_error(void)
{
    uart0_puts_p(cmd_error);
}

void cli_print_cmd_arg_error(void)
{
    uart0_puts_p(cmd_arg_error);
}

int cli_execute(int argc, const char *const *argv)
{
    // Move cursor to new line. Then user can see what was entered.
    //FIXME Why microrl does not do it?
    uart0_puts_p(PSTR("\r\n"));

    for (uint8_t i = 0; i < NUM_ELEMS(cli_cmds); i++) {
        if (!strcmp_P(argv[0], cli_cmds[i].cmd)) {
            // Test do we have correct arguments to run command
            // Function arguments count shall be defined in struct
            if ((argc - 1) != cli_cmds[i].func_argc) {
                cli_print_cmd_arg_error();
                return 0;
            }

            // Hand argv over to function via function pointer,
            // cross fingers and hope that funcion handles it properly
            cli_cmds[i].func_p (argv);
            return 0;
        }
    }

    cli_print_cmd_error();
    return 0;
}