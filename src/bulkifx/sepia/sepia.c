#include "../../../include/bulkifx/sepia/sepia.h"
#include <stdint.h>

static uint8_t clamp_u8(int value) {
  if (value > 255) return 255;
  if (value < 0) return 0;
  return (uint8_t)value;
}

void c_sepia(const uint8_t *input, uint8_t *output, int width, int height) {
  int total_pixel = height * width;

  for (int i = 0; i < total_pixel * 4; i += 4) {
    uint8_t red =
        clamp_u8((input[i] * 101 + input[i + 1] * 197 + input[i + 2] * 48) >> 8);
    uint8_t green =
        clamp_u8((input[i] * 89 + input[i + 1] * 176 + input[i + 2] * 43) >> 8);
    uint8_t blue =
        clamp_u8((input[i] * 70 + input[i + 1] * 137 + input[i + 2] * 34) >> 8);
    output[i] = red;
    output[i + 1] = green;
    output[i + 2] = blue;
    output[i + 3] = input[i + 3];
  }
}
