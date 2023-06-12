#include<iostream>

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



int main() {

    return 0;
}