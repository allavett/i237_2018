#ifndef HMI_H
#define HMI_H

#define BLINK_DELAY_MS 10
#define STUDENT_NAME "Allar Vendla\n"
#define VER_FV "Version: " FW_VERSION " built on: " __DATE__ " " __TIME__ "\n"
#define VER_LIBC "avr-libc version: " __AVR_LIBC_VERSION_STRING__ " avr-gcc version: " __VERSION__ "\n"
#define INPUT_ASK_LETTER "Enter Month name first letter >"
#define NAME_MONTH_COUNT 12
#define BANNER_ROW_COUNT 11

const unsigned char string_0[] PROGMEM = "January";
const unsigned char string_1[] PROGMEM = "February";
const unsigned char string_2[] PROGMEM = "March";
const unsigned char string_3[] PROGMEM = "April";
const unsigned char string_4[] PROGMEM = "May";
const unsigned char string_5[] PROGMEM = "June";
const unsigned char string_6[] PROGMEM = "July";
const unsigned char string_7[] PROGMEM = "August";
const unsigned char string_8[] PROGMEM = "September";
const unsigned char string_9[] PROGMEM = "October";
const unsigned char string_10[] PROGMEM = "November";
const unsigned char string_11[] PROGMEM = "December";

unsigned PGM_P const months_table[NAME_MONTH_COUNT] PROGMEM = {
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

#endif /* HMI_H */