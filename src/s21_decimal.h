#ifndef _S21_DECIMAL_H
#define _S21_DECIMAL_H

#include <limits.h>
#include <math.h>
#include <stdio.h>

typedef union {
  unsigned int bits[4];
  struct {
    unsigned int mantiss[3];
    unsigned int not_used0 : 16;
    unsigned int exponent : 8;
    unsigned int not_used1 : 7;
    unsigned int sign : 1;
  };
} s21_decimal;

/*--------------------------------------------
              Bits функции
 --------------------------------------------*/
int s21_get_bit(s21_decimal value, int i);
void s21_set_bit(s21_decimal *src, int i, int value);
int s21_get_float_exp(float src);
void s21_bits_copy(s21_decimal src, s21_decimal *dest);
int s21_last_bit(s21_decimal number);
int s21_shift_left(s21_decimal *first, int shift);
int s21_shift_right(s21_decimal *first, int shift);
void s21_reset(s21_decimal *value);

/*--------------------------------------------
              Вспомогательные функции
 --------------------------------------------*/
int s21_print_decimal_bin(s21_decimal dec1);
int s21_simple_add(s21_decimal value_1, s21_decimal value_2,
                   s21_decimal *result);

int s21_first_1_bit_position(s21_decimal value);
int s21_integer_division(s21_decimal value_1, s21_decimal value_2,
                         s21_decimal *remainder_of_division,
                         s21_decimal *result);
int s21_decimal_is_zero(s21_decimal value_1);

/*--------------------------------------------
              Арифметические функции
    return 0 - OK
    return 1 - the number is too big
    return 2 - the number is too small
    return 3 - division by 0
 --------------------------------------------*/
int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_mod(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);

/*--------------------------------------------
              Функции сравнения
    return 0 - FALSE
    return 1 - TRUE
 --------------------------------------------*/
int s21_is_less(s21_decimal, s21_decimal);
int s21_is_less_or_equal(s21_decimal, s21_decimal);
int s21_is_greater(s21_decimal, s21_decimal);
int s21_is_greater_or_equal(s21_decimal, s21_decimal);
int s21_is_equal(s21_decimal, s21_decimal);
int s21_is_not_equal(s21_decimal, s21_decimal);

/*--------------------------------------------
              Функции преобразования
    return 0 - ОК
    return 1 - ERROR
 --------------------------------------------*/
int s21_from_int_to_decimal(int src, s21_decimal *dst);
int s21_from_decimal_to_int(s21_decimal src, int *dst);
int s21_from_decimal_to_float(s21_decimal src, float *dst);
int s21_from_float_to_decimal(float src, s21_decimal *dst);
void s21_exponent_alignment(s21_decimal *first, s21_decimal *second);

/*--------------------------------------------
              Другие функции
    return 0 - ОК
    return 1 - ERROR
 --------------------------------------------*/
int s21_floor(s21_decimal value, s21_decimal *result);
int s21_round(s21_decimal value, s21_decimal *result);
int s21_truncate(s21_decimal value, s21_decimal *result);
int s21_negate(s21_decimal value, s21_decimal *result);

/*--------------------------------------------
              Доп функции деления
 --------------------------------------------*/
int integer_division(s21_decimal value_1, s21_decimal value_2,
                     s21_decimal *result, s21_decimal *remainder, int flagInt);
void simple_div(s21_decimal tmp_buf, s21_decimal *tmp_div, s21_decimal *tmp_mod,
                int *discharge);
void first_step(s21_decimal *tmp_div, s21_decimal value_2, int *exponent_c,
                s21_decimal *tmp_res, int *index_res);
void prepare_division(s21_decimal tmp_div, s21_decimal *tmp_mod,
                      s21_decimal *tmp_del, s21_decimal value_2,
                      int *discharge);

#endif  //_S21_DECIMAL_H
