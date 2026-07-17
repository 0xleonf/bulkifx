#ifndef CORE_H
#define CORE_H

#include <stdint.h>

typedef struct {
  uint8_t *R;
  uint8_t *G;
  uint8_t *B;
  // uint8_t *A;
  uint8_t *data;

  int width;
  int height;
  int channels;
} Image;

typedef struct {
  float *values;
  int capacity;
} Kernel;

/*
 * @brief
 * Memory management
 */
Image *create_image(int width, int height, int channels);
void free_image(Image *img);
Image *copy_image(const Image *src);

/*
 * @brief
 * Pixel access
 */
uint8_t *get_pixel(Image *img, int x, int y);
void set_pixel(Image *img, int x, int y, uint8_t r, uint8_t g, uint8_t b);

/*
 * @brief
 * Help me!!!!
 * */
void stored_pixel(Image *img, int total_pixel);
void zero_padding(uint8_t *channel, int width, int height, Kernel *kernel);
Kernel *blur_kernel(Kernel *kernel, int capacity);
void convolution(unsigned char *input, unsigned char *output, int width,
                 int height, float *kernel, int capacity);

/*
 * @brief
 * I/O Operations
 */
Image *load_image(const char *filename);
void save_image(const char *filename, Image *img);
void image_print_info(const Image *img);

#endif // !CORE_H
