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
 * Stored result into single array
 * */
void stored_pixel(Image *img, int total_pixel);

/*
 * @brief
 * I/O Operations
 */
Image *load_image(const char *filename);
void save_image(const char *filename, Image *img);
void image_print_info(const Image *img);

#endif // !CORE_H
