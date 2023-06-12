#include <iostream>

class S { // size: 24 4(4)81(7)
 public: // encapsulation (only compile, check last order!!, visible not ability)
  int x = 2;
  double d = 3;
  char c;

  friend int main();
  friend class SS; // not symmetric and not transitive

  struct SS {

  };
};

class NewS {
  // const int n;
  // int& m; // not error but must be defined in constructor
  // const std::string& s = "abc"; // will life until you in constructor

  mutable size_t var = 0; // can edit in const methods

 public:
  // NewS(NewS n) {} // recursive - CE
  NewS(const NewS& n) {}

  NewS() = default;

  NewS(int n): NewS() {}

  void g() {
    this->~NewS(); // UB - for 1 object 1 destructor
  }

  NewS& operator=(const NewS& other) {
    // copy and swap
    return *this;
  }

  NewS& operator-(); // unary minus

  int l = 0;
  int& l_ref = l;
  void k() const {
    // ++l; // CE in each reason
    ++l_ref; // but...
    std::cout << l_ref << '\n';
  }

  static int sss; // ld error
  // static int sss = 0; // CE error - must be out of class
  static const int ksss = 0;

  void f() = delete; // not want overload
};

NewS operator+(NewS, const NewS&); //symmetric

std::ostream& operator<<(std::ostream& out, const NewS&);

int NewS::sss = 321;

struct F { // functional class
  F() = default;

  void operator()(int k) const {
    std::cout << k << '\n';
  }

  operator int() const { // cast to int (implicit)
    return 123;
  }

  explicit operator bool() const { // static_cast
    return true;
  }

  explicit F(int) {} // explicit constructor

  int x;
  void fun() {}

  static int F::* get_y() {
    return &F::y;
  }

 private:
  int y = 222;
};

F operator""_suffix(unsigned long long x);
F operator""_suffix(const char* x, size_t size);

struct Greater { // Less - another
  bool operator()(int x, int y) const {
    return x > y;
  }
};

enum class TaxiClass: uint8_t { // type not required
  ECONOM,
  BUSINESS
};

int main () {
  // aggregate initialization
  S s1{};
  S s2;
  S s3{1, 2};
  S s4{1};

  S::SS ss;

  // S s5 = 1; // call constructor

  // as least 1 constructor -> must be default constructor

  // THE RULE OF THREE
  // destructor, copy constructor, operator=

  // Operators overloading
  /*
   * forbidden:
   * int::operator+
   * priority
   * . :: ?:
   *
   * const only for class members
   */

  NewS n;
  n.k();
  NewS::ksss;


  F f;
  f(123);

  // F ff = 5; // CE: explicit constructor

  if (f) {} // not implicit conversion

  int F::* ptr_x = &F::x; // offset, not private
  std::cout << f.*ptr_x << '\n';

  std::cout << f.*F::get_y() << '\n';

  void (F::* ptr_fun)() = &F::fun; // pointer to member
  (f.*ptr_fun)();

  TaxiClass tc = TaxiClass::ECONOM;
  std::cout << static_cast<int>(tc) << '\n';


  return 0;
}
