// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Based on https://github.com/akrzemi1/explicit/blob/master/include/ak_toolkit/not_null.hpp
// See https://github.com/akrzemi1/explicit/blob/master/doc/not_null.md
// Note this not_null is different from gsl::not_null.
// Copyright (C) 2016 Andrzej Krzemienski.
// Copyright (C) 2017 Vicente J. Botet Escriba

#ifndef JASEL_FUNDAMENTAL_V3_UTILITY_NOT_NULL_HPP
#define JASEL_FUNDAMENTAL_V3_UTILITY_NOT_NULL_HPP

#include <experimental/fundamental/v2/config.hpp>
#include <experimental/contract.hpp>

#include <functional>
#include <utility>

namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{

// fixme: with which other pointer types this should work? What are the constraints?
    template <typename Ptr>
    class not_null
    {
        Ptr _ptr;
    public:
        static_assert(is_assignable<Ptr&, nullptr_t>::value, "T cannot be assigned nullptr.");

        // todo add conversion from convertible to Ptr
        explicit not_null(Ptr p) : _ptr(move(p))
        {
            JASEL_EXPECTS(_ptr != nullptr);
        }
        not_null(not_null const& other) = delete;
        not_null(not_null && other)
        : _ptr(move(other._ptr))
        {
        }

        // todo add conversion from not_null<U>
        explicit not_null(nullptr_t) = delete;

        // conversions
        // fixme: do we want an implicit conversion to T*
        //    having it has IMO more trouble that benefit. We need to delete a lot of induced overloads as done in GSL.
        operator Ptr() const noexcept
        {   return get();}

        auto get() const JASEL_DECLTYPE_RETURN_NOEXCEPT(&*_ptr)

        auto operator*() const JASEL_DECLTYPE_RETURN_NOEXCEPT(*_ptr)
        //auto operator*()  JASEL_DECLTYPE_RETURN_NOEXCEPT(*_ptr)
        auto operator->() const JASEL_DECLTYPE_RETURN_NOEXCEPT(::std::addressof(*_ptr))
        //auto operator->() JASEL_DECLTYPE_RETURN_NOEXCEPT(::std::addressof(*_ptr))
    };

    // fixme: make use of strong types and mixins
    //    stronng type
    //    comparable
    template <typename T>
    class not_null<T*>
    {
        T* _ptr;
    public:
        explicit JASEL_CXX14_CONSTEXPR not_null(T* p) : _ptr(p)
        {
            JASEL_EXPECTS(p != nullptr);
        }
//      constexpr not_null(T& r) noexcept // NOLINT google-explicit-constructor
//              : _ptr(&r) {}
        not_null(nullptr_t) = delete;
        constexpr not_null(not_null const& ) = default;
        constexpr not_null(not_null && ) = default;
        template <class U, class Enabler = enable_if_t<std::is_convertible<U, T>::value>>
        constexpr not_null(not_null<U> const& other)
        : _ptr( other.get() )
        {

        }

        // fixme, if the construction is explicit, wouldn't we need to make assignment explicit?
        //    nnptr = as_not_null(p);
//      not_null& operator=(T* p)
//      {
//          JASEL_EXPECTS(p != nullptr);
//          _ptr = p ;
//          return this*;
//      }
        not_null& operator=(not_null const& ) = default;
        template <class U, class Enabler = enable_if_t<std::is_convertible<U, T>::value>>
        not_null& operator=(not_null<U> const& other)
        {
            _ptr = other.get();
            return *this;
        }

        // conversions
        // fixme: do we want an implicit conversion to T*
        //    having it has IMO more trouble that benefit. We need to delete a lot of induced overloads as done in GSL.
        operator T*() const noexcept
        {   return get();}

        // We don't want to be able to update the pointer from outside
        //T* const& underlying() const { return _ptr; }

        auto get() const JASEL_DECLTYPE_RETURN_NOEXCEPT(_ptr)

        auto operator*() const JASEL_DECLTYPE_RETURN_NOEXCEPT(*get())
        auto operator->() const JASEL_DECLTYPE_RETURN_NOEXCEPT(get())

        // fixme: Do we want to be able to compare with other not_null pointers
        // fixme: Do we want to be able to compare with other pointers
        friend bool operator==(not_null const& x, not_null const& y)
        {
            return x.get() == y.get();
        }
        friend bool operator!=(not_null const& x, not_null const& y)
        {
            return x.get() != y.get();
        }
        friend bool operator<(not_null const& x, not_null const& y)
        {
            return x.get() < y.get();
        }
        friend bool operator<=(not_null const& x, not_null const& y)
        {
            return x.get() <= y.get();
        }
        friend bool operator>(not_null const& x, not_null const& y)
        {
            return x.get() > y.get();
        }
        friend bool operator>=(not_null const& x, not_null const& y)
        {
            return x.get() >= y.get();
        }
    };

    template <typename Ptr>
    not_null<Ptr> as_not_null(Ptr p)
    {
        return not_null<Ptr>
        {   move(p)};
    }
//  template <typename T>
//  not_null<T*> as_not_null(T& r) noexcept
//  {
//      return not_null<T*>{r};
//  }

    void as_not_null(nullptr_t) = delete;

    template <class OSTREAM, class T>
    OSTREAM& operator<<(OSTREAM& os, const not_null<T>& val)
    {
        os << val.get();
        return os;
    }

}
}

template <class T>
struct hash<experimental::not_null<T>>
{
std::size_t operator()(const experimental::not_null<T>& value) const
{
    return hash<T> {}(value);
}
};
}

#endif // header
