#include<iostream>
#include<functional>

struct MyCompare {
    bool operator()(int a, int b) const {
        return a > b;
    }
};

bool compare(int a, int b) {
    return a < b;
}

struct S {
    int x = 0;
    void print(int y) {
        std::cout << x + y << '\n';
    }
};

template <typename... Args>
class function;

template <typename Ret, typename... Args>
class function<Ret(Args...)> {
    struct Base {
        virtual Ret operator()(Args...) = 0;
        ~Base() = default;
    };

    template<typename Functor>
    struct Derived: Base {
        Functor object;

        virtual Ret operator()(Args... args) override {
            return object(args...);
        }

        Derived(const Functor& f): object(f) {}
    };

    Base* f = nullptr;

public:
    template <typename F>
    function(const F& f): f(new Derived<F>(f)) {}

    ~function() {
        delete f;
    }

    Ret operator()(Args... args) const {
        return f->operator()(args...);
    }
}; 

// I am tired, sorry

int main() {
    std::function<bool(int, int)> f = [](int x, int y) {
        return x < y;
    };
    std::cout << f(1, 2) << ' ';
    f = MyCompare();
    std::cout << f(1, 2) << ' ';
    f = &compare;
    std::cout << f(1, 2) << '\n';

    S s{4};
    void (S::*pm)(int) = &S::print;
    std::function<void(S&, int)> f2 = pm;
    f2(s, 4);

    return 0;
}