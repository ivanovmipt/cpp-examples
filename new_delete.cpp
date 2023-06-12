#include<iostream>
#include<cstddef>

// weak signature
void* operator new(size_t count) { // count - bytes
    if (count == 0) {
        count = 1;
    }

    std::cout << count << '\n';

    // new_handler() if return nullptr
    // std::bad_alloc() if nullptr

    return malloc(count); 
}

void* operator new(size_t count, const std::nothrow_t&) { // return nullptr
    return malloc(count); // incorrect
}

// placement new

void operator delete(void* ptr) {
    std::cout << "Global delete" << '\n';
    free(ptr);
}

// [] version is equal standart but has number of bytes
// Destroying delete version

// if call new(args) than call delete(args)

struct S {
    long long q;
    S() {std::cout << "1" << '\n'; }
    ~S() {std::cout << "2" << '\n'; }

    void* operator new(size_t count) { // static as default
        std::cout << "AAAA!" << '\n';
        return malloc(count);
    }

    void operator delete(void* ptr) {
        std::cout << "Local delete" << '\n';
        free(ptr);
    }
};

struct alignas(float) F {
    /* data */
};


int main() {
    S* ps = new S();
    // S* ps = new(std::nothrow) S();
    delete ps;

    S* pss = new S[5];
    // S* ps = new(std::nothrow) S();
    delete[] pss;

    // Alignments
    std::cout << alignof(int) << ' ' << alignof(char) << '\n'; // 4 1
    std::cout << alignof(F) << ' ' << alignof(float) << '\n'; // 4 == 4
    std::cout << alignof(std::max_align_t()) << '\n';
    // ptr = std::align(alignof(F), size, ptr, offset);
    // alignas 64 for unusual processor instructions

    return 0;
}
