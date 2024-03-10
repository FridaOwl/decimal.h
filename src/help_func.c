#include "help_func.h"

#include "d3_decimal.h"

int d3_truncate_big(d3_big_decimal value, d3_big_decimal* result) {
  int scale = get_big_scale(value);
  for (; scale > 0; scale--) {
    d3_div_big_10(&value);
  }
  set_big_scale(&value, 0);
  *result = value;
  return 0;
}
int d3_round_bank(d3_big_decimal value, d3_big_decimal* result) {
  big_decimal_zeroing(result);
  int scale = get_big_scale(value);
  d3_big_decimal value_cpy = value;
  d3_truncate_big(value, &value_cpy);
  int bit = get_big_bit(value_cpy, 0);
  for (; scale > 0 &&
         (value.bits[3] || value.bits[4] || value.bits[5] || scale > 28);
       scale--) {
    d3_div_big_10(&value);
  }
  d3_big_decimal one = {{1, 0, 0, 0, 0, 0, 0}};
  if (!bit) {
    d3_add_big(value, one, &value);
  }
  *result = value;
  set_big_scale(result, scale);
  return 0;
}
int d3_add_big(d3_big_decimal value_1, d3_big_decimal value_2,
               d3_big_decimal* result) {
  int error = 0, index = 0, count_bit = 0;
  for (; index <= 191; ++index) {
    int count_1 = get_big_bit(value_1, index);
    int count_2 = get_big_bit(value_2, index);
    if (count_1 && count_2) {
      count_bit = bits_all_one(result, index, count_bit);
    }
    if ((count_1 == 0 && count_2) || (count_1 && count_2 == 0)) {
      count_bit = bits_diff(result, index, count_bit);
    }
    if (count_2 == 0 && count_1 == 0) {
      count_bit = bits_all_zero(result, index, count_bit);
    }
  }
  if (count_bit && index == 192) {
    error = 1;
    count_bit = 0;
  }
  if (count_bit) {
    bits_all_one(result, index, count_bit);
  }
  set_big_scale(result, get_big_scale(value_1));
  return error;
}
void d3_sub_big(d3_big_decimal value_1, d3_big_decimal value_2,
                d3_big_decimal* result) {
  int index = 0, scale = get_big_scale(value_1);
  for (; index <= 191; ++index) {
    int count_1 = get_big_bit(value_1, index);
    int count_2 = get_big_bit(value_2, index);
    if (count_1 && count_2) {
      set_big_bit(result, index, 0);
    }
    if (count_1 == 0 && count_2) {
      int i = index, count = 0;
      for (; i <= 191 && !count; i++) {
        if (get_big_bit(value_1, i)) {
          count = 1;
          set_big_bit(result, index, 1);
          set_big_bit(&value_1, i, 0);
          for (int j = i - 1; j > index; j--) {
            set_big_bit(&value_1, j, 1);
          }
        }
      }
      set_big_bit(result, index, 1);
    }
    if (count_1 && count_2 == 0) {
      set_big_bit(result, index, 1);
    }
    if (count_1 == 0 && count_2 == 0) {
      set_big_bit(result, index, 0);
    }
  }
  set_big_scale(result, scale);
}
int d3_mul_big(d3_big_decimal big_value_1, d3_big_decimal big_value_2,
               d3_big_decimal* big_result) {
  d3_big_decimal big_value_2_cpy;
  int error = 0;
  for (int i = 0; i < 192; ++i) {
    if (get_big_bit(big_value_1, i)) {
      big_value_2_cpy = big_value_2;
      for (int j = 0; j < i && !error; j++) {
        error = big_decimal_shift_left(&big_value_2_cpy);
      }
      d3_add_big(*big_result, big_value_2_cpy, big_result);
    }
  }
  return error;
}
int bits_all_one(d3_big_decimal* bits, int index, int count_bit) {
  if (count_bit == 0) {
    set_big_bit(bits, index, 0);
    count_bit = 1;
  } else {
    set_big_bit(bits, index, 1);
  }
  return count_bit;
}
int bits_diff(d3_big_decimal* bits, int index, int count_bit) {
  if (count_bit == 0) {
    set_big_bit(bits, index, 1);
  } else {
    set_big_bit(bits, index, 0);
  }
  return count_bit;
}
int bits_all_zero(d3_big_decimal* bits, int index, int count_bit) {
  if (count_bit == 0) {
    set_big_bit(bits, index, 0);
  } else {
    set_big_bit(bits, index, 1);
  }
  return 0;
}
void d3_normalization(d3_big_decimal* value_1, d3_big_decimal* value_2) {
  int scale_1 = get_big_scale(*value_1);
  int scale_2 = get_big_scale(*value_2);
  int sign_1 = get_big_sign(*value_1);
  int sign_2 = get_big_sign(*value_2);
  if (scale_1 > 28) {
    int diff_1 = scale_1 - 28;
    for (int i = 0; i < diff_1; i++) {
      d3_div_big_10(value_1);
    }
    scale_1 = 28;
    set_big_scale(value_1, 28);
  }
  if (scale_2 > 28) {
    int diff_2 = scale_2 - 28;
    for (int i = 0; i < diff_2; i++) {
      d3_div_big_10(value_2);
    }
    scale_2 = 28;
    set_big_scale(value_2, 28);
  }

  if (scale_1 > scale_2) {
    int diff = scale_1 - scale_2;
    set_big_scale(value_2, scale_1);
    for (int i = 0; i < diff; i++) {
      d3_mul_big_10(value_2);
    }
  } else {
    int diff = scale_2 - scale_1;
    set_big_scale(value_1, scale_2);
    for (int i = 0; i < diff; i++) {
      d3_mul_big_10(value_1);
    }
  }
  if (sign_1) set_big_sign(value_1);
  if (sign_2) set_big_sign(value_2);
}
int d3_mul_big_10(d3_big_decimal* result) {
  d3_big_decimal res_cpy;
  res_cpy = *result;
  int error = 0;
  result->bits[0] = 0b0;
  result->bits[1] = 0b0;
  result->bits[2] = 0b0;
  result->bits[3] = 0b0;
  result->bits[4] = 0b0;
  result->bits[5] = 0b0;
  for (int i = 0; i < 192; ++i) {
    if (get_big_bit(res_cpy, i)) {
      d3_big_decimal mask = {{10, 0, 0, 0, 0, 0, res_cpy.bits[6]}};
      for (int j = 0; j < i; j++) {
        big_decimal_shift_left(&mask);
      }
      error = d3_add_big(*result, mask, result);
      if (error) i = 192;
    }
  }
  return error;
}
int d3_div_10(d3_decimal* result) {
  int remainder = 0;
  for (int i = 95; i >= 0; --i) {
    int bit = get_bit(*result, i);
    int res = (bit + remainder * 2) / 10;
    remainder = (bit + remainder * 2) % 10;
    set_bit(result, i, res);
  }
  return remainder;
}
int d3_div_big_10(d3_big_decimal* result) {
  int remainder = 0;
  for (int i = 191; i >= 0; --i) {
    int bit = get_big_bit(*result, i);
    int res = (bit + remainder * 2) / 10;
    remainder = (bit + remainder * 2) % 10;
    set_big_bit(result, i, res);
  }
  return remainder;
}
int d3_all_zero(d3_decimal value) {
  int zero = 0;
  for (int i = 0; i < 3; ++i) {
    if (value.bits[i] == 0) zero++;
  }
  return zero == 3 ? 1 : 0;
}
int d3_all_big_zero(d3_big_decimal value) {
  int zero = 0;
  for (int i = 0; i < 6; ++i) {
    if (value.bits[i] == 0) zero++;
  }
  return zero == 6 ? 1 : 0;
}
void decimal_zeroing(d3_decimal* value) {
  value->bits[0] = 0;
  value->bits[1] = 0;
  value->bits[2] = 0;
  value->bits[3] = 0;
}
void big_decimal_zeroing(d3_big_decimal* value) {
  value->bits[0] = 0;
  value->bits[1] = 0;
  value->bits[2] = 0;
  value->bits[3] = 0;
  value->bits[4] = 0;
  value->bits[5] = 0;
  value->bits[6] = 0;
}
int big_decimal_shift_left(d3_big_decimal* value) {
  int bit_31 = get_big_bit(*value, 31);
  int bit_63 = get_big_bit(*value, 63);
  int bit_95 = get_big_bit(*value, 95);
  int bit_127 = get_big_bit(*value, 127);
  int bit_159 = get_big_bit(*value, 159);
  int bit_191 = get_big_bit(*value, 191);
  value->bits[0] = value->bits[0] << 1;
  value->bits[1] = value->bits[1] << 1;
  value->bits[2] = value->bits[2] << 1;
  value->bits[3] = value->bits[3] << 1;
  value->bits[4] = value->bits[4] << 1;
  value->bits[5] = value->bits[5] << 1;
  if (bit_31) set_big_bit(value, 32, 1);
  if (bit_63) set_big_bit(value, 64, 1);
  if (bit_95) set_big_bit(value, 96, 1);
  if (bit_127) set_big_bit(value, 128, 1);
  if (bit_159) set_big_bit(value, 160, 1);
  return bit_191;
}
void big_decimal_shift_right(d3_big_decimal* value) {
  int bit_32 = get_big_bit(*value, 32);
  int bit_64 = get_big_bit(*value, 64);
  int bit_96 = get_big_bit(*value, 96);
  int bit_128 = get_big_bit(*value, 128);
  int bit_159 = get_big_bit(*value, 159);
  value->bits[0] = value->bits[0] >> 1;
  value->bits[1] = value->bits[1] >> 1;
  value->bits[2] = value->bits[2] >> 1;
  value->bits[3] = value->bits[3] >> 1;
  value->bits[4] = value->bits[4] >> 1;
  value->bits[5] = value->bits[5] >> 1;
  if (bit_32) set_big_bit(value, 31, 1);
  if (bit_64) set_big_bit(value, 63, 1);
  if (bit_96) set_big_bit(value, 95, 1);
  if (bit_128) set_big_bit(value, 128, 1);
  if (bit_159) set_big_bit(value, 159, 1);
}
void decimal_to_big_decimal(d3_decimal small, d3_big_decimal* big) {
  big->bits[0] = small.bits[0];
  big->bits[1] = small.bits[1];
  big->bits[2] = small.bits[2];
  big->bits[3] = 0;
  big->bits[4] = 0;
  big->bits[5] = 0;
  big->bits[6] = small.bits[3];
}
int big_decimal_to_decimal(d3_big_decimal big, d3_decimal* small) {
  int error = 0;
  int sign = get_big_sign(big);
  int scale = get_big_scale(big);
  if (big.bits[3] || big.bits[4] || big.bits[5] || scale > 28) {
    d3_round_bank(big, &big);
  }
  small->bits[0] = big.bits[0];
  small->bits[1] = big.bits[1];
  small->bits[2] = big.bits[2];
  small->bits[3] = big.bits[6];
  if (big.bits[3] || big.bits[4] || big.bits[5]) {
    error = 1;
    decimal_zeroing(small);
  }
  if (sign && error) {
    error += 1;
  }
  return error;
}
int d3_big_is_less_or_equal(d3_big_decimal value_1, d3_big_decimal value_2) {
  int mean;
  if (d3_big_is_less(value_1, value_2) || d3_big_is_equal(value_1, value_2))
    mean = 1;
  else
    mean = 0;
  return mean;
}
int d3_big_is_less(d3_big_decimal value_1, d3_big_decimal value_2) {
  int mean = 0;
  int sign_1 = get_big_sign(value_1);
  int sign_2 = get_big_sign(value_2);
  if (!(d3_all_big_zero(value_1) && d3_all_big_zero(value_2))) {
    d3_normalization(&value_1, &value_2);
    if (sign_1 && sign_2 == 0) mean = 1;
    if (sign_1 && sign_2) {
      set_big_bit(&value_1, 223, 0);
      set_big_bit(&value_2, 223, 0);
      mean = d3_big_is_less(value_2, value_1);
    }
    if (sign_1 == 0 && sign_2 == 0) {
      for (int i = 191; 0 <= i; i--) {
        int bit_1 = get_big_bit(value_1, i);
        int bit_2 = get_big_bit(value_2, i);
        if (bit_1 == 0 && bit_2) {
          mean = 1;
          i = 0;
        }
        if (bit_1 && bit_2 == 0) {
          mean = 0;
          i = 0;
        }
      }
    }
  }
  return mean;
}
int d3_big_is_equal(d3_big_decimal big_value_1, d3_big_decimal big_value_2) {
  int mean = 0;
  if (d3_all_big_zero(big_value_1) && d3_all_big_zero(big_value_2)) {
    mean = 1;
  }
  d3_normalization(&big_value_1, &big_value_2);
  if (big_value_1.bits[0] == big_value_2.bits[0] &&
      big_value_1.bits[1] == big_value_2.bits[1] &&
      big_value_1.bits[2] == big_value_2.bits[2] &&
      big_value_1.bits[3] == big_value_2.bits[3] &&
      big_value_1.bits[4] == big_value_2.bits[4] &&
      big_value_1.bits[5] == big_value_2.bits[5] &&
      get_big_sign(big_value_1) == get_big_sign(big_value_2)) {
    mean = 1;
  }
  return mean;
}
int get_scale(d3_decimal value) {
  int mask = 127 << 16;
  int scale = (value.bits[3] & mask) >> 16;
  return scale;
}
void set_scale(d3_decimal* value, int scale) {
  int sign = get_sign(*value);
  value->bits[3] = 0;
  int mask = scale << 16;
  value->bits[3] = value->bits[3] | mask;
  if (sign) set_sign(value);
}
int get_big_scale(d3_big_decimal value) {
  int mask = 255 << 16;
  int scale = (value.bits[6] & mask) >> 16;
  return scale;
}
void set_big_scale(d3_big_decimal* value, int scale) {
  int sign = get_big_sign(*value);
  value->bits[6] = 0;
  int mask = scale << 16;
  value->bits[6] = value->bits[6] | mask;
  if (sign) set_big_sign(value);
}
int get_bit(d3_decimal value, int index) {
  int bit_compare = 0b1 << index % 32;
  bit_compare = value.bits[index / 32] & bit_compare;
  return bit_compare ? 1 : 0;
}
int get_big_bit(d3_big_decimal value, int index) {
  int bit_compare = 0b1 << index % 32;
  bit_compare = value.bits[index / 32] & bit_compare;
  return bit_compare ? 1 : 0;
}
void set_bit(d3_decimal* value, int index, int bit) {
  int mask = 0b1 << index % 32;
  if (bit)
    value->bits[index / 32] |= mask;
  else
    value->bits[index / 32] &= ~mask;
}
void set_big_bit(d3_big_decimal* value, int index, int bit) {
  unsigned int mask = 0b1 << index % 32;
  if (bit)
    value->bits[index / 32] |= mask;
  else
    value->bits[index / 32] &= ~mask;
}
int get_sign(d3_decimal value) {
  int sign = value.bits[3] & 2147483648;
  return sign ? 1 : 0;
}
void set_sign(d3_decimal* value) {
  value->bits[3] = value->bits[3] | 2147483648;
}
int get_big_sign(d3_big_decimal value) {
  int sign = value.bits[6] & 2147483648;
  return sign ? 1 : 0;
}
void set_big_sign(d3_big_decimal* value) {
  value->bits[6] = value->bits[6] | 2147483648;
}
