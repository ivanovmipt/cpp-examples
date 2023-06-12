#include <iostream>

void f(int x) {
  std::cout << 1 << '\n';
}

int main() {
  // Pointers
  int a = 5;
  std::cout << &a << '\n'; // int*
  int* a_ptr = &a;
  std::cout << *a_ptr << '\n'; // int

  // int* a_ptr_ptr = &a_ptr; // CE
  int** a_ptr_ptr = &a_ptr;

  a_ptr + 1; // +4

  void* b; // do not know what is it (but pointer)

  // Memory
  // data (static: text, literals, globals), text (code), stack (automatic: local, call back) - compile time
  // dynamic memory - runtime

  // Arrays
  int m[10], m2[10];
  *m; // array-to-pointer conversion
  m[3] == *(m + 3);
  // ++m; m = m2; // CE

  int** p = new int*[100];
  for (int i = 0; i < 100; ++i) {
    p[i] = new int [100];
  }
  for (int i = 0; i < 100; ++i) {
    delete[] p[i];
  }
  delete[] p;

  // Functions
  std::cout << &f << ' ' << (void*)&f << '\n';
  void (*f_ptr)(int) = &f; // or f - function-to-pointer conversion
  f_ptr(1);
  // must be defined else ld error
  // double -> float - ambiguous call

  // References
  int& r = a; // r - one more name for a
  // int& r2; // CE
  // int&* rm = nullptr; // pointer to ref - CE
  // Dangling reference - function return ref to local var

  // Constants
  const int* k = nullptr;
  ++k;
  // ++*k; // CE
  int* const k2 = nullptr; // another
  const int* const k3 = nullptr;
  // int& const r2 = a; // CE
  const int& r3 = 123; // ok with rvalue (lifetime extension)
  // int& r4 = 123; // CE

  // Conversions
  int x = 0;
  (double) x; // C-style cast
  static_cast<double>(x); // usual
  reinterpret_cast<int&>(x); // as bytes
  int& x2 = reinterpret_cast<int&>(x); // x2 -> (as type) -> x
  const int x3 = x;
  const_cast<int&>(x3);




  return 0;
}

inline void f(); // not jump - paste in code (not need usually)
