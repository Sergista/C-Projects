#include "s21_decimal.h"

int s21_is_less(s21_decimal value_1, s21_decimal value_2) {
  int flag = 0;
  s21_exponent_alignment(&value_1, &value_2);
  if (value_1.sign == value_2.sign) {
    int all_fine = 0;
    for (int i = 95; i >= 0; i--) {
      if (s21_get_bit(value_1, i) == s21_get_bit(value_2, i)) {
        continue;
      } else if (s21_get_bit(value_1, i) > s21_get_bit(value_2, i)) {
        all_fine = (value_1.sign == 0) ? 0 : 1;
        break;
      } else if (s21_get_bit(value_1, i) < s21_get_bit(value_2, i)) {
        all_fine = (value_1.sign == 0) ? 1 : 0;
        break;
      }
    }
    flag = (all_fine == 1) ? 1 : 0;
  } else if (value_1.sign > value_2.sign) {
    flag = 1;
  }
  return flag;
}

void s21_exponent_alignment(s21_decimal *first, s21_decimal *second) {
  s21_decimal *bigger = NULL, *smaller = NULL;
  s21_decimal remainder = {{0, 0, 0, 0}};
  if (first->exponent > second->exponent) {
    bigger = first;
    smaller = second;
  } else {
    bigger = second;
    smaller = first;
  }
  if (first->exponent != second->exponent) {
    int sign1 = bigger->sign, sign2 = smaller->sign;
    s21_decimal ten = {{10, 0, 0, 0}};
    bigger->sign = 0;
    smaller->sign = 0;
    while (smaller->exponent != bigger->exponent &&
           s21_last_bit(*smaller) < 93 && smaller->exponent <= 28) {
      int flagInf = 0;
      int exponentSmall = smaller->exponent;
      smaller->exponent = 0;
      ;
      flagInf = s21_mul(ten, *smaller, smaller);
      if (flagInf != 0) break;
      smaller->exponent = exponentSmall + 1;
    }
    while (smaller->exponent != bigger->exponent) {
      int flagInf = 0;
      int exponentBig = bigger->exponent;
      if (bigger->exponent - smaller->exponent == 1) {
        if (bigger->bits[0] >= 5 && bigger->bits[0] < 10) {
          bigger->bits[0] = 1;
          bigger->exponent = exponentBig - 1;
          break;
        }
      }
      flagInf = integer_division(*bigger, ten, bigger, &remainder, 1);
      if (flagInf != 0) break;
      bigger->exponent = exponentBig - 1;
    }
    bigger->sign = sign1;
    smaller->sign = sign2;
  }
}

int s21_is_less_or_equal(s21_decimal dec1, s21_decimal dec2) {
  return (s21_is_less(dec1, dec2) || s21_is_equal(dec1, dec2)) ? 1 : 0;
}

int s21_is_greater(s21_decimal dec1, s21_decimal dec2) {
  return !(s21_is_less_or_equal(dec1, dec2));
}

int s21_is_greater_or_equal(s21_decimal dec1, s21_decimal dec2) {
  return !(s21_is_less(dec1, dec2));
}

int s21_is_equal(s21_decimal dec1, s21_decimal dec2) {
  int error = 0;
  if (dec1.sign == dec2.sign && dec1.exponent == dec2.exponent &&
      dec1.mantiss[0] == dec2.mantiss[0] &&
      dec1.mantiss[1] == dec2.mantiss[1] && dec1.mantiss[2] == dec2.mantiss[2])
    error = 1;
  return error;
}

int s21_is_not_equal(s21_decimal dec1, s21_decimal dec2) {
  return !(s21_is_equal(dec1, dec2));
}
