#ifndef HMI_H
#define HMI_H

#define LED_RED     PA1 // Arduino Mega digital pin 23
#define LED_GREEN   PA3 // Arduino Mega digital pin 25
#define LED_BLUE    PA5 // Arduino Mega digital pin 27
#define LED_YELLOW  DDB7 // Arduino Mega on-board LED
#define BLINK_DELAY_MS      10
#define NAME_MONTH_COUNT    12
#define BANNER_ROW_COUNT    11
#define UART_STATUS_MASK    0x00FF

extern const char ver_fw[] PROGMEM;
extern const char ver_libc[] PROGMEM;
extern const char console_started[];
extern const char student_name[];
extern const char cmd_intro[] PROGMEM;
extern const char input_ask_letter[];
extern const PGM_P const months_table[];
extern const PGM_P const banner[];
extern const char cmd_error[] PROGMEM;
extern const char cmd_arg_error[] PROGMEM;
extern const char help[] PROGMEM;
extern const char help_cmd[] PROGMEM;
extern const char help_help[] PROGMEM;
extern const char example_cmd[] PROGMEM;
extern const char example_help[] PROGMEM;
extern const char example_print[] PROGMEM;
extern const char ver_cmd[] PROGMEM;
extern const char ver_help[] PROGMEM;
extern const char banner_cmd[] PROGMEM;
extern const char banner_help[] PROGMEM;
extern const char month_cmd[] PROGMEM;
extern const char month_help[] PROGMEM;

#endif /* HMI_H */