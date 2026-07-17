#include "../include/core.h"
#include "../include/utils.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STB_IMAGE_IMPLEMENTATION
#include "../include/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../include/stb_image_write.h"

Image *create_image(int width, int height, int channels) {
  Image *img = malloc(sizeof(Image));

  if (!img)
    return NULL;

  img->width = width;
  img->height = height;
  img->channels = channels;

  size_t data_size = width * height * sizeof(uint8_t);
  img->R = malloc(data_size);
  img->G = malloc(data_size);
  img->B = malloc(data_size);

  img->data = (uint8_t *)malloc(data_size * channels);

  if (!img->R || !img->G || !img->B) {
    free(img->R), free(img->G), free(img->B), free(img->data);
    free(img);
    return NULL;
  }

  memset(img->R, 0, data_size);
  memset(img->G, 0, data_size);
  memset(img->B, 0, data_size);
  return img;
}

void free_image(Image *img) {
  if (img) {

    if (img->R || img->G || img->B || img->data)
      free(img->R), free(img->G), free(img->B), free(img->data);

    free(img);
  }
}

uint8_t *get_pixel(Image *img, int x, int y) {
  if (!img || (!img->R && !img->G && !img->B))
    return NULL;

  if (x < 0 || x >= img->width || y < 0 || y >= img->height)
    return NULL;

  return &img->data[(y * img->width + x) * img->channels];
}

void stored_pixel(Image *img, int total_pixel) {
  for (int i = 0; i < total_pixel; i++) {
    img->data[3 * i] = img->R[i];
    img->data[3 * i + 1] = img->G[i];
    img->data[3 * i + 2] = img->B[i];
  }
}

void zero_padding(uint8_t *channel, int width, int height, Kernel *kernel) {
  int radius = kernel->capacity / 2;
  width += (2 * radius);
  height += (2 * radius);

  channel = calloc(width * height, sizeof(Image));
}

Kernel *blur_kernel(Kernel *kernel, int capacity) {
  float sum = 0.0f;
  int radius = capacity / 2;
  kernel->capacity = capacity;

  for (int i = 0; i < capacity; i++) {
    int row = i - radius;
    kernel->values[i] = gauss(i, radius);
  }
  return kernel;
}

void convolution(unsigned char *input, unsigned char *output, int width,
                 int height, float *kernel, int capacity) {
  int radius = capacity / 2;

  Kernel *k = malloc(sizeof(Kernel));

  blur_kernel(k, capacity);

  float *temp = malloc(width * height * sizeof(float));
  if (!temp)
    return;

  // horizontal
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      float sum = 0.0f;

      for (int kr = 0; kr < capacity; kr++) {
        int pixel_x = clamp(x + (kr - radius), 0, width - 1);
        sum += input[y * width + pixel_x] * k->values[kr];
      }
      temp[y * width + x] = sum;
    }
  }

  // vertical
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      float sum = 0.0f;

      for (int kr = 0; kr < capacity; kr++) {
        int pixel_y = clamp(y + (kr - radius), 0, width - 1);
        sum += temp[pixel_y * width + x];
      }
      int final_value = (int)(sum + 0.5f);
      output[y * width + x] = (unsigned char)clamp(final_value, 0, 255);
    }
  }

  free(temp);
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

  int total_pixel = width * height;

  for (int i = 0; i < total_pixel; i++) {
    img->R[i] = stb_data[3 * i];
    img->G[i] = stb_data[3 * i + 1];
    img->B[i] = stb_data[3 * i + 2];
  }

  stored_pixel(img, total_pixel);

  stbi_image_free(stb_data);
  return img;
}

void save_image(const char *filename, Image *img) {
  if (!img->R || !img->G || !img->B) {
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
