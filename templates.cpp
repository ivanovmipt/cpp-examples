#include<iostream>
#include<map>
#include<vector>
#include <type_traits>

template <typename T, typename U> // T, U - meta-variables (must all use)
T max(const T& x, const U& y) {
    return x > y ? x : y;
}

template <typename T>
class vector {
  private:
    T* arr;
    size_t sz;
    size_t cap;
    
    static const T base = 1;

  public:
    template <typename U> // generate when call method
    void push_back(const U& x);
};

template <typename T> // order is important
template <typename U>
void vector<T>::push_back(const U& x) {}

// STL = Standart Template Library

template <typename T>
using mymap = std::map<T, T>;

template <typename T>
const T base = vector<T>::base;

// Overloading
template <typename T>
void f(T x) {} // else
// void f(T& x) {} // CE - ambiguous call
void f(int x) {} // prefer if int

// Specialization
template <> // Full specialization
class vector<bool> {
  private:
    char* arr;
    size_t sz;
    size_t cap;
};

template <typename T>
struct S {
    void f() {}
};

template <typename T>
struct S<T*> {
    void f() {}
};

// template<> before function - specialization, not overloading
// order is important

// Non-type parameters
template <typename T, size_t N>
struct F {};

template <typename T, template<typename> typename U>
struct L {};

// Compile-time computations
template <size_t N> // for
struct Fibonacci {
    static const size_t value = Fibonacci<N-1>::value + Fibonacci<N-2>::value;
};

template <> // if
struct Fibonacci<1> {
    static const size_t value = 1;
};

template <>
struct Fibonacci<0> {
    static const size_t value = 0;
};

// Dependent names problem
template <typename T>
struct N {
    using A = int;
};

template <>
struct N<int> {
    static const size_t A = 25;
};

template <typename T>
void f() {
    // 'A' is dependent name
    // S<T>::A * x; // what 'A' is? - CE
    typename N<T>::A * x;
    // typename S<T>::template A * x; // if A - template
}

// Type traits
template <typename T>
void fun(T x) {
    if constexpr (std::is_same<T, int>::value) {
        std::cout << x << '\n';
    }
}

template <typename T, typename U> // meta-function
struct is_same {
    static const bool value = false;
};

template <typename T>
struct is_same<T, T> {
    static const bool value = true;
};

template <typename T, typename U>
const bool is_same_v = is_same<T, U>::value;

template <typename T>
struct remove_reference {
    using type = T;
};

template <typename T>
struct remove_reference<T&> {
    using type = T;
};

template <typename T>
void func() {
    typename remove_reference<T>::type x = 0;
}

template<bool B, typename T, typename F>
struct conditional {
    using type = F;
};

template<typename T, typename F>
struct conditional<true, T, F> {
    using type = T;
};

template<bool B, typename T, typename F>
using conditional_t = conditional<B, T, F>::type;

// Variadic templates
template <typename... Types>
void f(const Types&... args) {
    sizeof...(args); // number of arguments
}

void print() {
    std::cout << '\n';
}

template <typename Head, typename... Tail>
void print(const Head& head, const Tail&... tail) {
    // static_assert(std::is_same_v<Head, int>);
    std::cout << head << ' ';
    print(tail...);
}

//Fold expression
template <typename... Args>
bool all_empty(const Args&... args) {
    return (args.empty() && ...); // unpack all elements
}

template <typename... Args>
bool print(const Args&... args) {
    static_assert((!std::is_pointer_v<Args> && ...));
    (std::cout << args << ...);
}


int main() {
    max(1, 2); // instantiation
    // max(1.0, 2) // CE - different types (case T)
    max<int, double>(1, 2);
    max<int>(1, 2);

    vector<int> v;
    // vector<int&> v2; // CE - pointer to reference

    mymap<int> m;

    L<int, std::vector> l;

    std::cout << Fibonacci<20>::value << '\n';
    // static_assert(Fibonacci<20>::value != 6765);

    print(1, 4.0, "abcde");

    return 0;
}
