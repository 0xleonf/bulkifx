#include "../include/bulkifx/padding/zero_padding.h"
#include <check.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define CHANNELS 4

START_TEST(test_zero_padding_1x1_pad1) {
  const int w = 1, h = 1;
  const short pad = 1;
  uint8_t in[4] = {0x12, 0x34, 0x56, 0x78};

  const int out_w = w + 2 * pad;
  const int out_h = h + 2 * pad;
  uint8_t out[36];
  memset(out, 0xFF, sizeof(out));

  c_zero_padding(in, out, w, h, pad);

  ck_assert_int_eq(out_w, 3);
  ck_assert_int_eq(out_h, 3);

  for (int r = 0; r < out_h; r++) {
    for (int c = 0; c < out_w; c++) {
      int idx = (r * out_w + c) * CHANNELS;
      if (r == 1 && c == 1) {
        ck_assert_mem_eq(&out[idx], in, CHANNELS);
      } else {
        uint8_t zero[4] = {0, 0, 0, 0};
        ck_assert_mem_eq(&out[idx], zero, CHANNELS);
      }
    }
  }
}
END_TEST

START_TEST(test_zero_padding_2x3_pad2) {
  const int w = 2, h = 3;
  const short pad = 2;
  uint8_t in[24];
  for (int i = 0; i < 24; i++) {
    in[i] = (uint8_t)(i + 1);
  }

  const int out_w = w + 2 * pad; /* 6 */
  const int out_h = h + 2 * pad; /* 7 */
  const int total_out = out_w * out_h * CHANNELS; /* 42 * 4 = 168 */
  uint8_t *out = malloc(total_out);
  ck_assert_ptr_nonnull(out);
  memset(out, 0xEE, total_out);

  c_zero_padding(in, out, w, h, pad);

  for (int r = 0; r < out_h; r++) {
    for (int c = 0; c < out_w; c++) {
      int out_idx = (r * out_w + c) * CHANNELS;
      if (r >= pad && r < pad + h && c >= pad && c < pad + w) {
        int in_r = r - pad;
        int in_c = c - pad;
        int in_idx = (in_r * w + in_c) * CHANNELS;
        ck_assert_mem_eq(&out[out_idx], &in[in_idx], CHANNELS);
      } else {
        uint8_t zero[4] = {0, 0, 0, 0};
        ck_assert_mem_eq(&out[out_idx], zero, CHANNELS);
      }
    }
  }

  free(out);
}
END_TEST

START_TEST(test_zero_padding_zero_pad) {
  const int w = 2, h = 2;
  const short pad = 0;
  uint8_t in[16] = {
      1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
  uint8_t out[16];
  memset(out, 0xFF, sizeof(out));

  c_zero_padding(in, out, w, h, pad);

  ck_assert_mem_eq(out, in, 16);
}
END_TEST

START_TEST(test_zero_padding_input_unmodified) {
  const int w = 2, h = 2;
  const short pad = 1;
  uint8_t in[16] = {
      10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 110, 120, 130, 140, 150, 160};
  uint8_t in_copy[16];
  memcpy(in_copy, in, sizeof(in));
  uint8_t out[64];

  c_zero_padding(in, out, w, h, pad);

  ck_assert_mem_eq(in, in_copy, 16);
}
END_TEST

Suite *pad_suite(void) {
  Suite *s = suite_create("pad");
  TCase *tc = tcase_create("core");

  tcase_add_test(tc, test_zero_padding_1x1_pad1);
  tcase_add_test(tc, test_zero_padding_2x3_pad2);
  tcase_add_test(tc, test_zero_padding_zero_pad);
  tcase_add_test(tc, test_zero_padding_input_unmodified);

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
