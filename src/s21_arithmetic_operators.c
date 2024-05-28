#include "s21_decimal.h"

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int res = 0;
  int res_flag = 0;

  s21_reset(result);
  s21_exponent_alignment(&value_1, &value_2);
  int sign1 = value_1.sign;
  int sign2 = value_2.sign;
  if (sign1 != sign2) {
    if (sign1 == 1) {
      res_flag = 1;
    }
    if (sign2 == 1) {
      res_flag = 2;
    }
  }
  if (res_flag == 0) {
    int transfer = 0;
    for (int j = 0; j < 96; j++) {
      int tmp1 = s21_get_bit(value_1, j);
      int tmp2 = s21_get_bit(value_2, j);
      int tmp_res = tmp1 + tmp2 + transfer;
      if (tmp_res == 0) {
        transfer = 0;
        s21_set_bit(result, j, 0);
      } else if (tmp_res == 1) {
        transfer = 0;
        s21_set_bit(result, j, 1);
      } else if (tmp_res == 2) {
        if (j == 95) {
          res = 1;
          break;
        }
        transfer = 1;
        s21_set_bit(result, j, 0);
      } else if (tmp_res == 3) {
        if (j == 95) {
          res = 1;
          break;
        }
        transfer = 1;
        s21_set_bit(result, j, 1);
      }
    }
    result->exponent = value_1.exponent;
    if (sign1 == 1) {
      result->sign = 1;
    }
  } else if (res_flag == 1) {
    value_1.sign = 0;
    res = s21_sub(value_2, value_1, result);
  } else if (res_flag == 2) {
    value_2.sign = 0;
    res = s21_sub(value_1, value_2, result);
  }
  if (result->exponent > 28 ||
      (result->exponent == 28 && result->bits[0] == 0 && result->bits[1] == 0 &&
       result->bits[2] == 0)) {
    res = 2;
  }
  if (res != 0) {
    s21_reset(result);
  }
  return res;
}

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int res = 0;
  int res_flag = 0;

  s21_reset(result);
  s21_exponent_alignment(&value_1, &value_2);

  int sign1 = value_1.sign;
  int sign2 = value_2.sign;
  if (sign1 != sign2) {
    if (sign1 == 0) {
      res_flag = 1;
    }
    if (sign2 == 0) {
      res_flag = 2;
    }
  }

  if (res_flag == 0) {
    if (sign1 == 0) {
      if (s21_is_less(value_1, value_2)) {
        result->sign = 1;
        s21_decimal t = value_1;
        value_1 = value_2;
        value_2 = t;
      }
      for (int j = 0; j < 96; j++) {
        int tmp1 = s21_get_bit(value_1, j);
        int tmp2 = s21_get_bit(value_2, j);
        int tmp_res = tmp1 - tmp2;
        if (tmp_res == 0) {
          s21_set_bit(result, j, 0);
          continue;
        } else if (tmp_res == 1) {
          s21_set_bit(result, j, 1);
        } else if (tmp_res == -1) {
          int n = j + 1;
          while (s21_get_bit(value_1, n) != 1) {
            s21_set_bit(&value_1, n, 1);
            n++;
          }
          s21_set_bit(&value_1, n, 0);
          s21_set_bit(result, j, 1);
        }
      }
      result->exponent = value_1.exponent;
    } else if (sign1 == 1) {
      s21_set_bit(&value_1, 127, 0);
      s21_set_bit(&value_2, 127, 0);
      res = s21_sub(value_2, value_1, result);
    }
  } else if (res_flag == 1) {
    s21_set_bit(&value_2, 127, 0);
    res = s21_add(value_1, value_2, result);
  } else if (res_flag == 2) {
    s21_set_bit(&value_2, 127, 1);
    res = s21_add(value_1, value_2, result);
  }
  if (result->exponent > 28 ||
      (result->exponent == 28 && result->bits[0] == 0 && result->bits[1] == 0 &&
       result->bits[2] == 0))
    res = 2;
  if (res != 0) s21_reset(result);
  return res;
}

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int res = 0;
  s21_reset(result);
  s21_decimal tmp = {{0, 0, 0, 0}};
  s21_decimal ten = {{10, 0, 0, 0}};
  s21_decimal remainder = {{0, 0, 0, 0}};
  int sign1 = value_1.sign, sign2 = value_2.sign;
  value_1.sign = 0;
  value_2.sign = 0;

  int exponent1 = value_1.exponent, exponent2 = value_2.exponent;
  value_1.exponent = 0;
  value_2.exponent = 0;

  while (value_1.exponent + value_2.exponent > 28) {
    if (value_1.exponent >= value_2.exponent && s21_is_greater(value_1, ten)) {
      int exponent_value1 = value_1.exponent;
      integer_division(value_1, ten, &value_1, &remainder, 1);
      value_1.exponent = exponent_value1 - 1;
      exponent1 = exponent_value1 - 1;
    } else {
      if (s21_is_less(value_2, ten)) break;
      int exponent_value2 = value_2.exponent;
      integer_division(value_2, ten, &value_2, &remainder, 1);
      value_2.exponent = exponent_value2 - 1;
      exponent2 = exponent_value2 - 1;
    }
  }

  s21_bits_copy(value_1, &tmp);
  int n = s21_last_bit(value_2) + 1;
  if (n < 96) {
    for (int i = 0; i < n; i++) {
      if (s21_get_bit(value_2, i) != 0) {
        res = s21_add(*result, tmp, result);
        if (res != 0) break;
        s21_shift_left(&tmp, 1);
      } else {
        s21_shift_left(&tmp, 1);
      }
    }
    result->exponent = exponent1 + exponent2;
  } else {
    res = 1;
  }
  if (sign1 != sign2) {
    result->sign = 1;
  }
  if (res != 0 || exponent1 + exponent2 > 28) s21_reset(result);
  return res;
}

