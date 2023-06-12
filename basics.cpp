#include <iostream>

void fun();

int main() {
  uint64_t a; // == unsigned long long, 8 bytes
  int32_t b; // == int, 4 bytes

  // Integer promotion
  int m = 100; // size 4
  long long n = 1000; // size 8
  std::cout << sizeof(n + m) << '\n'; // size 8
  unsigned long long k = 1000;
  std::cout << sizeof(n + k) << '\n'; // size 8

  // Overflow
  size_t s = 0;
  std::cout << s - 1 << '\n';

  // Floating point
  float f; // 4 bytes
  double d; // 8 bytes
  long double ld; // 16 bytes

  // Literals (not variables!)
  int hex_n = 0x123;
  int oct_n = 0123;
  int bin_n = 0b101;
  std::cout << bin_n << '\n'; // 5
  unsigned int uint = 3233333333u;
  int not_uint = 3233333333u;
  std::cout << uint << ' ' << not_uint << '\n'; // overflow
  "abc"; // const char*

  fun();

  sizeof(int);
  sizeof(n + m);
  sizeof(n++);

  return 0;
}

// Declaration - объявление
int fun1(double);
extern int x1;

// Definition - определение
int fun2(double) {
  return 1;
}
int x2;

// Every definition is a declaration
// One definition rule


// Scopes
void fun() {
  int x = 1;
  int a = 1;
  std::cout << x << ' '; // 1
  {
    int x = 2; // shadow but ok
    a = 3;
    std::cout << x << ' '; // 2, ::x - global
  }
  std::cout << x << '\n'; // 1
}

namespace N1 {
  namespace N2{
  // only in global
  // allowed addition
  }
}

// Operator Precedence https://en.cppreference.com/w/cpp/language/operator_precedence

// Errors
// CE - compile-time error (lexical: 4783efg8, syntax: a=x+, semantic: cout << a = b; ambiguous call)
// RE - runtime error (segfault core dumped: a[100'000], floating point exception: 1/0, aborted)
// UB - undefined behaviour (index out of range, overflow, optimizations)
// unspecified behaviour (f() + g() * h())
