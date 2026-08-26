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

  size_t data_size = width * height * sizeof(int8_t);
  img->R = malloc(data_size);
  img->G = malloc(data_size);
  img->B = malloc(data_size);

  img->data = malloc(data_size * channels * sizeof(int8_t));

  if (!img->R || !img->G || !img->B || !img->data) {
    free(img);
    free(img->R);
    free(img->G);
    free(img->B);
    free(img->data);
    return NULL;
  }

  // img->R = calloc(data_size, sizeof(int8_t));
  // img->G = calloc(data_size, sizeof(int8_t));
  // img->B = calloc(data_size, sizeof(int8_t));

  return img;
}

void free_image(Image *img) {
  if (img) {

    if (img->R || img->G || img->B || img->data)
      free(img->R), free(img->G), free(img->B), free(img->data);

    free(img);
  }
}

int8_t *get_pixel(Image *img, int x, int y) {
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

Matrix *zero_padding(Image *img, int8_t *input, int pad_width) {
  int pad = pad_width * 2;
  Matrix *output = malloc(sizeof(Matrix));
  output->width = (img->width + pad);
  output->height = (img->height + pad);
  output->pixel = calloc((output->width) * (output->height), sizeof(int8_t));

  for (int y = 0; y < img->height; y++) {
    for (int x = 0; x < img->width; x++) {
      output->pixel[(y + pad_width) * output->width + (x + pad_width)] =
          input[y * img->width + x];
    }
  }

  return output;
}

Matrix *convolute_horizontal(Image *img, int width_pad, int height_pad,
                             int8_t *input, int8_t *filter, int filter_len) {
  Matrix *output = malloc(sizeof(Matrix));
  int new_height = img->height;
  int new_width = img->width - filter_len + 1;
  output->width = new_width;
  output->height = new_height;
  output->pixel = calloc(new_height * new_width, sizeof(int8_t));

  // horizontal
  for (int y = 0; y < new_height; y++) {
    for (int x = 0; x < new_width; x++) {
      int out_idx = y * new_width + x;
      int sum = 0;
      for (int i = 0; i < filter_len; i++) {
        int in_idx = y * width_pad + (x + i);
        sum += input[in_idx] * filter[i];
      }
      output->pixel[out_idx] = sum;
    }
  }

  return output;
}

Matrix *convolute_vertical(Image *img, int raw_width, int8_t *input,
                           int8_t *filter, int filter_len) {
  Matrix *output = malloc(sizeof(Matrix));
  int new_height = img->height - filter_len + 1;
  int new_width = img->width;
  output->width = new_width;
  output->height = new_height;
  output->pixel = calloc(new_height * new_width, sizeof(int8_t));

  for (int y = 0; y < new_height; y++) {
    for (int x = 0; x < new_width; x++) {
      int out_idx = y * new_width + x;
      int sum = 0;
      for (int i = 0; i < filter_len; i++) {
        int in_idx = (y + i) * raw_width + x;
        sum += input[in_idx] * filter[i];
      }
      output->pixel[out_idx] = sum;
    }
  }

  return output;
}

Kernel *blur_kernel(int ksize) {
  Kernel *filter;
  filter = malloc(sizeof(Kernel));
  filter->value = malloc(ksize * sizeof(int8_t));
  filter->ksize = ksize;

  if (ksize <= 0) {
    filter->value = malloc(0 * sizeof(int8_t));
    return filter;
  }

  int n = ksize - 1;
  int curr = 1;
  for (int i = 0; i < ksize; i++) {
    filter->value[i] = 1;
  }

  for (int k = 1; k < (ksize); k++) {
    curr = curr * (n - k + 1) / k;
    filter->value[k] = curr;
  }

  return filter;
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
                               img->data, 90);
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
