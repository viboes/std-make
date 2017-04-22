// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Copyright (C) 2016,2017 Vicente J. Botet Escriba

#ifndef JASEL_FUNDAMENTAL_V3_OPTIONALS_OPTIONALS_HPP
#define JASEL_FUNDAMENTAL_V3_OPTIONALS_OPTIONALS_HPP

#include <tuple>
#include <experimental/optional.hpp>
#include <type_traits>
# include <experimental/fundamental/v2/config.hpp>

// todo
// * make use of uninitialized storage
// * add move semantics
// * add emplace
// * add hash support
// * add SFINAE for requires conditions
// * try to refactor optional_ref<T> and optional_ref<const T>
//
// Open points:
// * Can an optional_ref<I,U> be converted to an optional_ref<I,T>? Under which conditions?
//    T -> const T or
//    U* is convertible to T* and sizeof(T) == sizeof(U) ?
// * Should the type of the mask be a parameter?

namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{

//todo  move this to meta
namespace detail {
  template <class Tpl, class T>
  struct index;
  template <class T, class ...Ts>
  struct index<std::tuple<T, Ts...>, T> { enum {value = 0}; };
  template <class T, class U, class ...Ts>
  struct index<std::tuple<T, Ts...>, U> { enum {value = 1 + index<std::tuple<Ts...>, U>::value}; };
}

namespace detail {

template <size_t I, class T>
struct optional_ref_fact;

}

/**
optional_ref is a proxy to a pseudo optional<T> included in optionals<Ts...>
It behaves like an optional<T> and it is explicitly convertible to optional<T>.
*/

template <size_t Index, class T>
class optional_ref {
public:
    using index_type = size_t;
    static constexpr index_type index = Index;
    using mask_type = size_t;
    using value_type = const T;

    optional_ref(optional_ref const&) = default;
    optional_ref(optional_ref &&) = default;
    optional_ref& operator=(optional_ref const&) = default;
    optional_ref& operator=(optional_ref &&) = default;

    //optional_ref(optional_ref<U> const&); // ???
    //optional_ref(optional_ref<U> &&); // ???
    //optional_ref& operator=(optional_ref<U> const&); // ???
    //optional_ref& operator=(optional_ref<U> &&); // ???

    optional_ref& operator=(nullopt_t) noexcept
    {
      reset();
      return *this;
    }

    template< class U = T >
    optional_ref& operator=(U && v)
    {
        *ref = forward<U>(v);
        set();
        return *this;
    }

    // fixme: should enplace use {args...} instead of T(args..)?
    template <class... Args>
    void emplace(Args&&... args) noexcept
    {
      *ref = T(std::forward<Args>(args)...);
      set();
    }

    template <class U, class... Args>
    void emplace(initializer_list<U> il, Args&&... args) noexcept
    {
      *ref = T(il, std::forward<Args>(args)...);
      set();
    }

    void reset() noexcept
    {
        *mask &= ~(1 << index);
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
            std::swap(mask, rhs.mask);
            std::swap(ref, rhs.ref);
    }
private:

    friend detail::optional_ref_fact<Index, T>;
    constexpr optional_ref(mask_type& m, T& r) noexcept
        : mask(&m), ref(&r) {}

    void set() noexcept
    {
        *mask |= (1 << index);
    }

    mask_type* mask;
    T* ref;
};

template <size_t Index, class T>
class optional_ref<Index, const T> {
public:
    using index_type = size_t;
    static constexpr index_type index = Index;
    using mask_type = size_t;
    using value_type = const T;

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

    explicit JASEL_CXX14_CONSTEXPR operator optional<const T>() const
    {
      if (has_value())
        return *get();
      return nullopt;
    }

    friend constexpr bool operator == (const optional_ref & x, const optional_ref & y) noexcept
    { return optional<const T>(x) == optional<const T>(y); }
    friend constexpr bool operator != (const optional_ref & x, const optional_ref & y) noexcept
    { return optional<const T>(x) != optional<const T>(y); }
    friend constexpr bool operator < (const optional_ref & x, const optional_ref & y) noexcept
    { return optional<const T>(x) < optional<const T>(y); }
    friend constexpr bool operator <= (const optional_ref & x, const optional_ref & y) noexcept
    { return optional<const T>(x) <= optional<const T>(y); }
    friend constexpr bool operator > (const optional_ref & x, const optional_ref & y) noexcept
    { return optional<const T>(x) > optional<const T>(y); }
    friend constexpr bool operator >= (const optional_ref & x, const optional_ref & y) noexcept
    { return optional<const T>(x) >= optional<const T>(y); }

    friend constexpr bool operator == (const optional_ref & x, nullopt_t y) noexcept
    { return optional<const T>(x) == y; }
    friend constexpr bool operator != (const optional_ref & x, nullopt_t y) noexcept
    { return optional<const T>(x) != y; }
    friend constexpr bool operator < (const optional_ref & x, nullopt_t y) noexcept
    { return optional<const T>(x) < y; }
    friend constexpr bool operator <= (const optional_ref & x, nullopt_t y) noexcept
    { return optional<const T>(x) <= y; }
    friend constexpr bool operator > (const optional_ref & x, nullopt_t y) noexcept
    { return optional<const T>(x) > y; }
    friend constexpr bool operator >= (const optional_ref & x, nullopt_t y) noexcept
    { return optional<const T>(x) >= y; }

    friend constexpr bool operator == (nullopt_t x, const optional_ref & y) noexcept
    { return x == optional<const T>(y); }
    friend constexpr bool operator != (nullopt_t x, const optional_ref & y) noexcept
    { return x != optional<const T>(y); }
    friend constexpr bool operator < (nullopt_t x, const optional_ref & y) noexcept
    { return x < optional<const T>(y); }
    friend constexpr bool operator <= (nullopt_t x, const optional_ref & y) noexcept
    { return x <= optional<const T>(y); }
    friend constexpr bool operator > (nullopt_t x, const optional_ref & y) noexcept
    { return x > optional<const T>(y); }
    friend constexpr bool operator >= (nullopt_t x, const optional_ref & y) noexcept
    { return x >= optional<const T>(y); }

    friend constexpr bool operator == (const optional_ref & x, const T&  y) noexcept
    { return optional<const T>(x) == y; }
    friend constexpr bool operator != (const optional_ref & x, const T&  y) noexcept
    { return optional<const T>(x) != y; }
    friend constexpr bool operator < (const optional_ref & x, const T&  y) noexcept
    { return optional<const T>(x) < y; }
    friend constexpr bool operator <= (const optional_ref & x, const T&  y) noexcept
    { return optional<const T>(x) <= y; }
    friend constexpr bool operator > (const optional_ref & x, const T&  y) noexcept
    { return optional<const T>(x) > y; }
    friend constexpr bool operator >= (const optional_ref & x, const T&  y) noexcept
    { return optional<const T>(x) >= y; }

    friend constexpr bool operator == (const T&  x, const optional_ref & y) noexcept
    { return x == optional<const T>(y); }
    friend constexpr bool operator != (const T&  x, const optional_ref & y) noexcept
    { return x != optional<const T>(y); }
    friend constexpr bool operator < (const T&  x, const optional_ref & y) noexcept
    { return x < optional<const T>(y); }
    friend constexpr bool operator <= (const T&  x, const optional_ref & y) noexcept
    { return x <= optional<const T>(y); }
    friend constexpr bool operator > (const T&  x, const optional_ref & y) noexcept
    { return x > optional<const T>(y); }
    friend constexpr bool operator >= (const T&  x, const optional_ref & y) noexcept
    { return x >= optional<const T>(y); }

    void swap(optional_ref& rhs) noexcept
    {
            std::swap(mask, rhs.mask);
            std::swap(ref, rhs.ref);
    }
private:

    friend detail::optional_ref_fact<Index, T>;
    constexpr optional_ref(mask_type const& m, T const& r) noexcept
        : mask(&m), ref(&r) {}

    mask_type const* mask;
    T const* ref;
};

template <size_t I, class T>
void swap(optional_ref<I,T>& x, optional_ref<I,T>& y)
{
    return x.swap(y);
}
namespace detail {
template <size_t I, class T>
struct optional_ref_fact {
    static constexpr optional_ref<I,T> make(size_t& mask, T& ref) noexcept
    {
        return optional_ref<I, T>(mask, ref);
    };
    static constexpr optional_ref<I, const T> make(size_t const& mask, T const& ref) noexcept
    {
        return optional_ref<I, const T>(mask, ref);
    };
};
}

/**
optionals<Ts..> behaves similarly to tuple<optional<Ts>...>
get<T> returns a optional_ref<T> that behaves as a proxy.
*/
template <class ...Ts>
class optionals
{
    using Types = tuple<Ts...>;
public:
    // This is not POD
    constexpr optionals() noexcept
      : mask(0)  {}
    void construct()  noexcept
    {
      mask = false;
    }
    // generated by default as a POD
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
    JASEL_CXX14_CONSTEXPR optional_ref<I, typename tuple_element<I, Types>::type> get_opt() noexcept
    {
        using T = typename tuple_element<I, Types>::type;
        return detail::optional_ref_fact<I, T>::make(mask, get_ref<I>());
    }
    template <size_t I>
    constexpr optional_ref<I, const typename tuple_element<I, Types>::type> get_opt() const noexcept
    {
        using T = typename tuple_element<I, Types>::type;
        return detail::optional_ref_fact<I, T>::make(mask, get_ref<I>());
    }

    template <class T>
    JASEL_CXX14_CONSTEXPR optional_ref<detail::index<Types, T>::value, T> get_opt() noexcept
    {        return get_opt<detail::index<Types, T>::value>();    }
    template <class T>
    constexpr optional_ref<detail::index<Types, T>::value, const T> get_opt() const noexcept
    {        return get_opt<detail::index<Types, T>::value>();    }

    // fixme: this should work only if T is unique
    template <class T>
    optionals& operator=(T const& v)
    {
        this->template get_opt<T>() = v;
        return *this;
    }
    template <class T>
    void reset()
    {
        this->template get_opt<T>().reset();
    }

    template <size_t I>
    void reset()
    {
        this->template get_opt<I>().reset();
    }

private:
    size_t mask;
    Types tpl;
};

// Optionals should be a model of product_type `optional<Ts>...`. However `get` returns a proxy

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
