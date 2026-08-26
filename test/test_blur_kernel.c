#include "../include/core.h"
#include <check.h>
#include <stdint.h>
#include <stdlib.h>

START_TEST(test_blur_kernel_3x3) {
  int8_t expected_pixel[3] = {1, 2, 1};

  Kernel *output = blur_kernel(3);

  for (int i = 0; i < 3; i++) {
    ck_assert_int_eq(output->value[i], expected_pixel[i]);
  }
}
END_TEST

START_TEST(test_blur_kernel_5x5) {
  int8_t expected_pixel[5] = {1, 4, 6, 4, 1};

  Kernel *output = blur_kernel(5);

  for (int i = 0; i < 5; i++) {
    ck_assert_int_eq(output->value[i], expected_pixel[i]);
  }
}
END_TEST

Suite *padding_suite(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("Blur Kernel");
  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_blur_kernel_3x3);
  tcase_add_test(tc_core, test_blur_kernel_5x5);
  suite_add_tcase(s, tc_core);

  return s;
}

int main() {
  int total_failed;
  Suite *s;
  SRunner *sr;

  s = padding_suite();
  sr = srunner_create(s);

  srunner_run_all(sr, CK_NORMAL);
  total_failed = srunner_ntests_failed(sr);
  srunner_free(sr);

  return (total_failed == 0) ? 0 : 1;
}