int s21_mod(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  s21_reset(result);
  int ret_val = 0;
  s21_decimal tmp_del = {{0, 0, 0, 0}};
  int s2 = value_2.sign, s1 = value_1.sign;
  value_1.sign = 0;
  value_2.sign = 0;
  int exponent1 = value_1.exponent, exponent2 = value_2.exponent;
  if (value_2.bits[0] == 0 && value_2.bits[1] == 0 && value_2.bits[2] == 0) {
    ret_val = 3;
  } else if (value_1.exponent > 28 || value_2.exponent > 28) {
    ret_val = 2;
  } else if (s21_is_less(value_1, value_2)) {
    if (s1 && !s2) {
      s21_sub(value_2, value_1, &tmp_del);
    } else if (!s1 && s2) {
      s21_sub(value_1, value_2, &tmp_del);
      value_1.sign = s1;
    } else if ((s1 && s2) || (!s1 && !s2)) {
      value_1.sign = s1;
      s21_bits_copy(value_1, &tmp_del);
    }
  } else {
    s21_exponent_alignment(&value_1, &value_2);
    integer_division(value_1, value_2, result, &tmp_del, 1);
    if (s2 && !s1) {
      s21_sub(value_2, tmp_del, &tmp_del);
      tmp_del.sign = 1;
    } else if (s1 && !s2) {
      tmp_del.sign = 1;
      s21_add(value_2, tmp_del, &tmp_del);
      tmp_del.sign = 0;
    } else if (s1 && s2) {
      tmp_del.sign = 1;
    }
    if (exponent1 > exponent2)
      tmp_del.exponent = exponent1;
    else
      tmp_del.exponent = exponent2;
  }
  *result = tmp_del;
  if (ret_val != 0) s21_reset(result);
  return ret_val;
}

int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  s21_decimal remainder = {{1, 0, 0, 0}};
  s21_decimal tmp_res = {{0, 0, 0, 0}};
  int ret_val = 0, count = 0;
  int exponent1 = value_1.exponent, exponent2 = value_2.exponent, exponent = 0;
  int sign1 = value_1.sign, sign2 = value_2.sign;
  value_1.sign = 0;
  value_2.sign = 0;
  (exponent1 >= exponent2) ? exponent = exponent1 - exponent2
                           : s21_exponent_alignment(&value_1, &value_2);
  if (s21_is_greater(value_2, value_1) && exponent == 0) {
    s21_decimal ten = {{10, 0, 0, 0}};
    while (s21_is_greater(value_2, value_1)) {
      s21_mul(value_1, ten, &value_1);
      exponent++;
      if (exponent == 28) break;
    }
  }
  value_1.exponent = exponent;
  while (count < 10) {
    *result = tmp_res;
    ret_val = integer_division(value_1, value_2, &tmp_res, &remainder, 0);
    if ((tmp_res.exponent) == 0 && s21_last_bit(tmp_res) > 93) {
      ret_val = 1;
      break;
    }
    s21_add(tmp_res, *result, &tmp_res);
    if (remainder.bits[0] == 0 && remainder.bits[1] == 0 &&
        remainder.bits[2] == 0)
      break;
    value_1 = remainder;
    value_1.exponent = exponent + count;
    count++;
  }
  *result = tmp_res;
  if (sign1 != sign2) result->sign = 1;
  if (ret_val != 0) s21_reset(result);
  return ret_val;
}

