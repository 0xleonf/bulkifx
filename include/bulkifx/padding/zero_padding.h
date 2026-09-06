#ifndef ZERO_PADDING_H
#define ZERO_PADDING_H
#include <stdint.h>

void zero_padding(uint8_t *input, uint8_t *output, int w, int h, short padding);

#define c_zeropadding zero_padding
#define c_padding zero_padding

#endif // !ZERO_PADDING_H
