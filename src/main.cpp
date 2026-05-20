#include "../include/core.hpp"
#include <iostream>

int main() {

  const char *filename = "spiderman.jpg";
  Image *img = load_image(filename);

  // std::cout << "number of arguments" << argc << std::endl;
  //
  // for (int i = 0; i < argc; i++)
  // {
  //   std::cout << "argv[" << i << "]" << argv[i] << std::endl;
  // }
  image_print_info(img);
  return 0;
}
