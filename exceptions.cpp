#include<iostream>

double f(int x) {
    if (x == 0) {
        throw std::string("Ouch!"); // can throw any type
    }
    return 1.0 / x;
}

struct S {
    S() { std::cout << "Create" << "\n"; }
    ~S() { std::cout << "Destroyed" << "\n"; }
    S(const S&) { std::cout << "Copied" << "\n"; }
};

void g() {
    // std::string s = "123";
    // throw s; // moves from static memory to dynamic

    S s;
    throw s;
}


struct F {
    int x;
    F() try: x(5) {
    } catch (...) {
    }

    ~F() noexcept(false) {} // can throw exceptions
};

void ff() noexcept try { // function-try block, noexcept(bool = true) - doesn't throw exceptions
} catch (...) {
}

int main() {
    int x;
    std::cin >> x;

    try {
        std::cout << f(x) << '\n';
    } catch (std::string str) { // (...)
        std::cout << str << '\n';
        // throw; // throw down original and delete local
        // throw str; // throw down local and delete original 
    } catch (double d) {
        std::cout << d << '\n';
    } // if not processing std::terminate (like pure virtual function call)
    // 'terminate' is a wrap of 'abort' 
    std::cout << "This is fine." << '\n';

    /* throw exception:
    * vector.at()
    * std::bad_alloc()
    * dynamic_cast
    * typeid(nullptr)
    * !!floating point exception - not exception
    */

    /*
    * segfault (!ex, re)
    * exit from deep recursion (!re, ex)
    */

    // Stack unwinding - delete locals etc

    try {
        g();
    } catch (S s) {
        std::cout << "Catched!" << "\n";
    }
    
    // RAII idiom - Resource Acquisition Is Initialization (smart pointers)

    /* exceptions in
    * constructor - delete fields but not object (not delegate constructors)
    * destructor - forbidden
    */
    // std::uncaught_exceptions() - int > 0 if throw exception

    /* exception safety
    * basic - stop
    * strong - return in start state
    */

    return 0;
}
