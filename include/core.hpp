#ifndef CORE_HPP
#define CORE_HPP

#include <cstdint>

struct Image {
  int width, height;
  uint8_t* data;
};

Image *create_image();
void free_image();

Image *copy_image();
uint8_t *get_pixel();
void set_pixel();
Image *load_image();
void save_image();

#endif // !CORE_HPP
