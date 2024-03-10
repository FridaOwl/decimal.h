#ifndef D3_HELP_DECIMAL_H
#define D3_HELP_DECIMAL_H
#include <math.h>
#include <stdio.h>

#include "d3_decimal.h"

typedef struct {
  unsigned int bits[7];
} d3_big_decimal;

int d3_round_bank(d3_big_decimal value, d3_big_decimal *result);
int d3_mul_big(d3_big_decimal value_1, d3_big_decimal value_2,
               d3_big_decimal *result);
int bits_all_one(d3_big_decimal *bits, int index, int count_bit);
int bits_diff(d3_big_decimal *bits, int index, int count_bit);
int bits_all_zero(d3_big_decimal *bits, int index, int count_bit);
void d3_normalization(d3_big_decimal *value_1, d3_big_decimal *value_2);
int d3_big_is_equal(d3_big_decimal big_value_1, d3_big_decimal big_value_2);
int d3_mul_big_10(d3_big_decimal *result);
int d3_div_10(d3_decimal *result);
int d3_div_big_10(d3_big_decimal *result);
int d3_all_zero(d3_decimal value);
int big_decimal_shift_left(d3_big_decimal *value);
void big_decimal_shift_right(d3_big_decimal *value);
void save_bit(d3_decimal *value, int index);
int d3_add_big(d3_big_decimal value_1, d3_big_decimal value_2,
               d3_big_decimal *result);
void d3_sub_big(d3_big_decimal value_1, d3_big_decimal value_2,
                d3_big_decimal *result);
int d3_big_is_less(d3_big_decimal value_1, d3_big_decimal value_2);
void decimal_to_big_decimal(d3_decimal small, d3_big_decimal *big);
int big_decimal_to_decimal(d3_big_decimal big, d3_decimal *small);
int d3_big_is_less_or_equal(d3_big_decimal value_1, d3_big_decimal value_2);
void decimal_zeroing(d3_decimal *value);
void big_decimal_zeroing(d3_big_decimal *value);
int d3_all_big_zero(d3_big_decimal value);
int get_scale(d3_decimal value);
void set_scale(d3_decimal *value, int scale);
int get_big_scale(d3_big_decimal value);
void set_big_scale(d3_big_decimal *value, int scale);
int get_bit(d3_decimal value, int index);
void set_bit(d3_decimal *value, int index, int bit);
int get_big_bit(d3_big_decimal value, int index);
void set_big_bit(d3_big_decimal *value, int index, int bit);
int get_sign(d3_decimal value);
void set_sign(d3_decimal *value);
int get_big_sign(d3_big_decimal value);
void set_big_sign(d3_big_decimal *value);

#endif
