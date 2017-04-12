// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Copyright (C) 2016,2017 Vicente J. Botet Escriba

#ifndef JASEL_FUNDAMENTAL_V3_OPTIONALS_OPTIONALS_HPP
#define JASEL_FUNDAMENTAL_V3_OPTIONALS_OPTIONALS_HPP

#include <tuple>
#include <experimental/optional.hpp>
# include <experimental/fundamental/v2/config.hpp>

// todo
// * make use of uninitialized storage
// * add index based functions
// * add move semantics
// * add emplace
// * add hash support
// * add SFINAE for requires conditions
// * try to refactor optional_ref<T> and optional_ref<const T>
//
// Open points:
// * Can a optional_ref<U> be converted to an optional_ref<T>? Under which conditions?
//    T -> const T or
//    U* is convertible to T* and sizeof(T) == sizeof(U) ?
// * Should the type of the mask be a parameter?

// Related classes
// * add a pod::optionals
// * add a optionals_view that takes a reference to an instance, and it is parameterized by the mask data member pointer and the tuple of each of the value data member pointer
//  template <class Mask, class ...Values>
//  class optionals_view;
//  template <class C, class MaskT, MaskT C::*Mask, class ...T, T C::*Value...>
//  class optionals_view< member<class C, class MaskT, MaskT C::*Mask>, member<class C, class T, T C::*Value>...>;
// What all these classes have in common
// If we make abstraction of the storage, all these classes define almost the same interface.
//



namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{

namespace detail {
  template <class Tpl, class T>
  struct index;
  template <class T, class ...Ts>
  struct index<std::tuple<T, Ts...>, T> { enum {value = 0}; };
  template <class T, class U, class ...Ts>
  struct index<std::tuple<T, Ts...>, U> { enum {value = 1 + index<std::tuple<Ts...>, U>::value}; };
}

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

  optional_ref(optional_ref const&) = default;
  optional_ref(optional_ref &&) = default;
  optional_ref& operator=(optional_ref const&) = default;
  optional_ref& operator=(optional_ref &&) = default;

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
    void reset() noexcept
    {
        *mask &= ~(1 << index);
    }
    optional_ref& operator=(nullopt_t) noexcept
    {
        reset();
        return *this;
    }


    constexpr bool has_value() const noexcept
    {
        return (*mask & (1 << index));
    }
    constexpr explicit operator bool() const noexcept { return has_value(); }

    JASEL_CXX14_CONSTEXPR T* get()
    {
        return ref;
    }
    constexpr T const* get() const
    {
        return ref;
    }
    JASEL_CXX14_CONSTEXPR T* operator->()
    {
        return get();
    }
    constexpr T const* operator->() const
    {
        return get();
    }
    JASEL_CXX14_CONSTEXPR T& operator*()
    {
        return *get();
    }
    constexpr T const& operator*() const
    {
        return *get();
    }

    JASEL_CXX14_CONSTEXPR T& value()
    {
        if (has_value())
            return *get();
        throw bad_optional_access("");
    }
    JASEL_CXX14_CONSTEXPR T const& value() const
    {
        if (has_value())
            return *get();
        throw bad_optional_access("");
    }

    template <class U>
    JASEL_CXX14_CONSTEXPR T value_or(U const& v) const
    {
        if (has_value())
            return *get();
        return v;
    }

    JASEL_CXX14_CONSTEXPR explicit operator optional<T>() const
    {
      if (has_value())
        return *get();
      return nullopt;
    }

    friend constexpr bool operator == (const optional_ref & x, const optional_ref & y) noexcept
    { return optional<T>(x) == optional<T>(y); }
    friend constexpr bool operator != (const optional_ref & x, const optional_ref & y) noexcept
    { return optional<T>(x) != optional<T>(y); }
    friend constexpr bool operator < (const optional_ref & x, const optional_ref & y) noexcept
    { return optional<T>(x) < optional<T>(y); }
    friend constexpr bool operator <= (const optional_ref & x, const optional_ref & y) noexcept
    { return optional<T>(x) <= optional<T>(y); }
    friend constexpr bool operator > (const optional_ref & x, const optional_ref & y) noexcept
    { return optional<T>(x) > optional<T>(y); }
    friend constexpr bool operator >= (const optional_ref & x, const optional_ref & y) noexcept
    { return optional<T>(x) >= optional<T>(y); }

    friend constexpr bool operator == (const optional_ref & x, nullopt_t y) noexcept
    { return optional<T>(x) == y; }
    friend constexpr bool operator != (const optional_ref & x, nullopt_t y) noexcept
    { return optional<T>(x) != y; }
    friend constexpr bool operator < (const optional_ref & x, nullopt_t y) noexcept
    { return optional<T>(x) < y; }
    friend constexpr bool operator <= (const optional_ref & x, nullopt_t y) noexcept
    { return optional<T>(x) <= y; }
    friend constexpr bool operator > (const optional_ref & x, nullopt_t y) noexcept
    { return optional<T>(x) > y; }
    friend constexpr bool operator >= (const optional_ref & x, nullopt_t y) noexcept
    { return optional<T>(x) >= y; }

    friend constexpr bool operator == (nullopt_t x, const optional_ref & y) noexcept
    { return x == optional<T>(y); }
    friend constexpr bool operator != (nullopt_t x, const optional_ref & y) noexcept
    { return x != optional<T>(y); }
    friend constexpr bool operator < (nullopt_t x, const optional_ref & y) noexcept
    { return x < optional<T>(y); }
    friend constexpr bool operator <= (nullopt_t x, const optional_ref & y) noexcept
    { return x <= optional<T>(y); }
    friend constexpr bool operator > (nullopt_t x, const optional_ref & y) noexcept
    { return x > optional<T>(y); }
    friend constexpr bool operator >= (nullopt_t x, const optional_ref & y) noexcept
    { return x >= optional<T>(y); }

    friend constexpr bool operator == (const optional_ref & x, const T&  y) noexcept
    { return optional<T>(x) == y; }
    friend constexpr bool operator != (const optional_ref & x, const T&  y) noexcept
    { return optional<T>(x) != y; }
    friend constexpr bool operator < (const optional_ref & x, const T&  y) noexcept
    { return optional<T>(x) < y; }
    friend constexpr bool operator <= (const optional_ref & x, const T&  y) noexcept
    { return optional<T>(x) <= y; }
    friend constexpr bool operator > (const optional_ref & x, const T&  y) noexcept
    { return optional<T>(x) > y; }
    friend constexpr bool operator >= (const optional_ref & x, const T&  y) noexcept
    { return optional<T>(x) >= y; }

    friend constexpr bool operator == (const T&  x, const optional_ref & y) noexcept
    { return x == optional<T>(y); }
    friend constexpr bool operator != (const T&  x, const optional_ref & y) noexcept
    { return x != optional<T>(y); }
    friend constexpr bool operator < (const T&  x, const optional_ref & y) noexcept
    { return x < optional<T>(y); }
    friend constexpr bool operator <= (const T&  x, const optional_ref & y) noexcept
    { return x <= optional<T>(y); }
    friend constexpr bool operator > (const T&  x, const optional_ref & y) noexcept
    { return x > optional<T>(y); }
    friend constexpr bool operator >= (const T&  x, const optional_ref & y) noexcept
    { return x >= optional<T>(y); }

    void swap(optional_ref& rhs) noexcept
    {
            std::swap(index, rhs.index);
            std::swap(mask, rhs.mask);
            std::swap(ref, rhs.ref);
    }
private:

    friend detail::optional_ref_fact<T>;
    constexpr optional_ref(size_t i, size_t& m, T& r) noexcept
        : index(i), mask(&m), ref(&r) {}

    void set() noexcept
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

    constexpr bool has_value() const noexcept
    {
        return *mask & (1 << index);
    }
    constexpr explicit operator bool() const noexcept { return has_value(); }

    constexpr T const* get() const
    {
        return ref;
    }
    constexpr T const* operator->() const
    {
        return get();
    }
    constexpr T const& operator*() const
    {
        return *get();
    }
    JASEL_CXX14_CONSTEXPR T const& value() const
    {
        if (has_value())
            return *get();
        throw bad_optional_access("");
    }
    template <class U>
    JASEL_CXX14_CONSTEXPR T value_or(U const& v) const
    {
        if (has_value())
            return *get();
        return v;
    }

    explicit JASEL_CXX14_CONSTEXPR operator optional<T>() const
    {
      if (has_value())
        return *get();
      return nullopt;
    }

    friend constexpr bool operator == (const optional_ref & x, const optional_ref & y) noexcept
    { return optional<T>(x) == optional<T>(y); }
    friend constexpr bool operator != (const optional_ref & x, const optional_ref & y) noexcept
    { return optional<T>(x) != optional<T>(y); }
    friend constexpr bool operator < (const optional_ref & x, const optional_ref & y) noexcept
    { return optional<T>(x) < optional<T>(y); }
    friend constexpr bool operator <= (const optional_ref & x, const optional_ref & y) noexcept
    { return optional<T>(x) <= optional<T>(y); }
    friend constexpr bool operator > (const optional_ref & x, const optional_ref & y) noexcept
    { return optional<T>(x) > optional<T>(y); }
    friend constexpr bool operator >= (const optional_ref & x, const optional_ref & y) noexcept
    { return optional<T>(x) >= optional<T>(y); }

    friend constexpr bool operator == (const optional_ref & x, nullopt_t y) noexcept
    { return optional<T>(x) == y; }
    friend constexpr bool operator != (const optional_ref & x, nullopt_t y) noexcept
    { return optional<T>(x) != y; }
    friend constexpr bool operator < (const optional_ref & x, nullopt_t y) noexcept
    { return optional<T>(x) < y; }
    friend constexpr bool operator <= (const optional_ref & x, nullopt_t y) noexcept
    { return optional<T>(x) <= y; }
    friend constexpr bool operator > (const optional_ref & x, nullopt_t y) noexcept
    { return optional<T>(x) > y; }
    friend constexpr bool operator >= (const optional_ref & x, nullopt_t y) noexcept
    { return optional<T>(x) >= y; }

    friend constexpr bool operator == (nullopt_t x, const optional_ref & y) noexcept
    { return x == optional<T>(y); }
    friend constexpr bool operator != (nullopt_t x, const optional_ref & y) noexcept
    { return x != optional<T>(y); }
    friend constexpr bool operator < (nullopt_t x, const optional_ref & y) noexcept
    { return x < optional<T>(y); }
    friend constexpr bool operator <= (nullopt_t x, const optional_ref & y) noexcept
    { return x <= optional<T>(y); }
    friend constexpr bool operator > (nullopt_t x, const optional_ref & y) noexcept
    { return x > optional<T>(y); }
    friend constexpr bool operator >= (nullopt_t x, const optional_ref & y) noexcept
    { return x >= optional<T>(y); }

    friend constexpr bool operator == (const optional_ref & x, const T&  y) noexcept
    { return optional<T>(x) == y; }
    friend constexpr bool operator != (const optional_ref & x, const T&  y) noexcept
    { return optional<T>(x) != y; }
    friend constexpr bool operator < (const optional_ref & x, const T&  y) noexcept
    { return optional<T>(x) < y; }
    friend constexpr bool operator <= (const optional_ref & x, const T&  y) noexcept
    { return optional<T>(x) <= y; }
    friend constexpr bool operator > (const optional_ref & x, const T&  y) noexcept
    { return optional<T>(x) > y; }
    friend constexpr bool operator >= (const optional_ref & x, const T&  y) noexcept
    { return optional<T>(x) >= y; }

    friend constexpr bool operator == (const T&  x, const optional_ref & y) noexcept
    { return x == optional<T>(y); }
    friend constexpr bool operator != (const T&  x, const optional_ref & y) noexcept
    { return x != optional<T>(y); }
    friend constexpr bool operator < (const T&  x, const optional_ref & y) noexcept
    { return x < optional<T>(y); }
    friend constexpr bool operator <= (const T&  x, const optional_ref & y) noexcept
    { return x <= optional<T>(y); }
    friend constexpr bool operator > (const T&  x, const optional_ref & y) noexcept
    { return x > optional<T>(y); }
    friend constexpr bool operator >= (const T&  x, const optional_ref & y) noexcept
    { return x >= optional<T>(y); }

    void swap(optional_ref& rhs) noexcept
    {
            std::swap(index, rhs.index);
            std::swap(mask, rhs.mask);
            std::swap(ref, rhs.ref);
    }
private:

    friend detail::optional_ref_fact<T>;
    constexpr optional_ref(size_t i, size_t const& m, T const& r) noexcept
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
    static constexpr optional_ref<T> make(size_t index, size_t& mask, T& ref) noexcept
    {
        return optional_ref<T>(index, mask, ref);
    };
    static constexpr optional_ref<const T> make(size_t index, size_t const& mask, T const& ref) noexcept
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

    constexpr optionals() noexcept
      : mask(0)  {}

    // genrated by default
    optionals(optionals const&) = default;
    optionals(optionals &&) = default;
    optionals& operator=(optionals const&) = default;
    optionals& operator=(optionals &&) = default;

    template <size_t I>
    constexpr bool has_value() const noexcept
    {
        return mask & (1 << I);
    }

    template <class T>
    constexpr bool has_value() const noexcept
    {
        return has_value<detail::index<Types, T>::value>();
    }

    template <size_t I>
    void set_mask()
    {
        mask |= (1 << I);
    }

    /// @pre has_value<I>
    template <size_t I>
    JASEL_CXX14_CONSTEXPR typename tuple_element<I, Types>::type& get_ref()
    {
        return std::get<I>(tpl);
    }
    template <size_t I>
    constexpr typename tuple_element<I, Types>::type const& get_ref() const
    {
        return std::get<I>(tpl);
    }
    /// @pre has_value<T>
    template <class T>
    constexpr T & get_ref()
    {
        return get_ref<detail::index<Types, T>::value>();
    }
    template <class T>
    constexpr T const& get_ref() const
    {
        return get_ref<detail::index<Types, T>::value>();
    }

    template <size_t I>
    JASEL_CXX14_CONSTEXPR auto get_if() noexcept -> decltype(&get_ref<I>())
    {
      if (has_value<I>())
        return &get_ref<I>();
      return nullptr;
    }
    template <size_t I>
    JASEL_CXX14_CONSTEXPR auto get_if() const noexcept -> decltype(&get_ref<I>())
    {
      if (has_value<I>())
        return &get_ref<I>();
      return nullptr;
    }
    template <class T>
    JASEL_CXX14_CONSTEXPR T* get_if() noexcept
    {
      return get_if<detail::index<Types, T>::value>();
    }
    template <class T>
    constexpr T const* get_if() const noexcept
    {
      return get_if<detail::index<Types, T>::value>();
    }

    template <size_t I>
    JASEL_CXX14_CONSTEXPR optional_ref<typename tuple_element<I, Types>::type> get_opt() noexcept
    {
        using T = typename tuple_element<I, Types>::type;
        return this->detail::optional_ref_fact<T>::make(I, mask, get_ref<I>());
    }
    template <size_t I>
    constexpr optional_ref<const typename tuple_element<I, Types>::type> get_opt() const noexcept
    {
        using T = typename tuple_element<I, Types>::type;
        return this->detail::optional_ref_fact<T>::make(I, mask, get_ref<I>());
    }

    template <class T>
    JASEL_CXX14_CONSTEXPR optional_ref<T> get_opt() noexcept
    {        return get_opt<detail::index<Types, T>::value>();    }
    template <class T>
    constexpr optional_ref<const T> get_opt() const noexcept
    {        return get_opt<detail::index<Types, T>::value>();    }

    template <class T>
    optionals& operator=(T const& v)
    {
        this->template get_opt<T>() = v;
        return *this;
    }

    template <class T>
    void reset()
    {
        this->template get<T>().reset();
    }

private:
    size_t mask;
    Types tpl;
};

template <size_t I, class ...Ts>
constexpr auto get(optionals<Ts...>& opts) -> decltype(opts.template get_opt<I>())
{
    return opts.template get_opt<I>();
}
template <size_t I, class ...Ts>
constexpr auto get(optionals<Ts...> const& opts) -> decltype(opts.template get_opt<I>())
{
    return opts.template get_opt<I>();
}

template <class T, class ...Ts>
constexpr auto get(optionals<Ts...>& opts) -> decltype(opts.template get_opt<T>())
{
    return opts.template get_opt<T>();
}
template <class T, class ...Ts>
constexpr auto get(optionals<Ts...> const& opts) -> decltype(opts.template get_opt<T>())
{
    return opts.template get_opt<T>();
}

template <class ...Ts>
void swap(optionals<Ts...>& x, optionals<Ts...>& y)
{
    return std::swap(x, y);
}

}} // experimental/fundamentals_v3/
} // std
#endif // header
