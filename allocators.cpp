#include<iostream>

template <typename T>
struct allocator_traits {
    template <typename... Args>
    static void construct(Alloc& alloc, typename Alloc::value_type* ptr, 
                            const Args&... args) {
        if constexpr (/* Alloc has method construct */) {
            alloc.construct(ptr, args...);
        } else {
            new (ptr) typename Alloc::value_type(args...);
        }
    }

    template <typename U>
    using rebind_alloc = ...;
};

template <typename T>
struct allocator { // stateless class
    T* allocate(size_t count) {
        return reinterpret_cast<T*>(new char[count * sizeof(T)]);
    }

    void deallocate(T* ptr, size_t) {
        delete[] reinterpret_cast<char*>(ptr);
    }

    template <typename... Args> // optional 
    void construct(T* ptr, const Args&... args) {
        new (ptr) T(args...);
    }

    void destroy(T* ptr) { // optional 
        ptr -> ~T();
    }

    template <typename U>
    struct rebind { // optional 
        using other = allocator<U>;
    };

    template <typename U>
    allocator(const allocator<U>&) {}
    // and operator=
};

template <typename T>
struct Poolallocator { // need copy constructor and copy assignment
    Poolallocator(): pool(reinterpret_cast<T*>(new char[100'000 * sizeof(T)])) {}

    T* allocate(size_t count) {
        int old = shift;
        shift += count;
        return pool + old;
    }

    void deallocate(T*, size_t) {}

    ~Poolallocator() {
        delete[] reinterpret_cast<char*>(pool);
    }

    template <typename U>
    struct rebind {
        using other = allocator<U>;
    };

  private:
    T* pool;
    int shift = 0;
};

// Stackallocator - Poolallocator on stack

template <typename Node, typename Alloc = std::allocator<T>>
struct S {
    // using NodeAlloc = typename Alloc::template rebind<Node>::other;
    using NodeAlloc = typename std::allocator_traits<Alloc>::template rebind_alloc<Node>;
    NodeAlloc nodealloc;

    S(const Alloc& alloc = Alloc()): nodealloc(alloc){}

    //  std::allocator_traits<Alloc>::construct(...);
};

int main() {
    // container -> allocator -> new -> malloc -> OS kernel

    /*
    * Allocator should be lightweight
    * select_on_container_copy_construction - object of alloc when copy container
    * propagate_on_container_copy_assignment - struct true/false: save object of alloc when assign container
    * doesn't throw exceptions
    * we can derive private container from Alloc (or [[no_unique_address]]) - EBO
    */

    return 0;
}