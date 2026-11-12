
#include <stdint.h>
#include <string.h>
void c_zero_padding(uint8_t *input, uint8_t *output, int w, int h,
                    short padding) {
  int out_w = w + 2 * (padding);
  int out_h = h + 2 * (padding);

  memset(output, 0, out_w * out_h * 4);

  for (int row = 0; row < h; row++) {
    int in_offset = row * w * 4;
    int out_offset = ((row + padding) * out_w + padding) * 4;
    memcpy(output + out_offset, input + in_offset, w * 4);
  }
}
