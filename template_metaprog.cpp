#include<iostream>
#include<vector>

template <typename T>
struct type_identity {
    using type = T;
};

template <typename T>
struct remove_const: type_identity<T> {};

template <typename T>
struct remove_const<const T>: type_identity<T> {};

template <bool B, typename T, typename F>
struct conditional: type_identity<F> {};

template <typename T, typename F>
struct conditional<true, T, F>: type_identity<T> {};


template <typename T, T v>
struct integral_constant {
    static const T value = v;
};

template <bool B>
using bool_constant = integral_constant<bool, B>;

template <typename T, typename U>
struct is_same: bool_constant<false> {};

template <typename T>
struct is_same<T, T>: bool_constant<true> {};

using true_type = bool_constant<true>;
using false_type = bool_constant<false>;

template <typename T, typename U>
const bool is_same_v = is_same<T, U>::value;


template <typename Head, typename... Tail>
struct conjuction: conjuction<Head, conjuction<Tail...>> {};

template <typename T, typename U>
struct conjuction<T, U>: std::conditional_t<T::value, U, false_type> {};

template <typename... Ts>
struct short_conjuction: bool_constant< (Ts::value && ...) > {};

// SFINAE - Substitution Faliure Is Not An Error
template <typename T>
auto f(const T& v) -> typename T::value_type { // compilier know that it isn't need version before instance
    return v[0];
}

int f(...) {
    return 5;
}

template <typename T, typename U = typename T::value_type>
int g(const T& v) { // compilier know that it isn't need version before instance
    return v[0];
}

int g(...) {
    return 5;
}

template <bool B, typename T = void>
struct enable_if {};

template <typename T>
struct enable_if<true, T>: type_identity<T> {};

template <bool B, typename T = void>
using enable_if_t = enable_if<B, T>::type;

template <typename T, typename U = enable_if_t<std::is_class_v<T>>>
int ff(const T& v) { // compilier know that it isn't need version before instance
    return v[0];
}

int ff(...) {
    return 5;
}

template <typename T, typename U = int(T::*)> // only for classes or structs
int is_class_helper(const T& x);

template <typename T>
char is_class_helper(...);

template <typename T>
struct is_class: is_same<decltype(is_class_helper<T>(0)), int> {};

// other in screenshots

int main() {
    std::vector v = {1, 2, 3, 4, 5};
    std::cout << f(v) << '\n';
    int x = 0;
    std::cout << f(x) << '\n';

    std::cout << g(v) << '\n';
    std::cout << g(x) << '\n';

    return 0;
}