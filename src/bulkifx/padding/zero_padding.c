
#include <stdint.h>
void zero_padding(uint8_t *input, uint8_t *output, int w, int h,
                  short padding) {
  int out_w = w + 2 * (padding);
  int out_h = h + 2 * (padding);

  int total_out = out_w * out_h;
  for (int i = 0; i < total_out; i++) {
    output[i] = 0;
  }

  for (int row = 0; row < h; row++) {
    for (int col = 0; col < w; col++) {
      int in_idx = row * w + col;

      int out_row = row + padding;
      int out_col = col + padding;
      int out_idx = out_row * out_w + out_col;
      output[out_idx] = input[in_idx];
    }
  }
}
