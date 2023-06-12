#include<iostream>
#include<vector>
#include<map>

int main() {
    std::vector v = {1, 2, 3, 4, 5, 6, 7};
    // std::sort(v.begin(), v.end(), [](int x, int y){ return abs(x-5) < abs(y-5); });
    auto f = [](int x, int y){ // capture globals and parameters as default
        return abs(x-5) < abs(y-5);
    };
    std::sort(v.begin(), v.end(), f);
    for (auto i: v) std::cout << i << ' ';
    std::cout << '\n';

    std::map<int, int, decltype(f)> m;

    auto g = [](int x, int y) -> bool {
        return abs(x-5) < abs(y-5);
    };

    std::cout << [](int x, int y){
        return x + y;
    }(1, 2) << '\n';

    int mid = 4;
    auto ff = [mid](int x, int y){ // make const copy of 'mid' so we can take &mid (nonconst)
    // auto ff = [mid](int x, int y) mutable { // not const
    // auto ff = [=](int x, int y) { // all that uses in ff (copy)
    // auto ff = [&](int x, int y) { // all that uses in ff (ref)
    // auto ff = [=, &mid](int x, int y) { // all that uses in ff (ref and copy)
    // auto ff = [m = mid + 1](int x, int y) {
    // auto ff = [a = std::move(a)](int x, int y) {
    // auto ff = [&mid = std::as_const(mid)](int x, int y) {
        return abs(x-mid) < abs(y-mid);
    };
    std::sort(v.begin(), v.end(), ff);
    for (auto i: v) std::cout << i << ' ';
    std::cout << '\n';

    // decltype(ff) gg; // CE: have not default constructor if catch mid
    // catch with ref - have not copy constructor
    // each lambda - each types: name of type - 'closure'

    auto fff = []<typename T>(const T& x, const T& y) {
        return x < y;
    };

    return 0;
}

struct A {
    int a = 0;
    void f() {
        auto g = [this](int x, int y) mutable { // save pointers
            ++a;
            return x + y;
        };

        auto g2 = [a = a](int x, int y) mutable { // copy field
            ++a;
            return x + y;
        };
    }
};
