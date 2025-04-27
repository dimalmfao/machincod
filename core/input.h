#ifndef _INPUT_H
#define _INPUT_H

#include <stdint.h>
#include <stddef.h>

// Function declarations
size_t _internal_read(char *ptr, unsigned int len);
void _internal_flush(void);

// Other declarations
int64_t input_integer();
char input_char();
char input_byte();

#endif
