#include "s21_decimal.h"

int s21_from_int_to_decimal(int src, s21_decimal *dst) {
  int ret_val = 0;
  if (dst) {
    s21_reset(dst);
    if (src >= 0) dst->bits[0] = src;
    if (src < 0) {
      src = -src;
      dst->bits[0] = src;
      dst->sign = 1;
    }
  } else {
    ret_val = 1;
  }
  return ret_val;
}

int s21_from_decimal_to_int(s21_decimal src, int *dst) {
  int res = 0;
  *dst = 0;
  s21_truncate(src, &src);
  if (src.bits[1] == 0 && src.bits[2] == 0) {
    *dst = src.bits[0];
    if (src.sign == 1) *dst = -*dst;
  } else {
    res = 1;
  }
  if ((s21_get_bit(src, 31) == 1 && src.sign == 1)) {
    *dst = 0;
    res = 1;
  }
  return res;
}

int s21_from_float_to_decimal(float src, s21_decimal *dst) {
  typedef union {
    float *src1;
    int *src_int;
  } float_to_int;
  float_to_int fti = {0};
  fti.src1 = &src;
  int ret_val = 1;
  if (dst) {
    s21_reset(dst);
    if (src < 0) dst->sign = 1;
    int exp = s21_get_float_exp(src);
    double temp = (double)fabs(src);
    int scale = 0;
    for (; scale < 28 && (int)temp / (int)pow(2, 22) == 0;
         temp *= 10, scale++) {
      ;
    }
    temp = round(temp);
    if (scale <= 28 && (exp > -94 && exp < 96)) {
      for (; fmod(temp, 10) == 0 && scale > 0; scale--, temp /= 10) {
        ;
      }
      src = (float)temp;
      exp = s21_get_float_exp(src);
      s21_set_bit(dst, exp, 1);
      for (int i = exp - 1, j = 22; j >= 0; i--, j--)
        if ((*fti.src_int & (1 << j)) != 0) s21_set_bit(dst, i, 1);
      // if ((*(int *)&src & (1 << j)) != 0) s21_set_bit(dst, i, 1);
      dst->exponent = scale;
      ret_val = 0;
    }
  }
  return ret_val;
}

int s21_from_decimal_to_float(s21_decimal src, float *dst) {
  int ret_val = 1;
  if (dst != NULL) {
    double temp = 0;
    for (int i = 0; i < 96; i++)
      if (s21_get_bit(src, i)) temp += pow(2, i);
    int exponent = src.exponent;
    for (int i = exponent; i > 0; i--) temp /= 10.0;
    if (src.sign) temp *= -1;
    *dst = (float)temp;
    ret_val = 0;
  }
  return ret_val;
}
