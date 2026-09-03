
#include <stdint.h>
#ifndef SEPIA_H
#define SEPIA_H

static uint8_t clamp_u8(int value);
void c_sepia(const uint8_t *input, uint8_t *output, int width, int height);

#endif /* SEPIA_H */
