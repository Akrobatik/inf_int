#include "inf_int.h"

inf_int::inf_int() {
  thesign = true;
  length = 1;

  digits = new char[2];
  digits[0] = '0';
  digits[1] = '\0';
}

inf_int::inf_int(int value) {
  char digits_temp[10 + 1]; 

  thesign = (value >= 0);

  value = abs(value);

  unsigned int i = 0;
  do {
    digits_temp[i++] = value % 10 + 48;
    value /= 10;
  } while (value != 0);

  length = i;

  digits = new char[i + 1];
  memcpy(digits, digits_temp, i);
  digits[i] = '\0';
}

inf_int::inf_int(const char* value_string) {
  if (value_string[0] == '-') {
    thesign = false;
    value_string += 1;  
  } else {
    thesign = true;
  }

  length = strlen(value_string);

  digits = new char[length + 1];
  for (long long i = 0; i < length; i++) {
    digits[i] = value_string[length - i - 1];
  }
  digits[length] = '\0';
}

inf_int::inf_int(const inf_int& copy) {
  thesign = copy.thesign;
  length = copy.length;

  digits = new char[length + 1];
  memcpy(digits, copy.digits, length);
  digits[length] = '\0';
}

inf_int::~inf_int() { delete[] digits; }

inf_int& inf_int::operator=(const inf_int& copy) {
  if (digits) {
    delete[] digits;
  }

  thesign = copy.thesign;
  length = copy.length;

  digits = new char[length + 1];
  memcpy(digits, copy.digits, length);
  digits[length] = '\0';

  return *this;
}

bool operator==(const inf_int& a, const inf_int& b) {
  return (a.thesign == b.thesign) && (strcmp(a.digits, b.digits) == 0);
}

bool operator!=(const inf_int& a, const inf_int& b) {
  return !operator==(a, b);
}

bool operator>(const inf_int& a, const inf_int& b) {
  if (a.thesign != b.thesign) {
    return a.thesign;
  }

  bool sign = a.thesign; 

  if (a.length != b.length) {
    return (a.length > b.length) == sign;
  }

  unsigned int length = a.length;

  for (long long i = length - 1; i >= 0; i--) {
    if (a.digits[i] == b.digits[i]) {
      continue;
    }

    return (a.digits[i] > b.digits[i]) == sign;
  }

  return false;
}

bool operator<(const inf_int& a, const inf_int& b) {
  return !operator>(a, b) && !operator==(a, b);
}

inf_int operator+(const inf_int& a, const inf_int& b) {
  if (a.thesign == b.thesign) {
    bool a_is_longer = a.length > b.length;
    char* longer_digits = (a_is_longer ? a.digits : b.digits);
    int longer_length = (a_is_longer ? a.length : b.length);
    int shorter_length = (a_is_longer ? b.length : a.length);

    inf_int add;
    int add_max_length = longer_length + 1; 
    char* add_digits = new char[add_max_length + 1];

    bool carry = false;

    for (size_t i = 0; i < shorter_length; i++) {
      int value = (int)(a.digits[i] - 48) + (int)(b.digits[i] - 48) + (carry ? 1 : 0);  // '0' -> 48
      carry = value >= 10;

      add_digits[i] = value % 10 + 48;
    }

    for (size_t i = shorter_length; i < longer_length; i++) {
      int value = (int)(longer_digits[i] - 48) + (carry ? 1 : 0);
      carry = value >= 10;

      add_digits[i] = value % 10 + 48;
    }

    if (carry) {
      add_digits[longer_length] = '1';
      add_digits[longer_length + 1] = '\0';

      add.length = longer_length + 1;
    } else {
      add_digits[longer_length] = '\0';

      add.length = longer_length;
    }

    add.thesign = a.thesign;

    delete[] add.digits;
    add.digits = add_digits;

    return add;
  } else {
    if (strcmp(a.digits, b.digits) == 0) {
      return inf_int();
    }

    inf_int minus(b);
    minus.thesign = !minus.thesign;

    bool a_is_bigger = (a > minus) == a.thesign;
    char* bigger_digits = (a_is_bigger ? a.digits : b.digits);
    char* smaller_digits = (a_is_bigger ? b.digits : a.digits);
    int bigger_length = (a_is_bigger ? a.length : b.length);
    int smaller_length = (a_is_bigger ? b.length : a.length);

    inf_int sub;
    int sub_max_length = bigger_length; 
    char* sub_digits = new char[sub_max_length + 1];

    bool carry = false;

    for (size_t i = 0; i < smaller_length; i++) {
      int value = (int)(bigger_digits[i] - 48) - (int)(smaller_digits[i] - 48) - (carry ? 1 : 0);  // '0' -> 48
      carry = value < 0;

      sub_digits[i] = value % 10 + (carry ? 10 : 0) + 48;

      if (value % 10) {
        sub.length = i + 1;
      }
    }

    for (size_t i = smaller_length; i < bigger_length; i++) {
      int value = (int)(bigger_digits[i] - 48) - (carry ? 1 : 0);  // '0' -> 48
      carry = value < 0;

      sub_digits[i] = value % 10 + (carry ? 10 : 0) + 48;

      if (value % 10) {
        sub.length = i + 1;
      }
    }

    sub.thesign = a.thesign == a_is_bigger;

    delete[] sub.digits;
    sub.digits = sub_digits;

    return sub;
  }
}

inf_int operator-(const inf_int& a, const inf_int& b) {
  inf_int minus(b);
  minus.thesign = !minus.thesign;
  return a + minus;
}

inf_int operator*(const inf_int& a, const inf_int& b) {
  if (strcmp(a.digits, "0") == 0 || strcmp(b.digits, "0") == 0) {
    return inf_int();
  }

  inf_int mul;
  mul.thesign = a.thesign == b.thesign;

  for (size_t i = 0; i < b.length; i++) {
    if (b.digits[i] == '0') {
      continue;
    }

    inf_int partial(a);
    partial.length = a.length + i;
    char* partial_digits = new char[partial.length + 1];
    memset(partial_digits, '0', i);
    memcpy(partial_digits + i, a.digits, a.length);
    partial_digits[a.length + i] = '\0';

    delete[] partial.digits;
    partial.digits = partial_digits;

    for (int j = 0; j < (int)(b.digits[i] - 48); j++) {
      mul = mul + partial;
    }
  }

  return mul;
}

std::ostream& operator<<(std::ostream& out, const inf_int& input) {
  if (!input.thesign) {
    out << '-';
  }

  for (long long i = input.length - 1; i >= 0; i--) {
    out << input.digits[i];
  }

  return out;
}