#include <avr/io.h>
#include <string.h>
#include <time.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include "../lib/matejx_avr_lib/mfrc522.h"
#include "type_def.h"
#include "card_list_helper.h"
#include "cli_microrl.h"
#include "hmi.h"
#include "../lib/hd44780_111/hd44780.h"


#define DOOR_OPEN_IN_SEC    3
#define DISPLAY_MSG_IN_SEC  2

typedef enum {
    door_opening,
    door_open,
    door_closing,
    door_closed,
    no_card
} door_state_t;

typedef enum {
    display_name,
    display_access_denied,
    display_clear,
    display_no_update,
} display_state_t;

byte BUFFER[2];
door_state_t door_state = door_closing;
display_state_t display_state = display_no_update;
time_t time_y2k_cpy, door_open_time, msg_display_time;
 

door_state_t rfid_card_read (void)
{
    Uid uid;
    Uid *card_uid_ptr = &uid;
    byte bufferSize = 2;
    door_state_t door_state;
    card_t *card_list = card_list_ptr;

    if (card_list != NULL) {
        if (PICC_IsNewCardPresent()) {
            PICC_ReadCardSerial(card_uid_ptr);
            PICC_WakeupA(BUFFER, &bufferSize);

            while (card_list != NULL) {
                if (memcmp(uid.uidByte, card_list->card_uid, uid.size) == 0) {
                    card_holder = card_list->name;
                    door_state = door_opening;
                    return door_state;
                }

                card_list = card_list->next;
            }

            door_state = door_closed;
            return door_state;
        }
    }

    door_state = no_card;
    return door_state;
}

char *get_card_holder (void)
{
    return card_holder;
}

void door_control(void)
{
    char *display_name_str = NULL;
    char lcd_buf[16];

    switch (door_state) {
        case door_opening:
        // Document door open time
        door_open_time = time(NULL);
        // Get holder name
        display_name_str = get_card_holder();
        display_state = display_name;
        PORTA |= _BV(LED_BLUE);
        PORTA &= ~_BV(LED_RED);
        // Unlock door
        door_state = door_open;
        break;

        case door_open:
        display_state = display_no_update;

        if (rfid_card_read() == door_opening) {
            if (get_card_holder() != display_name_str) {
                door_state = door_opening;
            }

            //reset timer
            door_open_time = time(NULL);
            } else if (rfid_card_read() == door_closed) {
            door_state = door_closing;
            display_state = display_access_denied;
        }

        // Get current time
        time_y2k_cpy = time(NULL);

        if ((time_y2k_cpy - door_open_time) > DOOR_OPEN_IN_SEC) {
            door_state = door_closing;
        }

        break;

        case door_closing:
        display_state = display_clear;
        // Lock door
        door_state = door_closed;
        PORTA &= ~_BV(LED_BLUE);
        PORTA |= _BV(LED_RED);
        break;

        case door_closed:
        door_state = no_card;
        break;

        case no_card:
        // Ping reader for new state
        door_state = rfid_card_read();

        if (door_state == door_closed) {
            door_state = door_closing;
            display_state = display_access_denied;
        }

        break;
    }

    switch (display_state) {
        case display_name:
        lcd_clr(LCD_ROW_2_START, LCD_VISIBLE_COLS);
        lcd_goto(LCD_ROW_2_START);

        // You need to point display_name_str pointer to read card struct card holder name
        if (display_name_str != NULL) {
            // You need to truncate name display because name can be long
            strncpy(lcd_buf, display_name_str, LCD_VISIBLE_COLS);
            lcd_puts(lcd_buf);
            } else {
            lcd_puts_P(PSTR("Name read error"));
        }

        msg_display_time = time(NULL);
        display_state = display_clear;
        break;

        case display_access_denied:
        lcd_clr(LCD_ROW_2_START, LCD_VISIBLE_COLS);
        lcd_goto(LCD_ROW_2_START);
        lcd_puts_P(PSTR("Access denied!"));
        msg_display_time = time(NULL);
        display_state = display_clear;
        break;

        case display_clear:
        time_y2k_cpy = time(NULL);

        if ((time_y2k_cpy - msg_display_time) > DISPLAY_MSG_IN_SEC) {
            lcd_clr(LCD_ROW_2_START, LCD_VISIBLE_COLS);
            display_state = display_no_update;
        }

        break;

        case display_no_update:
        break;
    }
}