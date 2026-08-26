#include "../include/core.h"
#include <check.h>
#include <stdint.h>
#include <stdlib.h>

START_TEST(test_zero_padding) {
  Image *input = malloc(sizeof(Image));
  input->width = 2;
  input->height = 2;
  input->R = malloc(4 * sizeof(uint8_t));

  uint8_t raw_input[4] = {1, 2, 3, 4};
  for (int i = 0; i < 4; i++) {
    input->R[i] = raw_input[i];
  }

  uint8_t expected_pixel[16] = {0, 0, 0, 0, 0, 1, 2, 0, 0, 3, 4, 0, 0, 0, 0, 0};

  Matrix *output = zero_padding(input, input->R, 1);

  ck_assert_ptr_nonnull(output);
  ck_assert_ptr_nonnull(output->pixel);

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      ck_assert_uint_eq(output->pixel[i * 2 + j], expected_pixel[i * 2 + j]);
    }
  }
}
END_TEST

START_TEST(test_zero_padding_not_squre) {
  Image *input = malloc(sizeof(Image));
  input->width = 3;
  input->height = 2;
  input->R = malloc(6 * sizeof(uint8_t));

  int8_t raw_input[6] = {1, 2, 3, 4, 5, 6};
  for (int i = 0; i < 6; i++) {
    input->R[i] = raw_input[i];
  }

  int8_t expected_pixel[20] = {0, 0, 0, 0, 0, 0, 1, 2, 3, 0,
                               0, 4, 5, 6, 0, 0, 0, 0, 0, 0};

  Matrix *output = zero_padding(input, input->R, 1);

  ck_assert_ptr_nonnull(output);
  ck_assert_ptr_nonnull(output->pixel);

  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 4; j++) {
      ck_assert_int_eq(output->pixel[i * 3 + j], expected_pixel[i * 3 + j]);
    }
  }
}
END_TEST

Suite *padding_suite(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("Zero padding");
  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_zero_padding);
  tcase_add_test(tc_core, test_zero_padding_not_squre);
  suite_add_tcase(s, tc_core);

  return s;
}

int main() {
  int total_failed;
  Suite *s;
  SRunner *sr;

  s = padding_suite();
  sr = srunner_create(s);

  srunner_run_all(sr, CK_VERBOSE);
  total_failed = srunner_ntests_failed(sr);
  srunner_free(sr);

  return (total_failed == 0) ? 0 : 1;
}
