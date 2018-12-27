#include <avr/pgmspace.h>
#include "hmi.h"

#define VER_FV "Version: " FW_VERSION " built on: " __DATE__ " " __TIME__ "\r\n"
#define VER_LIBC "avr-libc version: " __AVR_LIBC_VERSION_STRING__ " avr-gcc version: " __VERSION__ "\r\n"

const char console_started[] PROGMEM = "Console started\r\n";
const char student_name[] PROGMEM = "Allar Vendla";
const char cmd_intro[] PROGMEM =
    "Use backspace to delete entry and enter to confirm.\r\nArrow key's and del do not work currently.\r\n\tUse <help> for available commands.\r\n";
const char input_ask_letter[] PROGMEM = "Enter Month name first letter >";
const char ver_fw[] PROGMEM = VER_FV;
const char ver_libc[] PROGMEM = VER_LIBC;
const char cmd_error[] PROGMEM =
    "Command not implemented!\r\n\tUse <help> to get help.\r\n";
const char cmd_arg_error[] PROGMEM =
    "Too few or too many arguments for this command!\r\n\tUse <help>.\r\n";
const char help[] PROGMEM = "Implemented commands:\r\n";
const char help_cmd[] PROGMEM = "help";
const char help_help[] PROGMEM = "Get help";
const char example_cmd[] PROGMEM = "example";
const char example_help[] PROGMEM =
    "Prints out all provided 3 arguments.\r\n\tUsage: example <argument> <argument> <argument>";
const char example_print[] PROGMEM = "Command had following arguments:\r\n";
const char ver_cmd[] PROGMEM = "version";
const char ver_help[] PROGMEM = "Print FW version";
const char banner_cmd[] PROGMEM = "banner";
const char banner_help[] PROGMEM = "Print banner";
const char month_cmd[] PROGMEM = "month";
const char month_help[] PROGMEM =
    "Print and display matching month.\r\n\tUsage: month <string>";
const char card_read_cmd[] PROGMEM = "read-uid";
const char card_read_help[] PROGMEM =
    "Read Mifare card and print Card ID";
const char read_no_card[] PROGMEM = "Unable to select card!\r\n";
const char card_add_cmd[] PROGMEM = "add";
const char card_add_help[] PROGMEM =
    "Add card to list  Usage: add <card UID> <card holder name>";
const char card_add_not_possible[] PROGMEM =
    "Card length must be 8-20 even number of characters!\r\n";
const char card_remove_cmd[] PROGMEM = "rm";
const char card_remove_help[] PROGMEM =
    "Remove card from the list  Usage: rm <card UID>";
const char card_remove_no_match[] PROGMEM =
    "Entered UID does not match with any cards on the list!\r\n";
const char card_list_print_cmd[] PROGMEM = "print";
const char card_list_print_help[] PROGMEM = "Print stored list of cards";
const char card_list_empty[] PROGMEM = "Card list is empty!\r\n";
const char mem_stat_cmd[] PROGMEM = "mem";
const char mem_stat_help[] PROGMEM =
    "Print memory usage and change compared to previous call";

const char string_0[] PROGMEM = "January";
const char string_1[] PROGMEM = "February";
const char string_2[] PROGMEM = "March";
const char string_3[] PROGMEM = "April";
const char string_4[] PROGMEM = "May";
const char string_5[] PROGMEM = "June";
const char string_6[] PROGMEM = "July";
const char string_7[] PROGMEM = "August";
const char string_8[] PROGMEM = "September";
const char string_9[] PROGMEM = "October";
const char string_10[] PROGMEM = "November";
const char string_11[] PROGMEM = "December";

const PGM_P const months_table[NAME_MONTH_COUNT] PROGMEM = {
    string_0,
    string_1,
    string_2,
    string_3,
    string_4,
    string_5,
    string_6,
    string_7,
    string_8,
    string_9,
    string_10,
    string_11
};

const char row_0[] PROGMEM = "  _________________";
const char row_1[] PROGMEM = " | | ___________ |o|";
const char row_2[] PROGMEM = " | | ___________ | |";
const char row_3[] PROGMEM = " | | ___________ | |";
const char row_4[] PROGMEM = " | | ___________ | |";
const char row_5[] PROGMEM = " | |_____________| |";
const char row_6[] PROGMEM = " |     _______     |";
const char row_7[] PROGMEM = " |    |       |   ||";
const char row_8[] PROGMEM = " | DD |       |   V|";
const char row_9[] PROGMEM = " |____|_______|____|";
const char row_10[] PROGMEM = " Robert Craggs";

const PGM_P const banner[BANNER_ROW_COUNT] PROGMEM = {
    row_0,
    row_1,
    row_2,
    row_3,
    row_4,
    row_5,
    row_6,
    row_7,
    row_8,
    row_9,
    row_10,
};