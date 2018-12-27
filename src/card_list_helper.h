#ifndef CARD_LIST_HELPER_H
#define CARD_LIST_HELPER_H

extern card_t *create_card(card_t *card, const char *card_uid, const char *name);
extern void remove_card(card_t **list_head_ptr, const char *rm_card_uid);
extern void print_card_list(card_t *list_head_ptr);
//extern uint8_t tallymarker_hextobin(const char *str, uint8_t *bytes, size_t blen);

#endif /* CARD_LIST_HELPER_H */