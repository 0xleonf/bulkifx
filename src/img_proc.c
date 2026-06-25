#include "../include/img_proc.h"
#include "../include/core.h"
#include <immintrin.h>
#include <stdint.h>
#include <stdlib.h>

Image *grayscale(Image *img) {
  if (!img || !img->R || !img->G || !img->B)
    return NULL;

  Image *gray_img = create_image(img->width, img->height, 1);

  int total_pixel = (img->width * img->height);
  uint8_t *red = (uint8_t *)malloc(total_pixel);
  uint8_t *green = (uint8_t *)malloc(total_pixel);
  uint8_t *blue = (uint8_t *)malloc(total_pixel);

  for (int i = 0; i < total_pixel; i++) {
    red[i] = (img->R[i] * 0.299f);
    green[i] = (img->G[i] * 0.587f);
    blue[i] = (img->B[i] * 0.114f);
 
    gray_img->data[i] = (uint8_t)(red[i] + green[i] + blue[i]); 
  } 

  free(red), free(green), free(blue);

  return gray_img;
}
