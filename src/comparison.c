#include "d3_decimal.h"
#include "help_func.h"

int d3_is_less(d3_decimal value_1, d3_decimal value_2) {
  d3_big_decimal big_value_1 = {0}, big_value_2 = {0};
  decimal_to_big_decimal(value_1, &big_value_1);
  decimal_to_big_decimal(value_2, &big_value_2);
  int mean = d3_big_is_less(big_value_1, big_value_2);
  return mean;
}
int d3_is_less_or_equal(d3_decimal value_1, d3_decimal value_2) {
  int mean;
  if (d3_is_less(value_1, value_2) || d3_is_equal(value_1, value_2))
    mean = 1;
  else
    mean = 0;
  return mean;
}
int d3_is_greater(d3_decimal value_1, d3_decimal value_2) {
  int mean;
  if (d3_is_less(value_2, value_1))
    mean = 1;
  else
    mean = 0;
  return mean;
}
int d3_is_greater_or_equal(d3_decimal value_1, d3_decimal value_2) {
  int mean;
  if (d3_is_less(value_2, value_1) || d3_is_equal(value_1, value_2))
    mean = 1;
  else
    mean = 0;
  return mean;
}
int d3_is_equal(d3_decimal value_1, d3_decimal value_2) {
  d3_big_decimal big_value_1 = {0}, big_value_2 = {0};
  decimal_to_big_decimal(value_1, &big_value_1);
  decimal_to_big_decimal(value_2, &big_value_2);
  int mean = d3_big_is_equal(big_value_1, big_value_2);
  return mean;
}

int d3_is_not_equal(d3_decimal value_1, d3_decimal value_2) {
  return (!d3_is_equal(value_1, value_2));
}
