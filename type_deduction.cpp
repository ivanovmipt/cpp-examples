#include<iostream>
#include<vector>

template <typename T>
void f(T&) {} // auto = this type
// conflicting return types because compile-time

auto&& g() {
    return 1; // UB
}

auto sum(int a, int b) -> decltype(a + b) {
    return a + b;
}

decltype(auto) fun(int a, int b) { // & -> &, notref -> copy
    return a + b;
}

int main() {
    auto x = 1;
    f(x);

    std::vector<bool> v(5);
    auto br = v[2]; // not good

    int a = 0;
    decltype(a)& b = a; // not delete references
    decltype(throw 1)* p = nullptr; // void*
    // decltype(++a) // not runtime

    int j;
    int& i = j;
    static_assert(std::is_same_v<decltype((i)), int&>); // ok

    return 0;
}