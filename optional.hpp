/*
 * taken from http://stackoverflow.com/questions/11875045/implementing-boostoptional-in-c11
 */
#import <exception>
#import <array>
#import <type_traits>
#import <cassert>

template<typename T, class Enable = void>
class optional;

template<typename T>
optional<T> make_optional(const T& val)
{
	return optional<T>(val);
}

template<typename T>
optional<T> make_optional(T&& val)
{
	return optional<T>(std::move(val));
}

template<typename T>
class optional<T, typename std::enable_if<!std::is_reference<T>::value>::type>
{
	friend class optional<T&>;
	friend class optional<const T&>;
private:
    bool valid;
    // needs to be a union, b/c in a union nothing is initialized by default
    union {
        T value;
    };
    
    void destruct_if_valid()
    {
        if (valid) {
            value.~T();
        }
    }
    
public:
    // default constructor
    optional() : valid(false) {}
    
    // initializer
    explicit optional(const T& _v)
    {
        valid = true;
        new (&value) T(_v);
    }

	explicit optional(T&& _v)
    {
        valid = true;
        new (&value) T(std::move(_v));
    }
    
    // copy constructor
    optional(const optional& other)
    {
        valid = other.valid;
        if (valid) {
            new (&value) T(other.value);
        }
    }
    
    optional& operator=(const optional& other)
    {
        destruct_if_valid();
        valid = other.valid;
        new (&value) T(other.value);
        return *this;
    }
    
    optional& operator=(optional&& other)
    {
        destruct_if_valid();
        valid = other.valid;
        if (valid) {
            new (&value) T(std::move(other.value));
            other.valid = false;
        }
        return *this;
    }
    
    // move constructor
    optional(optional&& other)
    {
        valid = other.valid;
        if (valid) {
            new (&value) T(std::move(other.value));
            other.valid = false;
        }
    }

    ~optional()
    {
        destruct_if_valid();
    }

    bool operator!() const { return !valid; }
    
    operator bool() const { return valid; }

    T& operator*()
    {
        assert(valid);
        if (valid) return value;
        throw std::bad_exception();
    }
    
    const T& operator*() const
    {
        assert(valid);
        if (valid) return value;
        throw std::bad_exception();
    }
    
    T* operator->()
    {
        assert(valid);
        if (valid) return &value;
        throw std::bad_exception();
    }
    
    const T* operator->() const
    {
        assert(valid);
        if (valid) return &value;
        throw std::bad_exception();
    }
    
    template<typename... Args>
    void emplace(Args... args)
    {
        destruct_if_valid();
        valid = true;
        new (&value) T(args...);
    }
    
    void clear()
    {
        destruct_if_valid();
        valid = false;
    }
};
