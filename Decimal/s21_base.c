#include "s21_decimal.h"

int s21_get_bit(s21_decimal value, int i) {
  int res = 0;
  if (i / 32 < 3) {
    unsigned int mask = 1u << (i % 32);
    res = value.bits[i / 32] & mask;
  }
  return !!res;
}

void s21_set_bit(s21_decimal *src, int i, int value) {
  unsigned int mask = 1u << (i % 32);
  (value) ? (src->bits[i / 32] |= mask) : (src->bits[i / 32] &= ~mask);
}

int s21_get_float_exp(float src) {
  typedef union {
    float src1;
    int src_int;
  } float_to_int;
  float_to_int fti = {0};
  fti.src1 = src;
  return ((fti.src_int & ~0x80000000) >> 23) - 127;
}

void s21_bits_copy(s21_decimal src, s21_decimal *dest) {
  dest->bits[0] = src.bits[0];
  dest->bits[1] = src.bits[1];
  dest->bits[2] = src.bits[2];
  dest->bits[3] = src.bits[3];
}

void s21_reset(s21_decimal *value) {
  value->bits[0] = value->bits[1] = value->bits[2] = value->bits[3] = 0;
}

int s21_last_bit(s21_decimal number) {
  int s21_last_bit = 95;
  for (; s21_last_bit >= 0 && s21_get_bit(number, s21_last_bit) == 0;
       s21_last_bit--) {
    ;
  }
  return s21_last_bit;
}
