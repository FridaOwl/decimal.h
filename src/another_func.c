#include "d3_decimal.h"
#include "help_func.h"
int d3_floor(d3_decimal value, d3_decimal *result) {
  int sign = get_sign(value), scale = get_scale(value), error = 0;
  if (result) {
    d3_truncate(value, result);
    if (sign && scale) {
      set_bit(result, 127, 0);
      d3_decimal one = {{1, 0, 0, 0}};
      error = d3_add(*result, one, result);
      set_sign(result);
    }
  } else {
    error = 1;
  }
  return error;
}
int d3_round(d3_decimal value, d3_decimal *result) {
  int sign = get_sign(value), scale = get_scale(value), error = 0;
  set_bit(&value, 127, 0);
  if (result) {
    int rem = 0;
    d3_truncate(value, result);
    for (int i = 0; i < scale; i++) {
      rem = d3_div_10(&value);
    }
    if (rem >= 5) {
      d3_decimal one = {{1, 0, 0, 0}};
      error = d3_add(*result, one, result);
    }
    if (sign) set_sign(result);
  } else {
    error = 1;
  }
  return error;
}
int d3_truncate(d3_decimal value, d3_decimal *result) {
  int scale = get_scale(value);
  for (; scale > 0; scale--) {
    d3_div_10(&value);
  }
  set_scale(&value, 0);
  *result = value;
  return 0;
}
int d3_negate(d3_decimal value, d3_decimal *result) {
  unsigned int mask = 2147483648;
  *result = value;
  result->bits[3] = value.bits[3] ^ mask;
  return 0;
}