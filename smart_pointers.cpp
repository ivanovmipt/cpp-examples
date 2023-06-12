#include<iostream>
#include<memory>

template <typename T>
struct default_delete {
    void operator()(T* ptr) {
        delete ptr;
    }
};

template <typename T, typename Deleter = std::default_delete<T>>
class UniquePtr: private Deleter { // EBO
    T* ptr;
    Deleter deleter;
  public:
    UniquePtr(T* ptr): ptr(ptr) {}

    UniquePtr(const UniquePtr&) = delete; // can't copy but move is allowed

    UniquePtr& operator=(const UniquePtr&) = delete;

    UniquePtr(UniquePtr&& u): ptr(u.ptr) {
        u.ptr = nullptr;
    }

    UniquePtr& operator=(UniquePtr&& u) {
        UniquePtr copy = std::move(u);
        std::swap(ptr, copy.ptr);
        return *this;
    }

    ~UniquePtr() { deleter(ptr); }
};

/*
template <typename T>
class UniquePtr<T[]> {
    T* ptr;
  public:
    UniquePtr(T* ptr): ptr(ptr) {}
    ~UniquePtr() { delete ptr[]; }
};
*/

template <typename T>
class WeakPtr;

template <typename T>
class SharedPtr {
    friend class WeakPtr<T>;

    struct ControlBlock {
        T object;
        int count;

        ControlBlock(const T& object, int count): object(object), count(count) {}
    };

    T* ptr;
    int* count;
    ControlBlock* cb;

    
  public:
    SharedPtr(T* ptr): ptr(ptr), count(new int) {} // too much new

    SharedPtr(const SharedPtr& other): ptr(other.ptr), count(other.count) {
        ++*count; // TODO: if nullptr or cb
    }

    ~SharedPtr() {
        --*count;
        if (*count == 0) {
            delete ptr;
            delete count;
        }
    }
};

template<typename T, typename... Args>
SharedPtr<T> make_shared(Args&&... args) { // fewer calls of new
    auto cb = new typename SharedPtr<T>::ControlBlock(T(std::forward<Args>(args)...), 1); 
    return SharedPtr<T>(cb);
}

template<typename T, typename... Args>
UniquePtr<T> make_unique(Args&&... args) {
    return UniquePtr<T>(new T(std::forward<Args>(args)...));
}

template <typename T>
class SharedPtr_OkLetsGoooo {
    friend class WeakPtr<T>;

    struct BaseControlBlock {
        int shared_count;
        int weak_count;
    };
    
    struct ControlBlock: BaseControlBlock {
        T object;
    };

    T* ptr;
    ControlBlock* cb;

    
  public:
    SharedPtr(T* ptr): ptr(ptr) {} // too much new

    SharedPtr(const SharedPtr& other): ptr(other.ptr) {}

    ~SharedPtr() {
        --cb->shared_count;
        if (cb->shared_count == 0 && cb->weak_count == 0) {
            delete cb; // TODO: 2 cases
        } else {
            if (ptr) {
                delete ptr;
            } else {
                static_cast<ControlBlock*>(cb)->object.~T();
            }
        }
    }
};

/*
void f(unique_ptr(new int), expr) {}

may be:
1. new
2. expr (throw)
3. unique
memory leak

solution: make_shared
*/

template <typename T>
class WeakPtr { // can't call destructor
    T* ptr;
    int* count;
    SharedPtr<T>::ControlBlock* cb;

  public:
    WeakPtr(const SharedPtr<T>& other) {}

    bool expired() const noexcept {}

    SharedPtr<T> lock() const noexcept;

    ~WeakPtr() {
        /*
        if we last and there aren't sharedptrs than free memory
        */
    }
};

template <typename T>
class SharedPtr_OkLetsGoooo2 {
    friend class WeakPtr<T>;

    struct BaseControlBlock {
        int shared_count;
        int weak_count;
    };
    
    struct ControlBlock: BaseControlBlock {
        T object;
    };

    T* ptr;
    ControlBlock* cb;

    
  public:
    SharedPtr(T* ptr): ptr(ptr){
        if constexpr (std::is_base_of_v<enable_shared_from_this<T>, T>) {
            ptr -> wptr = *this;
        }
    } // too much new

    SharedPtr(const SharedPtr& other): ptr(other.ptr) {}

    ~SharedPtr() {
        --cb->shared_count;
        if (cb->shared_count == 0 && cb->weak_count == 0) {
            delete cb; // TODO: 2 cases
        } else {
            if (ptr) {
                delete ptr;
            } else {
                static_cast<ControlBlock*>(cb)->object.~T();
            }
        }
    }
};

template <typename T>
struct enable_shared_from_this {
private:
    WeakPtr<T> wptr;
public:
    SharedPtr<T> shared_from_this() const noexcept {
        return wptr.lock();
    }
};


struct S: public std::enable_shared_from_this<S> { // CRTP idiom - Curiously Recurcive Template Pattern
    SharedPtr<S> getPtr() {
        return shared_from_this();
    }
};

// not standart allocator - for counters

template <typename T>
class SharedPtr_OkLetsGoooo3{
    friend class WeakPtr<T>;

    struct BaseControlBlock {
        int shared_count;
        int weak_count;
        virtual useDeleter() = 0;
        virtual ~BaseControlBlock(T* ptr) = default; // type erasure
    };
    
    template <typename Alloc, typename Deleter>
    struct ControlBlockRegular: BaseControlBlock {
        Deleter deleter;
        Alloc alloc;
        T* object;

        virtual void useDeleter(T* ptr) {

        }
    };

    template <typename Alloc>
    struct ControlBlockMakeShared: BaseControlBlock {
        Alloc alloc;
        T object;
    };

    T* ptr;
    ControlBlock* cb;

    
  public:
    SharedPtr(T* ptr): cb(new ControlBlockRegular{1, 0, ptr, deleter}) {
        if constexpr (std::is_base_of_v<enable_shared_from_this<T>, T>) {
            ptr -> wptr = *this;
        }
    } // too much new

    SharedPtr(const SharedPtr& other): ptr(other.ptr) {}

    ~SharedPtr() {
        --cb->shared_count;
        if (cb->shared_count == 0 && cb->weak_count == 0) {
            delete cb; // TODO: 2 cases
        } else {
            if (ptr) {
                delete ptr;
            } else {
                static_cast<ControlBlock*>(cb)->object.~T();
            }
        }
    }
};

int main() {
    std::unique_ptr<int> p1(new int);
    std::unique_ptr<int, default_delete<int>> p2(new int);
    // not allowed to make several smartptrs to one c-ptr

    std::shared_ptr<int> sp1(new int);
    auto sp2 = std::make_shared<int>(5);

    return 0;
}
