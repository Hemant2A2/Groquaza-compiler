// runtime.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "runtime.hpp"

void print_int(int n) {
    printf("%d\n", n);
}

void print_float(float f) {
    printf("%f\n", f);
}

void print_string(const char *s) {
    printf("%s\n", s);
}

void print_char(char c) {
    printf("%c\n", c);
}

int read_int(void) {
    int n;
    if (scanf("%d", &n) != 1) {
        fprintf(stderr, "Error reading integer.\n");
        exit(EXIT_FAILURE);
    }
    return n;
}

float read_float(void) {
    float f;
    if (scanf("%f", &f) != 1) {
        fprintf(stderr, "Error reading float.\n");
        exit(EXIT_FAILURE);
    }
    return f;
}


void read_string(char *buffer, int size) {
    if (fgets(buffer, size, stdin) == NULL) {
        fprintf(stderr, "Error reading string.\n");
        exit(EXIT_FAILURE);
    }
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    }
}

char read_char(void) {
    char c;
    if (scanf(" %c", &c) != 1) {
        fprintf(stderr, "Error reading character.\n");
        exit(EXIT_FAILURE);
    }
    return c;
}
