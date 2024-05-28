#include "s21_decimal.h"
int s21_shift_left(s21_decimal *first, int shift) {
  int res_val = 1;
  for (int i = 0; i < shift; i++) {
    if (s21_get_bit(*first, 95)) break;
    int value_31bit = s21_get_bit(*first, 31);
    int value_63bit = s21_get_bit(*first, 63);
    first->bits[0] <<= 1;
    first->bits[1] <<= 1;
    first->bits[2] <<= 1;
    (value_31bit) ? s21_set_bit(first, 32, 1) : s21_set_bit(first, 32, 0);
    (value_63bit) ? s21_set_bit(first, 64, 1) : s21_set_bit(first, 64, 0);
    res_val = 0;
  }
  return res_val;
}

int s21_shift_right(s21_decimal *first, int shift) {
  int res_val = 1;
  for (int i = 0; i < shift; i++) {
    int value_32bit = s21_get_bit(*first, 32);
    int value_64bit = s21_get_bit(*first, 64);
    first->bits[0] >>= 1;
    first->bits[1] >>= 1;
    first->bits[2] >>= 1;
    value_32bit ? s21_set_bit(first, 31, 1) : s21_set_bit(first, 31, 0);
    value_64bit ? s21_set_bit(first, 63, 1) : s21_set_bit(first, 63, 0);
    s21_set_bit(first, 95, 0);
    res_val = 0;
  }
  return res_val;
}