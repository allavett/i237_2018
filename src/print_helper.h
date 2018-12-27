#ifndef PRINT_HELPER_H
#define PRINT_HELPER_H
#include <stdio.h>

extern void print_banner_P(void (*puts_P_function)(const char*), const char * const *banner, const uint8_t rows);
extern void print_bytes(const uint8_t *array, const size_t len);

#endif /* PRINT_HELPER_H */