#include "../include/bulkifx/padding/zero_padding.h"
#include <check.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

START_TEST(test_zero_padding_1x1_pad1) {
  const int w = 1, h = 1;
  const short pad = 1;
  uint8_t in[1] = {0x42};

  const int out_w = w + 2 * pad;
  const int out_h = h + 2 * pad;
  const int total_out = out_w * out_h;
  uint8_t out[9];
  memset(out, 0xFF, sizeof(out));

  zero_padding(in, out, w, h, pad);

  ck_assert_int_eq(out_w, 3);
  ck_assert_int_eq(out_h, 3);

  for (int r = 0; r < out_h; r++) {
    for (int c = 0; c < out_w; c++) {
      int idx = r * out_w + c;
      if (r == 1 && c == 1) {
        ck_assert_uint_eq(out[idx], 0x42);
      } else {
        ck_assert_uint_eq(out[idx], 0);
      }
    }
  }
}
END_TEST

START_TEST(test_zero_padding_2x3_pad2) {
  const int w = 2, h = 3;
  const short pad = 2;
  uint8_t in[6] = {10, 20, 30, 40, 50, 60};

  const int out_w = w + 2 * pad; /* 6 */
  const int out_h = h + 2 * pad; /* 7 */
  const int total_out = out_w * out_h; /* 42 */
  uint8_t *out = malloc(total_out);
  ck_assert_ptr_nonnull(out);
  memset(out, 0xEE, total_out);

  zero_padding(in, out, w, h, pad);

  for (int r = 0; r < out_h; r++) {
    for (int c = 0; c < out_w; c++) {
      int out_idx = r * out_w + c;
      if (r >= pad && r < pad + h && c >= pad && c < pad + w) {
        int in_r = r - pad;
        int in_c = c - pad;
        int in_idx = in_r * w + in_c;
        ck_assert_uint_eq(out[out_idx], in[in_idx]);
      } else {
        ck_assert_uint_eq(out[out_idx], 0);
      }
    }
  }

  free(out);
}
END_TEST

START_TEST(test_zero_padding_zero_pad) {
  const int w = 2, h = 2;
  const short pad = 0;
  uint8_t in[4] = {1, 2, 3, 4};
  uint8_t out[4] = {0xFF, 0xFF, 0xFF, 0xFF};

  zero_padding(in, out, w, h, pad);

  ck_assert_mem_eq(out, in, 4);
}
END_TEST

START_TEST(test_zero_padding_input_unmodified) {
  const int w = 2, h = 2;
  const short pad = 1;
  uint8_t in[4] = {10, 20, 30, 40};
  uint8_t in_copy[4] = {10, 20, 30, 40};
  uint8_t out[16];

  zero_padding(in, out, w, h, pad);

  ck_assert_mem_eq(in, in_copy, 4);
}
END_TEST

START_TEST(test_zero_padding_aliases) {
  const int w = 1, h = 1;
  const short pad = 1;
  uint8_t in[1] = {99};
  uint8_t out1[9], out2[9], out3[9];

  zero_padding(in, out1, w, h, pad);
  c_zeropadding(in, out2, w, h, pad);
  c_padding(in, out3, w, h, pad);

  ck_assert_mem_eq(out1, out2, 9);
  ck_assert_mem_eq(out1, out3, 9);
}
END_TEST

Suite *pad_suite(void) {
  Suite *s = suite_create("pad");
  TCase *tc = tcase_create("core");

  tcase_add_test(tc, test_zero_padding_1x1_pad1);
  tcase_add_test(tc, test_zero_padding_2x3_pad2);
  tcase_add_test(tc, test_zero_padding_zero_pad);
  tcase_add_test(tc, test_zero_padding_input_unmodified);
  tcase_add_test(tc, test_zero_padding_aliases);

  suite_add_tcase(s, tc);
  return s;
}

int main(void) {
  int n_failed;
  Suite *s = pad_suite();
  SRunner *sr = srunner_create(s);

  srunner_run_all(sr, CK_VERBOSE);
  n_failed = srunner_ntests_failed(sr);
  srunner_free(sr);

  return (n_failed == 0) ? 0 : 1;
}
