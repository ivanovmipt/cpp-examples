#include <iostream>

struct A {
    A() { std::cout << "A constructed" << '\n'; }
    ~A() { std::cout << "A dectroyed" << '\n'; }
};


struct Base {
    Base() { std::cout << "Base constructed" << '\n'; }
    ~Base() { std::cout << "Base dectroyed" << '\n'; }

    int x;
    A a;

    Base(int x): x(x) {}

    void f() {
        std::cout << "base function" << '\n';
    }
};

struct Derived: public Base {
    Derived() { std::cout << "Derived constructed" << '\n'; }
    ~Derived() { std::cout << "Derived dectroyed" << '\n'; }

    A a;

    Derived (int x): Base(x) {} // else default constructor of Base

    void g() {
        std::cout << "derived function" << '\n';
    }
};

struct Derived2: Base {
    Base b; // composition
    /*
    memory: b(padding)other
`   Empty base optimization (EBO): if parent is empty don't allocate additional memory
    not work with composition
    */

    // no constructors in start moment
    using Base::Base; // but not now (excluding copy and move constructors)
};


int main() {
    // encapsulation
    /*
    * private is inherited but not visible
    * protected - members, friends, heirs
    */

    // inheritance
    /*
    * private - only child
    * protected - child and child of child
    */

   // one signature method - hidding
   // d.Base::X
    // hidding - first, overloading - second

    // for public, not private and protected
    Derived d;
    Base& b_ref = d;
    b_ref.f();
    // b_ref.g(); // CE
    Base* b_ptr = &d;
    b_ptr->f();
    Base b = d; // this is copy (real Base) - slicing
    // Derived dd = b; // CE but static_cast
    // Derived dd = static_cast<Derived>(b); // CE! need reference to Base that Derived 

    std::cout << '\n';
    Derived ddd;



    return 0;
}