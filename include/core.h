#ifndef CORE_H
#define CORE_H

#include <stdint.h>

typedef struct {
  int width;
  int height;
  int channels;
  uint8_t *data;
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
 * I/O Operations
 */
Image *load_image(const char *filename);
void save_image(const char *filename, const Image *img);

void image_print_info(const Image *img);

#endif // !CORE_H
