#include <avr/pgmspace.h>
#include "hmi.h"

const char student_name[] PROGMEM = "Allar Vendla";
const char input_ask_letter[] PROGMEM = "Enter Month name first letter >";

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