#include <iostream>

struct Base { // Polymorphic type
    virtual void f(int) { std::cout << "Base" << '\n'; }
    virtual ~Base() { std::cout << "Base destructor" << '\n'; };
};

struct Derived: Base { // Polymorphic type
    int* p = new int;
    ~Derived() {
        delete p;
        std::cout << "Derived destructor" << '\n';
    }

    void f(int) { std::cout << "Derived" << '\n'; }
};

struct Son: Derived { // Polymorphic type
  private: // private doesn't affect
    void f(int) override { std::cout << "Son int" << '\n'; }
    void f(double) { std::cout << "Son double" << '\n'; } // override - CE if another signature
    // final = override + not allowed write down
    // virtual/override/final
};

struct Shape { // Abstract class (not allowed to create objects)
    virtual double area() const = 0; // pure virtual method
    virtual ~Shape() = 0;
};

struct Granny {
    int g;
    virtual ~Granny() = default;
};
struct Mother: Granny { int m; };
struct Father: Granny { int f; };
struct Son2: Mother, Father { int s; };


int main() {
    Son s;
    Base& b = s; // pointer too but not copy
    b.f(3.0); // must be similar signature (const too), another return type - CE
    
    Base* b_ptr = new Derived(); // memory leak without virtual destructor
    delete b_ptr;

    // RTTI - Runtime type information
    int x;
    std::cin >> x;
    Base* p = x % 2 ? new Base() : new Derived();
    p->f(x); // choose version in runtime
    std::cout << typeid(*p).name() << '\n'; // runtime check
    delete p;

    // dynamic_cast
    // cast down with check what reference is
    // only polymorphic types (need rtti)
    // but can't solve not virtual Granny
    // Diamond problem
    Son ss;
    Mother& mm = ss;
    // static_cast<Father&>(m); // CE - right not allowed
    dynamic_cast<Father&>(m); // correct
    reinterpret_cast<Father&>(m); // UB
    
    // virtual function must be definied 
    // virtual function in constructor - not virtual
    // pure virtual function in constructor not allowed
    // can cheat: function that calls pure virtual - RE (pure virtual method called)
    // virtual function with default values - call down with up value (compile + runtime)
    // pointer to member support virtual (first bit says that virtual)

    return 0;
}
