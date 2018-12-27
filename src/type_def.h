#ifndef TYPE_DEF_H
#define TYPE_DEF_H

typedef struct card {
    char *name;
    uint8_t *card_uid;
    int card_uid_len;
    struct card *next;
} card_t;

#endif /* TYPE_DEF_H */