#ifndef HMI_H
#define HMI_H

#define VER_FV "Version: " FW_VERSION " built on: " __DATE__ " " __TIME__ "\n"
#define VER_LIBC "avr-libc version: " __AVR_LIBC_VERSION_STRING__ " avr-gcc version: " __VERSION__ "\n"
#define BLINK_DELAY_MS 10
#define NAME_MONTH_COUNT 12
#define BANNER_ROW_COUNT 11

extern const char student_name[];
extern const char input_ask_letter[];
extern const PGM_P const months_table[];
extern const PGM_P const banner[];

#endif /* HMI_H */