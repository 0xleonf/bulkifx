#include "../include/img_proc.h"
#include "../include/core.h"
#include <immintrin.h>
#include <stdint.h>
#include <stdlib.h>

Image *grayscale(Image *img) {
  if (!img->data)
    return NULL;

  Image *gray_img = create_image(img->width, img->height, 1);

  int total_pixel = (img->width * img->height);
  uint8_t *gray_pixel = (uint8_t *)malloc(total_pixel);
  uint8_t *red = (uint8_t *)malloc(total_pixel);
  uint8_t *green = (uint8_t *)malloc(total_pixel);
  uint8_t *blue = (uint8_t *)malloc(total_pixel);
  int index = 0;

  for (int i = 0; i < total_pixel * 3; i += 3) {
    red[index] = img->data[i + 0] * 0.299f;
    green[index] = img->data[i + 1] * 0.587f;
    blue[index] = img->data[i + 2] * 0.114f;

    gray_img->data[index] = gray_pixel[index] =
        (red[index] + green[index] + blue[index]);

    index++;
  }

  free(red), free(green), free(blue), free(gray_pixel);

  return gray_img;
}