int integer_division(s21_decimal value_1, s21_decimal value_2,
                     s21_decimal *result, s21_decimal *remainder, int flagInt) {
  s21_decimal tmp_div = {{0, 0, 0, 0}};
  s21_decimal tmp_del = {{0, 0, 0, 0}};
  s21_decimal tmp_mod = {{0, 0, 0, 0}};
  s21_decimal tmp_res = {{0, 0, 0, 0}};
  s21_decimal tmp_buf = {{0, 0, 0, 0}};

  int exponent_c = value_1.exponent;
  int index_res = 95, ret_val = 0, discharge = 1;

  int sign1 = value_1.sign, sign2 = value_2.sign;
  int exponent1 = value_1.exponent, exponent2 = value_2.exponent;
  s21_bits_copy(value_1, &tmp_div);

  tmp_div.exponent = 0;
  value_2.exponent = 0;

  tmp_div.sign = 0;
  value_2.sign = 0;

  if (value_2.bits[0] == 0 && value_2.bits[1] == 0 && value_2.bits[2] == 0) {
    ret_val = 3;
  } else if ((value_1.bits[0] == 0 && value_1.bits[1] == 0 &&
              value_1.bits[2] == 0) ||
             exponent1 > 28 || exponent2 > 28) {
    ret_val = 2;
  } else {
    if (s21_is_greater_or_equal(tmp_div, value_2) && flagInt) {
      prepare_division(tmp_div, &tmp_mod, &tmp_del, value_2, &discharge);
    } else if (!flagInt) {
      first_step(&tmp_div, value_2, &exponent_c, &tmp_res, &index_res);
      prepare_division(tmp_div, &tmp_mod, &tmp_del, value_2, &discharge);
    }
    while (discharge != -1 && index_res >= 0 && exponent_c <= 28) {
      if (s21_is_greater_or_equal(tmp_del, value_2)) {
        ret_val = s21_sub(tmp_del, value_2, &tmp_buf);
        s21_set_bit(&tmp_res, index_res, 1);
        index_res--;
        simple_div(tmp_buf, &tmp_del, &tmp_mod, &discharge);
      } else {
        simple_div(tmp_del, &tmp_del, &tmp_mod, &discharge);
        s21_set_bit(&tmp_res, index_res, 0);
        index_res--;
      }
    }
    s21_reset(result);
    for (int i = 95; i > index_res; i--) {
      s21_get_bit(tmp_res, i) == 1 ? s21_set_bit(result, i - index_res - 1, 1)
                                   : s21_set_bit(result, i - index_res - 1, 0);
    }
    if (sign1 != sign2) result->sign = 1;
    result->exponent = exponent_c;
    s21_shift_right(&tmp_del, 1);
    *remainder = tmp_del;
  }
  if (ret_val != 0) s21_reset(result);
  return ret_val;
}

void simple_div(s21_decimal tmp_buf, s21_decimal *tmp_div, s21_decimal *tmp_mod,
                int *discharge) {
  s21_bits_copy(tmp_buf, tmp_div);
  s21_shift_left(tmp_div, 1);
  s21_get_bit(*tmp_mod, *discharge - 1) == 1 ? s21_set_bit(tmp_div, 0, 1)
                                             : s21_set_bit(tmp_div, 0, 0);
  s21_set_bit(tmp_mod, *discharge - 1, 0);
  (*discharge)--;
}

void first_step(s21_decimal *tmp_div, s21_decimal value_2, int *exponent_c,
                s21_decimal *tmp_res, int *index_res) {
  s21_decimal ten = {{10, 0, 0, 0}};
  int x = s21_is_greater_or_equal(*tmp_div, value_2);
  while (x != 1) {
    s21_set_bit(tmp_res, *index_res, 0);
    (*index_res)--;
    s21_mul(*tmp_div, ten, tmp_div);
    (*exponent_c)++;
    x = s21_is_greater_or_equal(*tmp_div, value_2);
  }
}

void prepare_division(s21_decimal tmp_div, s21_decimal *tmp_mod,
                      s21_decimal *tmp_del, s21_decimal value_2,
                      int *discharge) {
  *discharge = 0;
  int shift = s21_last_bit(tmp_div) - s21_last_bit(value_2);
  int n = 0;
  while (1) {
    s21_bits_copy(tmp_div, tmp_del);
    s21_shift_right(tmp_del, shift - n);
    if (s21_is_greater_or_equal(*tmp_del, value_2) == 1) {
      break;
    } else {
      n++;
    }
  }
  s21_bits_copy(tmp_div, tmp_mod);
  *discharge = s21_last_bit(tmp_div) - s21_last_bit(*tmp_del);
  for (int i = 95; i > s21_last_bit(tmp_div) - s21_last_bit(*tmp_del) - 1;
       i--) {
    s21_set_bit(tmp_mod, i, 0);
  }
}
