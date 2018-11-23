#ifndef PRINT_HELPER_H
#define PRINT_HELPER_H
#include <stdio.h>

void print_banner_P(void (*puts_P_function)(const char*), const char * const *banner, const uint8_t rows);

#endif /* PRINT_HELPER_H */