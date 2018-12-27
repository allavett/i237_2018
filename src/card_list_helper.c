/************************************************************************/
/*         Inspired by http://www.learn-c.org/en/Linked_lists           */
/************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "print_helper.h"
#include "card_list_helper.h"
#include "../lib/andygock_avr_uart/uart.h"
#include <avr/pgmspace.h>

card_t *create_card(card_t *card, const char *card_uid, const char *name);
void remove_card(card_t **list_head_ptr, const char *rm_w_id);
void print_card_list(card_t *list_head_ptr);
uint8_t tallymarker_hextobin(const char *str, uint8_t *bytes, size_t blen);


/* Handles new card creation. Can also create a new list */
card_t *create_card(card_t *card, const char *card_uid, const char *name)
{
    int uid_s_length = strlen(card_uid);
    int uid_b_length = uid_s_length / 2;
    uint8_t bytes[uid_b_length];
    tallymarker_hextobin(card_uid, bytes, uid_s_length);
    card_t *current = card;

    do {
        if (current != NULL) {
            if (memcmp(current->card_uid, bytes, sizeof(bytes)) == 0) {
                uart0_puts_p(PSTR("Card already on the list!\r\n"));
                return NULL;
            }

            if (current->next == NULL) {
                current->next = malloc(sizeof(card_t));
                current = current->next;
                break;
            }

            current = current->next;
        } else {
            current = malloc(sizeof(card_t));
            break;
        }
    } while (current != NULL);

    if (current == NULL) {
        uart0_puts_p(PSTR("Memory operation failed\r\n"));
        free(current);
        exit(1);
    }

    current->name = malloc(strlen(name) + 1);

    if (current->name == NULL) {
        uart0_puts_p(PSTR("Memory operation failed\r\n"));
        free(current->name);
        free(current);
        exit(1);
    }

    current->card_uid = malloc(sizeof(bytes));

    if (current->card_uid == NULL) {
        uart0_puts_p(PSTR("Memory operation failed\r\n"));
        free(current->card_uid);
        free(current->name);
        free(current);
        exit(1);
    }

    current->card_uid_len = uid_b_length;
    strcpy(current->name, name);
    memcpy(current->card_uid, bytes, sizeof(bytes));
    current->next = NULL;
    return (card != NULL) ? card : current;
}


/* Remove specific card from card list */
void remove_card(card_t **list_head_ptr, const char *rm_card_uid)
{
    int uid_s_length = strlen(rm_card_uid);
    int uid_b_length = uid_s_length / 2;
    uint8_t bytes[uid_b_length];
    tallymarker_hextobin(rm_card_uid, bytes, uid_s_length);
    card_t *current = *list_head_ptr;
    card_t *prev = NULL;

    while (current != NULL) {
        if (memcmp(bytes, current->card_uid, current->card_uid_len) == 0) {
            if (prev == NULL) {
                card_t * temp_card = NULL;
                temp_card = *list_head_ptr;
                temp_card = temp_card->next;
                free((*list_head_ptr)->card_uid);
                free((*list_head_ptr)->name);
                free(*list_head_ptr);
                *list_head_ptr = temp_card;
                return;
            }
            
            prev->next = current->next;
            free(current->card_uid);
            free(current->name);
            free(current);
            return;
        }

        prev = current;
        current = current->next;
    }
}


void print_card_list(card_t *list_head_ptr)
{
    card_t *current = list_head_ptr;
    int counter = 0;
    char snum[5];
    uart0_puts_p(PSTR("Card list:\r\n"));

    while (current != NULL) {
        uart0_puts_p(PSTR("Card No. "));
        uart0_puts(itoa(++counter, snum, 10));
        uart0_puts_p(PSTR(" Card UID: "));
        print_bytes(current->card_uid, current->card_uid_len);
        uart0_puts_p(PSTR(" Card UID length: "));
        uart0_puts(itoa(current->card_uid_len, snum, 10));
        uart0_puts_p(PSTR(" Card holder: "));
        uart0_puts(current->name);
        uart0_puts_p(PSTR("\r\n"));
        current = current->next;
    }

    if (counter) {
        uart0_puts_p(PSTR("Total number of cards in the list: "));
        uart0_puts(itoa(counter, snum, 10));
        uart0_puts_p(PSTR("\r\n"));
    }
}

/*
** Code snipet taken from SO:
** https://stackoverflow.com/questions/3408706/hexadecimal-string-to-byte-array-in-c/23898449#23898449
*/
uint8_t tallymarker_hextobin(const char *str, uint8_t *bytes, size_t blen)
{
    uint8_t  pos;
    uint8_t  idx0;
    uint8_t  idx1;
    // mapping of ASCII characters to hex values
    const uint8_t hashmap[] = {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // ........
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // ........
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // ........
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // ........
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //  !"#$%&'
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // ()*+,-./
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, // 01234567
        0x08, 0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 89:;<=>?
        0x00, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x00, // @ABCDEFG
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // HIJKLMNO
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // PQRSTUVW
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // XYZ[\]^_
        0x00, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x00, // `abcdefg
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // hijklmno
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // pqrstuvw
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // xyz{|}~.
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // ........
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // ........
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // ........
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // ........
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // ........
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // ........
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // ........
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // ........
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // ........
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // ........
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // ........
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // ........
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // ........
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // ........
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // ........
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00  // ........
    };
    memset(bytes, 0, blen);

    for (pos = 0; ((pos < (blen * 2)) && (pos < strlen(str))); pos += 2) {
        idx0 = (uint8_t)str[pos + 0];
        idx1 = (uint8_t)str[pos + 1];
        bytes[pos / 2] = (uint8_t)(hashmap[idx0] << 4) | hashmap[idx1];
    };

    return (0);
}