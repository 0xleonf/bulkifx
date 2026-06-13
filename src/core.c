#include "../include/core.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STB_IMAGE_IMPLEMENTATION
#include "../include/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../include/stb_image_write.h"

Image *create_image(int width, int height, int channels) {
  Image *img = (Image *)malloc(sizeof(Image));

  if (!img)
    return NULL;

  img->width = width;
  img->height = height;
  img->channels = channels;

  size_t data_size = width * height * channels * sizeof(uint8_t);
  img->data = (uint8_t *)malloc(data_size);

  if (!img->data) {
    free(img);
    return NULL;
  }

  memset(img->data, 0, data_size);
  return img;
}

void image_free(Image *img) {
  if (img) {
    if (img->data)
      free(img->data);
    free(img);
  }
}

uint8_t *get_pixel(Image *img, int x, int y) {
  if (!img || !img->data)
    return NULL;

  if (x < 0 || x >= img->width || y < 0 || y >= img->height)
    return NULL;

  return &img->data[(y * img->width + x) * img->channels];
}

Image *load_image(const char *filename) {
  int width;
  int height;
  int channels;

  unsigned char *stb_data = stbi_load(filename, &width, &height, &channels, 0);
  if (!stb_data) {
    fprintf(stderr, "Error, failed to load %s \n", filename);
    return NULL;
  }

  Image *img = create_image(width, height, 3);
  if (!img) {
    stbi_image_free(stb_data);
    return NULL;
  }

  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      int src_idx = (y * width + x) * channels;
      uint8_t *dst = get_pixel(img, x, y);

      if (channels >= 3) {
        dst[0] = stb_data[src_idx];
        dst[1] = stb_data[src_idx + 1];
        dst[2] = stb_data[src_idx + 2];
      } else if (channels == 1) {
        dst[0] = dst[1] = dst[3] = stb_data[src_idx];
      }
    }
  }

  stbi_image_free(stb_data);
  return img;
}

void save_image(const char *filename, const Image *img) {
  if (!img->data) {
    fprintf(stderr, "Error: no image to save!");
    return;
  }

  int stride_in_bytes = img->width * img->channels;

  int success = stbi_write_jpg(filename, img->width, img->height, img->channels,
                               img->data, stride_in_bytes);
  if (success) {
    printf("Succeed: Image successfully saved '%s'\n", filename);
  } else {
    fprintf(stderr, "Error: Failed to write image %s\n", filename);
  }
}

void image_print_info(const Image *img) {
  if (!img) {
    printf("Image: NULL\n");
    return;
  }

  printf("=== Image Info ===\n");
  printf("Width:    %d\n", img->width);
  printf("Height:   %d\n", img->height);
  printf("Channels: %d\n", img->channels);
  printf("Data size: %zu bytes\n",
         (size_t)img->width * img->height * img->channels);
  printf("=================\n");
}
