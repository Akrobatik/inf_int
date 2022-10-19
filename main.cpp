#include "inf_int.h"

int main() {
  inf_int a;
  std::cout << "a : " << a << std::endl;

  inf_int b(100);
  std::cout << "b : " << b << std::endl;

  inf_int c("-123456");
  std::cout << "c : " << c << std::endl;

  inf_int d(c);
  std::cout << "d : " << d << std::endl;

  inf_int e = c;
  std::cout << "e : " << e << std::endl;

  inf_int f = inf_int(10000) + inf_int(20000);
  std::cout << "f : " << f << std::endl;

  inf_int g = inf_int(10000) - inf_int(20000);
  std::cout << "g : " << g << std::endl;

  inf_int h = inf_int("10000000000") * inf_int("123456789123456789");
  std::cout << "h : " << h << std::endl;

  std::cout << "b == e : " << std::boolalpha << (b == e) << std::endl;
  std::cout << "c == e : " << std::boolalpha << (c == e) << std::endl;

  std::cout << "b != e : " << std::boolalpha << (b != e) << std::endl;
  std::cout << "c != e : " << std::boolalpha << (c != e) << std::endl;

  std::cout << "b > e : " << std::boolalpha << (b > e) << std::endl;
  std::cout << "c > e : " << std::boolalpha << (c > e) << std::endl;

  std::cout << "e < b : " << std::boolalpha << (e < b) << std::endl;
  std::cout << "e < c : " << std::boolalpha << (e < c) << std::endl;

  inf_int* org = new inf_int(100);
  inf_int copy1 = *org;
  inf_int copy2(*org);

  delete org;

  [&]() -> void {
    __try {
      std::cout << "copy1 : " << copy1 << std::endl;
      std::cout << "operator= -> Deep Copy" << std::endl;

      std::cout << "copy2 : " << copy2 << std::endl;
      std::cout << "copy constructor -> Deep Copy" << std::endl;
    } __except (1) {
      std::cout << "Exception by Shallow Copy" << std::endl;
    }
  }();

  return 0;
}