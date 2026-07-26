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

Image *sepia(Image *img) {
  if (!img || !img->R || !img->G || !img->B) return NULL;

  Image *sepia_img = create_image(img->width, img->height, 3);

  int total_pixel = img->width * img->height;

  for (int i = 0; i < total_pixel; i++) {
    float r_val = (img->R[i] * 0.393f) + (img->G[i] * 0.769f) + (img->B[i] * 0.189f);
    float g_val = (img->R[i] * 0.349f) + (img->G[i] * 0.686f) + (img->B[i] * 0.168f);
    float b_val = (img->R[i] * 0.272f) + (img->G[i] * 0.534f) + (img->B[i] * 0.131f);

    float clamped_r = (r_val > 255.0f) ? 255.0f : r_val;
    float clamped_g = (g_val > 255.0f) ? 255.0f : g_val;
    float clamped_b = (b_val > 255.0f) ? 255.0f : b_val;

    sepia_img->R[i] = (uint8_t)clamped_r;
    sepia_img->G[i] = (uint8_t)clamped_g;
    sepia_img->B[i] = (uint8_t)clamped_b;

    sepia_img->data[3 * i] = (uint8_t)clamped_r;
    sepia_img->data[3 * i + 1] = (uint8_t)clamped_g;
    sepia_img->data[3 * i + 2] = (uint8_t)clamped_b;

  }
  return sepia_img;
}