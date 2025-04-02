#pragma once

#ifdef __cplusplus
extern "C" {
#endif

void print_int(int n);
void print_float(float f);
void print_string(const char *s);
void print_char(char c);

int read_int(void);
float read_float(void);
void read_string(char *buffer, int size);
char read_char(void);

#ifdef __cplusplus
}
#endif
