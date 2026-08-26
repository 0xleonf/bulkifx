#ifndef CORE_H
#define CORE_H

#include <stdint.h>

typedef struct {
  int8_t *R;
  int8_t *G;
  int8_t *B;
  // uint8_t *A;
  int8_t *data;

  int width;
  int height;
  int channels;
} Image;

typedef struct {
  int8_t *pixel;

  int width;
  int height;
} Matrix;

typedef struct {
  int8_t *value;
  int ksize;
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
int8_t *get_pixel(Image *img, int x, int y);
void set_pixel(Image *img, int x, int y, int8_t r, int8_t g, int8_t b);

/*
 * @brief
 * Help me!!!!
 * */
void stored_pixel(Image *img, int total_pixel);
// void zero_padding(uint8_t *red_ch, uint8_t *green_ch, uint8_t *blue_ch,
//                   Image *img, int width, int height, int kernel_size);
Matrix *zero_padding(Image *img, int8_t *input, int pad_width);
Matrix *convolute_horizontal(Image *img, int width_pad, int height_pad,
                             int8_t *input, int8_t *filter, int filter_len);
Matrix *convolute_vertical(Image *img, int raw_width, int8_t *input,
                           int8_t *filter, int filter_len);
Kernel *blur_kernel(int ksize);
void convolution(Image *input, Image *output, int width, int height,
                 float *kernel, int capacity);

/*
 * @brief
 * I/O Operations
 */
Image *load_image(const char *filename);
void save_image(const char *filename, Image *img);
void image_print_info(const Image *img);

#endif // !CORE_H
