#include "d3_decimal.h"
#include "help_func.h"
int d3_add(d3_decimal value_1, d3_decimal value_2, d3_decimal* result) {
  d3_big_decimal big_value_1 = {0}, big_value_2 = {0}, big_result = {0};
  int sign_1 = get_sign(value_1), error;
  int sign_2 = get_sign(value_2);
  decimal_zeroing(result);
  decimal_to_big_decimal(value_1, &big_value_1);
  decimal_to_big_decimal(value_2, &big_value_2);
  d3_normalization(&big_value_1, &big_value_2);
  set_big_bit(&big_value_2, 223, 0);
  set_big_bit(&big_value_1, 223, 0);
  if (sign_1 && sign_2) {
    d3_add_big(big_value_1, big_value_2, &big_result);
    set_big_sign(&big_result);
  }
  if (sign_1 == 0 && sign_2) {
    if (d3_big_is_less(big_value_1, big_value_2)) {
      d3_sub_big(big_value_2, big_value_1, &big_result);
      if (d3_all_big_zero(big_result) == 0) set_big_sign(&big_result);
    } else
      d3_sub_big(big_value_1, big_value_2, &big_result);
  }
  if (sign_1 && sign_2 == 0) {
    if (d3_big_is_less(big_value_1, big_value_2)) {
      d3_sub_big(big_value_2, big_value_1, &big_result);
    } else {
      d3_sub_big(big_value_1, big_value_2, &big_result);
      if (d3_all_big_zero(big_result) == 0) set_big_sign(&big_result);
    }
  }
  if (sign_1 == 0 && sign_2 == 0) {
    d3_add_big(big_value_1, big_value_2, &big_result);
    if (d3_all_zero(value_1) == 0 && d3_all_zero(value_2)) {
      big_result = big_value_1;
    }
  }
  set_big_scale(&big_result, get_big_scale(big_value_1));
  error = big_decimal_to_decimal(big_result, result);
  return error;
}
int d3_sub(d3_decimal value_1, d3_decimal value_2, d3_decimal* result) {
  int borrow, sign_1 = get_sign(value_1), sign_2 = get_sign(value_2);
  d3_big_decimal big_value_1 = {0}, big_value_2 = {0}, big_result = {0};
  decimal_to_big_decimal(value_1, &big_value_1);
  decimal_to_big_decimal(value_2, &big_value_2);
  decimal_zeroing(result);
  set_big_bit(&big_value_1, 223, 0);
  set_big_bit(&big_value_2, 223, 0);
  d3_normalization(&big_value_1, &big_value_2);
  if (d3_all_big_zero(big_result)) {
    if (sign_1 && sign_2) {
      if (d3_big_is_less(big_value_1, big_value_2)) {
        d3_sub_big(big_value_2, big_value_1, &big_result);
      } else {
        d3_sub_big(big_value_1, big_value_2, &big_result);
        set_big_sign(&big_result);
      }
    }
    if (sign_1 == 0 && sign_2) {
      d3_add_big(big_value_1, big_value_2, &big_result);
    }
    if (sign_1 && sign_2 == 0) {
      d3_add_big(big_value_1, big_value_2, &big_result);
      set_big_sign(&big_result);
    }
    if (sign_1 == 0 && sign_2 == 0) {
      if (d3_big_is_less(big_value_1, big_value_2)) {
        d3_sub_big(big_value_2, big_value_1, &big_result);
        set_big_sign(&big_result);
      } else
        d3_sub_big(big_value_1, big_value_2, &big_result);
    }
  }
  set_big_scale(&big_result, get_big_scale(big_value_1));
  borrow = big_decimal_to_decimal(big_result, result);
  if (borrow) decimal_zeroing(result);
  return borrow;
}

int d3_mul(d3_decimal value_1, d3_decimal value_2, d3_decimal* result) {
  d3_big_decimal big_value_1 = {0}, big_value_2 = {0}, big_result = {0},
                 big_value_2_cpy = {0};
  if (result) decimal_zeroing(result);
  decimal_to_big_decimal(value_1, &big_value_1);
  decimal_to_big_decimal(value_2, &big_value_2);
  decimal_to_big_decimal(value_2, &big_value_2_cpy);
  int error = 0, sign = 0;
  if (get_big_sign(big_value_1) || get_big_sign(big_value_2)) {
    sign = 1;
  }
  if (get_big_sign(big_value_1) && get_big_sign(big_value_2)) sign = 0;
  d3_mul_big(big_value_1, big_value_2, &big_result);
  int scale_1 = get_big_scale(big_value_1);
  int scale_2 = get_big_scale(big_value_2);
  int scale = scale_1 + scale_2;
  if (sign) {
    set_big_sign(&big_result);
  }
  set_big_scale(&big_result, scale);
  if (result)
    error = big_decimal_to_decimal(big_result, result);
  else
    error = 1;
  return error;
}
int d3_div(d3_decimal value_1, d3_decimal value_2, d3_decimal* result) {
  if (!result) return 1;
  d3_big_decimal big_value_1, big_value_2, big_result_gg = {0}, big_two_cpy;
  decimal_zeroing(result);
  int sign_1 = get_sign(value_1), sign_2 = get_sign(value_2), error = 0,
      zero_2 = d3_all_zero(value_2), code = 0;
  decimal_to_big_decimal(value_1, &big_value_1);
  decimal_to_big_decimal(value_2, &big_value_2);
  int scale = get_big_scale(big_value_1) - get_big_scale(big_value_2),
      zero_1 = d3_all_zero(value_1);
  set_big_bit(&big_value_1, 223, 0);
  set_big_bit(&big_value_2, 223, 0);
  set_big_scale(&big_value_1, 0);
  set_big_scale(&big_value_2, 0);
  if (zero_1) code = 1;
  for (int i = 0; i < 192 && !code; ++i) {
    big_two_cpy = big_value_2;
    for (int j = 0; j < i; j++) {
      big_decimal_shift_left(&big_two_cpy);
    }
    if (d3_big_is_less_or_equal(big_two_cpy, big_value_1) == 0) {
      if (i) big_decimal_shift_right(&big_two_cpy);
      d3_big_decimal big_one = {{1, 0, 0, 0, 0, 0, 0}};
      for (int g = 0; g < i - 1; g++) {
        big_decimal_shift_left(&big_one);
      }
      d3_add_big(big_result_gg, big_one, &big_result_gg);
      d3_sub_big(big_value_1, big_two_cpy, &big_value_1);
      if (d3_big_is_less_or_equal(big_value_2, big_value_1)) {
        i = 0;
      } else {
        if (big_value_1.bits[0] || big_value_1.bits[1] || big_value_1.bits[2]) {
          code += d3_mul_big_10(&big_result_gg);
          d3_mul_big_10(&big_value_1);
          scale++;
          i = 0;
        } else {
          i = 192;
        }
      }
    }
  }
  if (scale < 0) {
    code = 0;
    while (scale < 0 && !code) {
      code = d3_mul_big_10(&big_result_gg);
      scale++;
    }
  }
  if (zero_1 == 0 && scale >= 0 && !zero_2) {
    error = big_decimal_to_decimal(big_result_gg, result);
    set_scale(result, scale);
    if ((sign_1 && sign_2 == 0) || (sign_1 == 0 && sign_2)) set_sign(result);
  }
  if (zero_2) {
    error = 3;
  }
  return error;
}