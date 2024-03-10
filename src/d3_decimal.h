#ifndef D3_DECIMAL_H
#define D3_DECIMAL_H

typedef struct {
  unsigned int bits[4];
} d3_decimal;

int d3_add(d3_decimal value_1, d3_decimal value_2, d3_decimal *result);
int d3_sub(d3_decimal value_1, d3_decimal value_2, d3_decimal *result);
int d3_mul(d3_decimal value_1, d3_decimal value_2, d3_decimal *result);
int d3_div(d3_decimal value_1, d3_decimal value_2, d3_decimal *result);

int d3_is_less(d3_decimal value_1, d3_decimal value_2);
int d3_is_less_or_equal(d3_decimal value_1, d3_decimal value_2);
int d3_is_greater(d3_decimal value_1, d3_decimal value_2);
int d3_is_greater_or_equal(d3_decimal value_1, d3_decimal value_2);
int d3_is_equal(d3_decimal value_1, d3_decimal value_2);
int d3_is_not_equal(d3_decimal value_1, d3_decimal value_2);

int d3_from_int_to_decimal(int src, d3_decimal *dst);
int d3_from_float_to_decimal(float src, d3_decimal *dst);
int d3_from_decimal_to_int(d3_decimal src, int *dst);
int d3_from_decimal_to_float(d3_decimal src, float *dst);

int d3_floor(d3_decimal value, d3_decimal *result);
int d3_round(d3_decimal value, d3_decimal *result);
int d3_truncate(d3_decimal value, d3_decimal *result);

int d3_negate(d3_decimal value, d3_decimal *result);
#endif
