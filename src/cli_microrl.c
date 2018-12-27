#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <avr/pgmspace.h>
#include "../lib/hd44780_111/hd44780.h"
#include "../lib/andygock_avr_uart/uart.h"
#include "../lib/helius_microrl/microrl.h"
#include "../lib/matejx_avr_lib/mfrc522.h"
#include "../lib/andy_brown_memdebug/memdebug.h"
#include "hmi.h"
#include "cli_microrl.h"
#include "print_helper.h"
#include "card_list_helper.h"

#define NUM_ELEMS(x)        (sizeof(x) / sizeof((x)[0]))

byte BUFFER[2];
card_t *card_list_ptr = NULL;
char *card_holder = NULL;

void cli_print_help(const char *const *argv);
void cli_example(const char *const *argv);
void cli_print_ver(const char *const *argv);
void cli_print_banner(const char *const *argv);
void cli_handle_month(const char *const *argv);
void cli_print_cmd_error(void);
void cli_print_cmd_arg_error(void);
void cli_card_read(const char *const *argv);
void cli_card_add(const char *const *argv);
void cli_card_remove(const char *const *argv);
void cli_card_list_print(const char *const  *argv);
void cli_mem_stat(const char *const *argv);

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
    {card_read_cmd, card_read_help, cli_card_read, 0},
    {card_add_cmd, card_add_help, cli_card_add, 2},
    {card_remove_cmd, card_remove_help, cli_card_remove, 1},
    {card_list_print_cmd, card_list_print_help, cli_card_list_print, 0},
    {mem_stat_cmd, mem_stat_help, cli_mem_stat, 0}
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

void cli_card_read(const char *const *argv)
{
    (void) argv;
    Uid uid;
    byte bufferSize = 2;

    if (PICC_IsNewCardPresent()) {
        uart0_puts_p(PSTR("Card selected!\r\n"));
        PICC_ReadCardSerial(&uid);
        uart0_puts_p(PSTR("Card type: "));
        uart0_puts(PICC_GetTypeName(PICC_GetType(uid.sak)));
        uart0_puts_p(PSTR("\r\n"));
        uart0_puts_p(PSTR("Card UID: "));
        print_bytes(uid.uidByte, uid.size);
        uart0_puts_p(PSTR(" (size "));
        print_bytes(&uid.size, 1);
        uart0_puts_p(PSTR(" bytes)\r\n"));
    } else {
        uart0_puts_p(read_no_card);
        return;
    }

    PICC_WakeupA(BUFFER, &bufferSize); // wake up after card read
}

void cli_card_add(const char *const *argv)
{
    const char* card_uid_buf = argv[1];
    const char* name_buf = argv[2];
    int uid_s_length = strlen(card_uid_buf);

    if (uid_s_length < 8 || uid_s_length > 20 || uid_s_length % 2 != 0) {
        uart0_puts_p(card_add_not_possible);
        return;
    }

    card_list_ptr = create_card(card_list_ptr, card_uid_buf, name_buf);
}

void cli_card_list_print(const char *const  *argv)
{
    (void) argv;

    if (card_list_ptr == NULL) {
        uart0_puts_p(card_list_empty);
        return;
    }

    print_card_list(card_list_ptr);
}

void cli_card_remove(const char *const *argv)
{
    const char* card_uid_buf = argv[1];
    int uid_s_length = strlen(card_uid_buf);

    if (uid_s_length > 20 || uid_s_length % 2 != 0) {
        uart0_puts_p(card_remove_no_match);
        return;
    }

    if (card_list_ptr != NULL) {
        remove_card(&card_list_ptr, card_uid_buf);
        return;
    }

    uart0_puts_p(card_list_empty);
}

void cli_mem_stat(const char *const *argv)
{
    (void) argv;
    char print_buf[256] = {0x00};
    extern int __heap_start, *__brkval;
    int v;
    int space;
    static int prev_space;
    space = (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
    uart0_puts_p(PSTR("Heap statistics\r\n"));
    sprintf_P(print_buf, PSTR("Used: %u\r\nFree: %u\r\n"), getMemoryUsed(),
              getFreeMemory());
    uart0_puts(print_buf);
    uart0_puts_p(PSTR("\r\nSpace between stack and heap:\r\n"));
    sprintf_P(print_buf, PSTR("Current  %d\r\nPrevious %d\r\nChange   %d\r\n"),
              space, prev_space, space - prev_space);
    uart0_puts(print_buf);
    uart0_puts_p(PSTR("\r\nFreelist\r\n"));
    sprintf_P(print_buf, PSTR("Freelist size:             %u\r\n"),
              getFreeListSize());
    uart0_puts(print_buf);
    sprintf_P(print_buf, PSTR("Blocks in freelist:        %u\r\n"),
              getNumberOfBlocksInFreeList());
    uart0_puts(print_buf);
    sprintf_P(print_buf, PSTR("Largest block in freelist: %u\r\n"),
              getLargestBlockInFreeList());
    uart0_puts(print_buf);
    sprintf_P(print_buf, PSTR("Largest freelist block:    %u\r\n"),
              getLargestAvailableMemoryBlock());
    uart0_puts(print_buf);
    sprintf_P(print_buf, PSTR("Largest allocable block:   %u\r\n"),
              getLargestNonFreeListBlock());
    uart0_puts(print_buf);
    prev_space = space;
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
            // cross fingers and hope that function handles it properly
            cli_cmds[i].func_p (argv);
            return 0;
        }
    }

    cli_print_cmd_error();
    return 0;
}