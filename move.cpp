#include<iostream>
#include<vector>

template <typename T>
void swap(T& x, T& y) { // O(1)
    T t = std::move(x);
    x = std::move(y);
    y = std::move(t);
}

template <typename T>
struct S {
    char* arr;
    size_t sz;
    size_t cap;

    S(S&& s): arr(s.arr), sz(s.sz), cap(s.cap) { // move-constructor
        s.arr  = nullptr;
        s.sz = s.cap = 0;
    }

    // S(S&& s): arr(std::move(s.arr)), sz(std::move(s.sz)), cap(std::move(s.cap)) {} // default move-constructor
    // doesn't generate as usual -> copy-constructor
    // The Rule of Three -> The Rule of Five

     S& operator=(S&& s) { // move-assignment
        S copy = std::move(s); // call move-constructor
        // swap(copy);
        return *this;
    }
};

template <typename T>
T&& naive_move(T& x) noexcept {
    return static_cast<T&&>(x); // not runtime
}

// Reference collapsing
// & + & = &
// && + & = &
// & + && = &
// && + && = &&

template <typename T>
std::remove_reference_t<T>&& move(T&& x) noexcept { // need to broke reference, what...
    return static_cast<std::remove_reference_t<T>&&>(x);
}

// naive-forward is not allowed (need template argument)

template <typename T>
T&& forward(std::remove_reference_t<T>& x) noexcept {
    return static_cast<T&&>(x);
}

template <typename T>
T&& forward(std::remove_reference_t<T>&& x) noexcept {
    static_assert(!std::is_lvalue_reference_v<T>);
    return static_cast<T&&>(x);
}

template <typename T>
std::conditional_t<std::is_nothrow_move_constructible_v<T>, T&&, const T&>
move_if_noexcept(T& x) noexcept {
    return move(x);
}

int main() {
    std::vector<std::string> v;
    std::string s = "fghjbknlsbfgdthdngrbfdvbfh";
    std::move(s);
    std::cout << s.size() << '\n'; // 26 (0 actions)
    v.push_back(std::move(s));
    std::cout << s.size() << '\n'; // 0

    // && - rvalue reference

    // lvalue and rvalue are categories of expressions, not types!
    // bitreference - rvalue, const var - lvalue

    /* lvalues
    * name of var, identifier: x
    * binary for standart types: = += -= &= /= <<= >>=  etc 
    * unary for standart types: * []
    * prefix ++ or -- for standart types
    * a ? b : c if both are lvalues
    * a, b if b lvalue
    * function/operator if return a lvalue-ref
    * cast to lvalue
    */

    /* rvalues
    * literals: 5, 'a', true
    * binary for standart types: + - & / << >> == && etc 
    * unary for standart types: & + - 
    * postfix ++ or -- for standart types
    * a ? b : c else
    * a, b if b rvalue
    * function/operator if return a nonref or rvalue-ref
    * cast to non-ref or rvalue-ref
    */

    // bi1 + bi2 = bi3 // correct but must be CE

    // Rvalue-refs
    int x = 0;
    // int& r = 1; // CE: lvalue = rvalue
    int& r = x;
    // int&& r = x; // CE: rvalue = lvalue
    int&& r2 = std::move(x);
    int&& r3 = 1;
    ++r2; ++r3;
    std::cout << r2 << ' ' << r3 << '\n'; // 1 2
    // rvalue-ref extend life
    int& r4 = r3; // lvalue = lvalue

    // Perfect forwarding problem - how to transfer values to emplace_back

    std::string str = std::string("abc"); // copy elision (like RVO)
    // S(S(S(S()))) // constructor called 0 times
    // S x = S(S(S(S()))) // constructor called 1 times (prvalues)

    // xvalues (Cppreference)
    // xvalue - rvalue in memory
    // prvalue - rvalue may not be in memory

    // S(S(S(S()))).x // temporary materialization - cast to xvalue

    // new (arr + i) T(std::move_if_noexcept(arr[i]));
    // cause why we need to mark move-constructor as noexcept (else copy)

    return 0;
}

struct F {
    F& operator=(F&) & { // ref-qualifier: only for lvalue
    }

    F& operator=(F&) && { // ref-qualifier: only for rvalue
    }

    template <typename... Args>
    void emplace_back(const Args&... args) {
        /* in allocator:
        alloc.construct(ptr, std::forward<Args>(args)...);
        take arguments with Args&&... - it is Forwarding ref (only this syntax with templates)
        */
    }

    template<class T>
    int f(T&& x) { // x is a forwarding reference
        return emplace_back(std::forward<T>(x)); // and so can be forwarded
    }
};

struct BI {
    BI& operator+=(const BI& a) {
        return *this;
    }
};

BI operator+(const BI& a, const BI& b) {
    BI copy = a;
    copy += b;
    return copy; // NRVO - Named Return Value Optimization
     // return BI(a += b); // RVO - Return Value Optimization
}
