#include "../include/img_proc.h"
#include "../include/core.h"
#include <stdint.h>
#include <stdlib.h>

Image *grayscale(Image *img) {
  if (!img->data)
    return NULL;

  Image *gray_img = create_image(img->width, img->height, 1);

  for (int y = 0; y < img->height; y++) {
    for (int x = 0; x < img->width; x++) {

      uint8_t *src_pixel = get_pixel(img, x, y);

      uint8_t *dst_pixel = get_pixel(gray_img, x, y);

      if (src_pixel && dst_pixel) {
        uint8_t red = src_pixel[0], green = src_pixel[1], blue = src_pixel[2];

        dst_pixel[0] = ((0.299 * red) + (0.587 * green) + (0.114 * blue) + 0.5);
      }
    }
  }

  return gray_img;
}
