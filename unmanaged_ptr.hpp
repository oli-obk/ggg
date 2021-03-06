#import <cstddef>
#import <memory>
#import <cassert>
#import <type_traits>	

template<typename T>
class unmanaged_ptr
{
private:
    T* value;

public:
    // default constructor
    constexpr unmanaged_ptr() noexcept : value(nullptr) {}
    
    constexpr unmanaged_ptr(nullptr_t) noexcept : value(nullptr) {}
    
    // initializer
    constexpr unmanaged_ptr(T* v) noexcept : value(v) {}
    unmanaged_ptr& operator=(T* v) noexcept { value = v; return *this; }
    constexpr unmanaged_ptr(std::unique_ptr<T>* v) noexcept : value(v.get()) {}
    unmanaged_ptr& operator=(std::unique_ptr<T>& v) noexcept { value = v.get(); return *this; }
    constexpr unmanaged_ptr(std::shared_ptr<T>* v) noexcept : value(v.get()) {}
    unmanaged_ptr& operator=(std::shared_ptr<T>& v) noexcept { value = v.get(); return *this; }
    
    // copy constructor
    constexpr unmanaged_ptr(const unmanaged_ptr<T>& other) noexcept
    :value(other.value)
    {
    }
    
    unmanaged_ptr& operator=(const unmanaged_ptr<T>& other) noexcept = default;
    
    ~unmanaged_ptr()
    {
        // do nothing, we do not own this pointer
    }

    bool operator!() const noexcept { return !value; }
    
    explicit operator bool() const  noexcept{ return value; }

    T& operator*()
    {
        assert(value);
        if (value) return *value;
        throw std::bad_exception();
    }
    
    const T& operator*() const
    {
        assert(value);
        if (value) return *value;
        throw std::bad_exception();
    }
    
    T* operator->()
    {
        assert(value);
        if (value) return value;
        throw std::bad_exception();
    }
    
    const T* operator->() const
    {
        assert(value);
        if (value) return value;
        throw std::bad_exception();
    }
    
    void reset() noexcept
    {
        value = nullptr;
    }
    
    void reset(T* ptr) noexcept
    {
        value = ptr;
    }
    
    void reset(std::unique_ptr<T>& ptr) noexcept
    {
        value = ptr.get();
    }
    
    void reset(std::shared_ptr<T>& ptr) noexcept
    {
        value = ptr.get();
    }
    
    void clear() noexcept
    {
        value = nullptr;
    }
    
    bool operator!=(const T* ptr) const noexcept
    {
        return value != ptr;
    }
    
    bool operator!=(const unmanaged_ptr<T>& ptr) const noexcept
    {
        return value != ptr.value;
    }
    
    bool operator!=(const std::unique_ptr<T>& ptr) const noexcept
    {
        return value != ptr.get();
    }
    
    bool operator!=(const std::shared_ptr<T>& ptr) const noexcept
    {
        return value != ptr.get();
    }
    
    bool operator==(const T* ptr) const noexcept
    {
        return value == ptr;
    }
    
    bool operator==(const unmanaged_ptr<T>& ptr) const noexcept
    {
        return value == ptr.value;
    }
    
    bool operator==(const std::unique_ptr<T>& ptr) const noexcept
    {
        return value == ptr.get();
    }
    
    bool operator==(const std::shared_ptr<T>& ptr) const noexcept
    {
        return value == ptr.get();
    }
    
    T* get() noexcept
    {
        return value;
    }
    
    const T* get() const noexcept
    {
        return value;
    }
    
    bool operator<(const unmanaged_ptr& rhs) const
    {
        return value < rhs.value;
    }
};

template<typename T>
bool operator==(const std::unique_ptr<T>& lhs, const unmanaged_ptr<T>& rhs)
{
    return rhs == lhs;
}

template<typename T>
bool operator!=(const std::unique_ptr<T>& lhs, const unmanaged_ptr<T>& rhs)
{
    return rhs != lhs;
}

template<typename T>
bool operator==(const std::shared_ptr<T>& lhs, const unmanaged_ptr<T>& rhs)
{
    return rhs == lhs;
}

template<typename T>
bool operator!=(const std::shared_ptr<T>& lhs, const unmanaged_ptr<T>& rhs)
{
    return rhs != lhs;
}

template<typename T>
bool operator==(const T* lhs, const unmanaged_ptr<T>& rhs)
{
    return rhs == lhs;
}

template<typename T>
bool operator!=(const T* lhs, const unmanaged_ptr<T>& rhs)
{
    return rhs != lhs;
}
