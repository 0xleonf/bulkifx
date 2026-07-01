#include "./include/core.h"
#include "./include/img_proc.h"
#include <stdint.h>
#include <stdio.h>

// void check_matrices(Image *img, int target_channel, int region_size) {
//   if (!img)
//     return;
//
//   for (int y = 0; y < region_size; y++) {
//     for (int x = 0; x < region_size; x++) {
//
//       uint8_t *pixel = get_pixel(img, x, y);
//
//       if (pixel != NULL) {
//         printf("%3d ", pixel[target_channel]);
//       } else {
//         printf(" ? ");
//       }
//     }
//     printf("\n");
//   }

//   uint8_t *pixel = get_pixel(img, 2, 2);
//   double hasil = pixel[1] * 0.587;
//
//   printf("%d\n", pixel[1]);
//   printf("%f\n", hasil);
// }
//
int main() {
  const char *filename = "spon.jpg";
  Image *img = load_image(filename);

  Image *gray = grayscale(img);

  save_image("output_gray.jpg", gray);

  image_print_info(img);

  free_image(gray);
  free_image(img);

  // check_matrices(img, 0, 10);
  // printf("hello");
  return 0;
}
