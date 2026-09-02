#include "../../../include/bulkifx/grayscale/grayscale.h"
#include <stdint.h>

void c_grayscale(const uint8_t *input, uint8_t *output, int width, int height) {
  int total_pixel = height * width;
  for (int i = 0; i < total_pixel * 4; i += 4) {
    uint8_t gray = (uint8_t)((19595 * input[i] + 38470 * input[i+1] + 7471 * input[i+2]) >> 16);
    output[i] = gray;
    output[i + 1] = gray;
    output[i + 2] = gray;
    output[i + 3] = input[i + 3];
  }
}
