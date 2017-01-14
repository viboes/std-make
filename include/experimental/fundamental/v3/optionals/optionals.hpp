// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Copyright (C) 2016 Vicente J. Botet Escriba

#ifndef JASEL_FUNDAMENTAL_V3_OPTIONALS_OPTIONALS_HPP
#define JASEL_FUNDAMENTAL_V3_OPTIONALS_OPTIONALS_HPP

#include <tuple>
#include <experimental/optional.hpp>

// todo
// * make use of uninitialized storage
// * add index based functions
// * add move semantics
// * add emplace
// * add hash support
// * add SFINAE for requires conditions
// * try to refactor optional_ref<T> and optional_ref<const T>
// * Can a optional_ref<U> be converted to an optional_ref<T>? Under which conditions?
//    T -> const T or
//    U* is convertible to T* and sizeof(T) == sizeof(U) ?

namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{

namespace detail {

template <class T>
struct optional_ref_fact;

}

/**
optional_ref is a proxy to a pseudo optional<T> included in optionals<Ts...>
It behaves like an optional<T> and it is explicitly convertible to optional<T>.
*/
template <class T>
class optional_ref {
public:

  //optional_ref(optional_ref const&) = default;
  //optional_ref(optional_ref &&) = default;
  //optional_ref& operator=(optional_ref const&) = default;
  //optional_ref& operator=(optional_ref &&) = default;

  //optional_ref(optional_ref<U> const&); // ???
  //optional_ref(optional_ref<U> &&); // ???
  //optional_ref& operator=(optional_ref<U> const&); // ???
  //optional_ref& operator=(optional_ref<U> &&); // ???

    template< class U = T >
    optional_ref& operator=(U && v)
    {
        *ref = forward<U>(v);
        set();
        return *this;
    }
    void reset()
    {
        *mask &= ~(1 << index);
    }
    optional_ref& operator=(nullopt_t)
    {
        reset();
        return *this;
    }


    bool has_value() const
    {
        return (*mask & (1 << index));
    }
    explicit operator bool() const { return has_value(); }

    T* get()
    {
        return ref;
    }
    T const* get() const
    {
        return ref;
    }
    T* operator->()
    {
        return get();
    }
    T const* operator->() const
    {
        return get();
    }
    T& operator*()
    {
        return *get();
    }
    T const& operator*() const
    {
        return *get();
    }

    T& value()
    {
        if (has_value())
            return *get();
        throw bad_optional_access("");
    }
    T const& value() const
    {
        if (has_value())
            return *get();
        throw bad_optional_access("");
    }

    template <class U>
    T value_or(U const& v) const
    {
        if (has_value())
            return *get();
        return v;
    }

    explicit operator optional<T>() const
    {
      if (has_value())
        return *get();
      return nullopt;
    }

    friend bool operator == (const optional_ref & x, const optional_ref & y)
    { return optional<T>(x) == optional<T>(y); }
    friend bool operator != (const optional_ref & x, const optional_ref & y)
    { return optional<T>(x) != optional<T>(y); }
    friend bool operator < (const optional_ref & x, const optional_ref & y)
    { return optional<T>(x) < optional<T>(y); }
    friend bool operator <= (const optional_ref & x, const optional_ref & y)
    { return optional<T>(x) <= optional<T>(y); }
    friend bool operator > (const optional_ref & x, const optional_ref & y)
    { return optional<T>(x) > optional<T>(y); }
    friend bool operator >= (const optional_ref & x, const optional_ref & y)
    { return optional<T>(x) >= optional<T>(y); }

    friend bool operator == (const optional_ref & x, nullopt_t y)
    { return optional<T>(x) == y; }
    friend bool operator != (const optional_ref & x, nullopt_t y)
    { return optional<T>(x) != y; }
    friend bool operator < (const optional_ref & x, nullopt_t y)
    { return optional<T>(x) < y; }
    friend bool operator <= (const optional_ref & x, nullopt_t y)
    { return optional<T>(x) <= y; }
    friend bool operator > (const optional_ref & x, nullopt_t y)
    { return optional<T>(x) > y; }
    friend bool operator >= (const optional_ref & x, nullopt_t y)
    { return optional<T>(x) >= y; }

    friend bool operator == (nullopt_t x, const optional_ref & y)
    { return x == optional<T>(y); }
    friend bool operator != (nullopt_t x, const optional_ref & y)
    { return x != optional<T>(y); }
    friend bool operator < (nullopt_t x, const optional_ref & y)
    { return x < optional<T>(y); }
    friend bool operator <= (nullopt_t x, const optional_ref & y)
    { return x <= optional<T>(y); }
    friend bool operator > (nullopt_t x, const optional_ref & y)
    { return x > optional<T>(y); }
    friend bool operator >= (nullopt_t x, const optional_ref & y)
    { return x >= optional<T>(y); }

    friend bool operator == (const optional_ref & x, const T&  y)
    { return optional<T>(x) == y; }
    friend bool operator != (const optional_ref & x, const T&  y)
    { return optional<T>(x) != y; }
    friend bool operator < (const optional_ref & x, const T&  y)
    { return optional<T>(x) < y; }
    friend bool operator <= (const optional_ref & x, const T&  y)
    { return optional<T>(x) <= y; }
    friend bool operator > (const optional_ref & x, const T&  y)
    { return optional<T>(x) > y; }
    friend bool operator >= (const optional_ref & x, const T&  y)
    { return optional<T>(x) >= y; }

    friend bool operator == (const T&  x, const optional_ref & y)
    { return x == optional<T>(y); }
    friend bool operator != (const T&  x, const optional_ref & y)
    { return x != optional<T>(y); }
    friend bool operator < (const T&  x, const optional_ref & y)
    { return x < optional<T>(y); }
    friend bool operator <= (const T&  x, const optional_ref & y)
    { return x <= optional<T>(y); }
    friend bool operator > (const T&  x, const optional_ref & y)
    { return x > optional<T>(y); }
    friend bool operator >= (const T&  x, const optional_ref & y)
    { return x >= optional<T>(y); }

    void swap(optional_ref& rhs) noexcept
    {
            std::swap(index, rhs.index);
            std::swap(mask, rhs.mask);
            std::swap(ref, rhs.ref);
    }
private:

    friend detail::optional_ref_fact<T>;
    optional_ref(size_t i, size_t& m, T& r)
        : index(i), mask(&m), ref(&r) {}

    void set()
    {
        *mask |= (1 << index);
    }

    size_t index;
    size_t* mask;
    T* ref;
};

template <class T>
class optional_ref<const T> {
public:
  //optional_ref(optional_ref const&) = default;
  //optional_ref(optional_ref &&) = default;
  //optional_ref& operator=(optional_ref const&) = default;
  //optional_ref& operator=(optional_ref &&) = default;

    bool has_value() const
    {
        return *mask & (1 << index);
    }
    explicit operator bool() const { return has_value(); }

    T const* get() const
    {
        return ref;
    }
    T const* operator->() const
    {
        return get();
    }
    T const& operator*() const
    {
        return *get();
    }
    T const& value() const
    {
        if (has_value())
            return *get();
        throw bad_optional_access("");
    }
    template <class U>
    T value_or(U const& v) const
    {
        if (has_value())
            return *get();
        return v;
    }

    explicit operator optional<T>() const
    {
      if (has_value())
        return *get();
      return nullopt;
    }

    friend bool operator == (const optional_ref & x, const optional_ref & y)
    { return optional<T>(x) == optional<T>(y); }
    friend bool operator != (const optional_ref & x, const optional_ref & y)
    { return optional<T>(x) != optional<T>(y); }
    friend bool operator < (const optional_ref & x, const optional_ref & y)
    { return optional<T>(x) < optional<T>(y); }
    friend bool operator <= (const optional_ref & x, const optional_ref & y)
    { return optional<T>(x) <= optional<T>(y); }
    friend bool operator > (const optional_ref & x, const optional_ref & y)
    { return optional<T>(x) > optional<T>(y); }
    friend bool operator >= (const optional_ref & x, const optional_ref & y)
    { return optional<T>(x) >= optional<T>(y); }

    friend bool operator == (const optional_ref & x, nullopt_t y)
    { return optional<T>(x) == y; }
    friend bool operator != (const optional_ref & x, nullopt_t y)
    { return optional<T>(x) != y; }
    friend bool operator < (const optional_ref & x, nullopt_t y)
    { return optional<T>(x) < y; }
    friend bool operator <= (const optional_ref & x, nullopt_t y)
    { return optional<T>(x) <= y; }
    friend bool operator > (const optional_ref & x, nullopt_t y)
    { return optional<T>(x) > y; }
    friend bool operator >= (const optional_ref & x, nullopt_t y)
    { return optional<T>(x) >= y; }

    friend bool operator == (nullopt_t x, const optional_ref & y)
    { return x == optional<T>(y); }
    friend bool operator != (nullopt_t x, const optional_ref & y)
    { return x != optional<T>(y); }
    friend bool operator < (nullopt_t x, const optional_ref & y)
    { return x < optional<T>(y); }
    friend bool operator <= (nullopt_t x, const optional_ref & y)
    { return x <= optional<T>(y); }
    friend bool operator > (nullopt_t x, const optional_ref & y)
    { return x > optional<T>(y); }
    friend bool operator >= (nullopt_t x, const optional_ref & y)
    { return x >= optional<T>(y); }

    friend bool operator == (const optional_ref & x, const T&  y)
    { return optional<T>(x) == y; }
    friend bool operator != (const optional_ref & x, const T&  y)
    { return optional<T>(x) != y; }
    friend bool operator < (const optional_ref & x, const T&  y)
    { return optional<T>(x) < y; }
    friend bool operator <= (const optional_ref & x, const T&  y)
    { return optional<T>(x) <= y; }
    friend bool operator > (const optional_ref & x, const T&  y)
    { return optional<T>(x) > y; }
    friend bool operator >= (const optional_ref & x, const T&  y)
    { return optional<T>(x) >= y; }

    friend bool operator == (const T&  x, const optional_ref & y)
    { return x == optional<T>(y); }
    friend bool operator != (const T&  x, const optional_ref & y)
    { return x != optional<T>(y); }
    friend bool operator < (const T&  x, const optional_ref & y)
    { return x < optional<T>(y); }
    friend bool operator <= (const T&  x, const optional_ref & y)
    { return x <= optional<T>(y); }
    friend bool operator > (const T&  x, const optional_ref & y)
    { return x > optional<T>(y); }
    friend bool operator >= (const T&  x, const optional_ref & y)
    { return x >= optional<T>(y); }

    void swap(optional_ref& rhs) noexcept
    {
            std::swap(index, rhs.index);
            std::swap(mask, rhs.mask);
            std::swap(ref, rhs.ref);
    }
private:

    friend detail::optional_ref_fact<T>;
    optional_ref(size_t i, size_t const& m, T const& r)
        : index(i), mask(&m), ref(&r) {}


    size_t index;
    size_t const* mask;
    T const* ref;
};

template <class T>
void swap(optional_ref<T>& x, optional_ref<T>& y)
{
    return x.swap(y);
}
namespace detail {
template <class T>
struct optional_ref_fact {
    static optional_ref<T> make(size_t index, size_t& mask, T& ref)
    {
        return optional_ref<T>(index, mask, ref);
    };
    static optional_ref<const T> make(size_t index, size_t const& mask, T const& ref)
    {
        return optional_ref<const T>(index, mask, ref);
    };
};
}

/**
optionals<Ts..> behaves similarly to tuple<optional<Ts>...>
get<T> returns a optional_ref<T> that behaves as a proxy.
*/
template <class ...Ts>
class optionals: detail::optional_ref_fact<Ts>...
{
    using Types = tuple<Ts...>;

public:

    optionals(): mask(0) {}
    //optionals(optionals const&) = default;
    //optionals(optionals &&) = default;
    //optionals& operator=(optionals const&) = default;
    //optionals& operator=(optionals &&) = default;

    template <size_t I>
    bool has_value() const
    {
        return mask & (1 << I);
    }
    template <class T>
    bool has_value() const;
    //{
    //    return mask & (1 << index<Types, T>::value);
    //}

    template <size_t I>
    optional_ref<typename tuple_element<I, Types>::type> get()
    {
        using T = typename tuple_element<I, Types>::type;
        return this->detail::optional_ref_fact<T>::make(I, mask, std::get<I>(tpl));
    }
    template <size_t I>
    optional_ref<const typename tuple_element<I, Types>::type> get() const
    {
        using T = typename tuple_element<I, Types>::type;
        return this->detail::optional_ref_fact<T>::make(I, mask, std::get<I>(tpl));
    }

    template <class T>
    optional_ref<T> get();
    //{        return get<index<Types, T>::value>(tpl);    }
    template <class T>
    optional_ref<const T> get() const;
    //{        return get<index<Types, T>::value>(tpl);    }

    template <class T>
    optionals& operator=(T const& v)
    {
        this->template get<T>() = v;
        return *this;
    }

    template <class T>
    void reset()
    {
        this->template get<T>().reset();
    }

private:

    template <size_t I>
    void set()
    {
        mask |= (1 << I);
    }

    size_t mask;
    Types tpl;
};

template <size_t I, class ...Ts>
auto get(optionals<Ts...>& opts) -> decltype(opts.template get<I>())
{
    return opts.template get<I>();
}
template <size_t I, class ...Ts>
auto get(optionals<Ts...> const& opts) -> decltype(opts.template get<I>())
{
    return opts.template get<I>();
}

template <class T, class ...Ts>
auto get(optionals<Ts...>& opts) -> decltype(opts.template get<T>())
{
    return opts.template get<T>();
}
template <class T, class ...Ts>
auto get(optionals<Ts...> const& opts) -> decltype(opts.template get<T>())
{
    return opts.template get<T>();
}

template <class ...Ts>
void swap(optionals<Ts...>& x, optionals<Ts...>& y)
{
    return std::swap(x, y);
}

}} // experimental/fundamentals_v3/
} // std
#endif // header
