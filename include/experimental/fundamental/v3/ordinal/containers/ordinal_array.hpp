//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2011,2015-2016.
// Distributed under the Boost
// Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or
// copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Based on the boost/array.hpp implementation
//
//////////////////////////////////////////////////////////////////////////////

/*!
\file
\brief
The header \c <ordinal/containers/ordinal_array.hpp> defines a class template for storing
sequences of objects fixed by the size of the enumeration.
We can say that
\c ordinal_array is the counterpart of \c std::array when the index are ordinal types.
*/

#ifndef JASEL_FUNDAMENTAL_V3_ORDINAL_ORDINAL_ARRAY_HPP
#define JASEL_FUNDAMENTAL_V3_ORDINAL_ORDINAL_ARRAY_HPP

#include <experimental/ordinal.hpp>
#include <experimental/swappable.hpp>
#include <cassert>
#include <iterator>
#include <algorithm>

namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{
    /** @file
       \brief
        The header \c <experimental/ordinal_array.hpp> defines a class template \c ordinal_array for viewing an enumerations as a range.


    An \c ordinal_array supports random access iterators. An instance of \c ordinal_array<T, O>
    stores as many elements of type \c T as literals are on the ordinal \c O,
    so that <tt>size() == meta::size<O>::value</tt> is an invariant.

    The elements of an \c ordinal_array are stored contiguously, meaning that if \c a is an
    \c ordinal_array<T, O>, then it obeys the identity

     \code
     &a[n] == &a[0] + n for all 0 <= n <  meta::size<O>::value.
     \endcode

    An \c ordinal_array is an aggregate that can (????) be initialized with the syntax

     \code
      ordinal_array<T, O> a = { initializer-list };
     \endcode

    where \e initializer-list is a comma separated list of up to \c meta::size<O>::value
    elements whose types are convertible to \c T.

    An \c ordinal_array satisfies all of the requirements of a container and of a
    reversible container, except that a default constructed array object is not
    empty and that swap does not have constant complexity. An \c ordinal_array satisfies
    some of the requirements of a sequence container. Descriptions are provided
    here only for operations on ordinal_array that are not described in one of these
    tables and for operations where there is additional semantic information.

     The conditions for an aggregate are met. Class ordinal_array relies on
     the implicitly-declared special member functions to conform to the
     container requirements



    @par Note:<br> The member variable elems is shown for exposition only, to emphasize
    that ordinal_array is a class aggregate. The name elems is not part of
    ordinal_array's interface.

    @tparam T array's element type
    @tparam O array's index ordinal
  */

    template<class T, typename O>
    class ordinal_array {
      public:
        //! For exposition only
        T elems[ordinal::meta::size<O>::value];    //! fixed-size array of elements of type T

      public:
        // type definitions
        typedef T                 value_type;
        typedef T*                iterator;
        typedef const T*          const_iterator;
        typedef T&                reference;
        typedef const T&          const_reference;
        typedef value_type*       pointer;
        typedef const value_type* const_pointer;
        typedef O                 key_type;
        typedef std::size_t       size_type;
        typedef std::ptrdiff_t    difference_type;
        static const std::size_t  static_size = ordinal::meta::size<O>::value;
        //ordinal_array() {}

        // iterator support
        //! @par Returns:<br> iterator for the first element
        //! @par Throws:<br> Nothing
        JASEL_MUTABLE_CONSTEXPR iterator        begin() noexcept     { return elems; }

        //! @par Returns:<br> const iterator for the first element
        constexpr const_iterator  begin() const noexcept { return elems; }
        constexpr const_iterator cbegin() const noexcept { return elems; }

        //! @par Returns:<br> iterator for position after the last element
        JASEL_MUTABLE_CONSTEXPR iterator        end() noexcept      { return elems+static_size; }

        //! @par Returns:<br> const iterator for position after the last element
        constexpr const_iterator  end() const noexcept { return elems+static_size; }
        constexpr const_iterator cend() const noexcept { return elems+static_size; }

        // reverse iterator support
        typedef std::reverse_iterator<iterator> reverse_iterator;
        typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

        //! @par Returns:<br> reverse iterator for the first element of reverse iteration
        JASEL_MUTABLE_CONSTEXPR reverse_iterator rbegin() noexcept {
          return reverse_iterator(end());
        }
        constexpr const_reverse_iterator rbegin() const noexcept {
            return const_reverse_iterator(end());
        }
        constexpr const_reverse_iterator crbegin() const noexcept {
            return const_reverse_iterator(end());
        }

        //! @par Returns:<br> reverse iterator for position after the last element in reverse iteration
        JASEL_MUTABLE_CONSTEXPR reverse_iterator rend() noexcept {
          return reverse_iterator(begin());
        }
        constexpr const_reverse_iterator rend() const noexcept {
            return const_reverse_iterator(begin());
        }
        constexpr const_reverse_iterator crend() const noexcept {
            return const_reverse_iterator(begin());
        }

        // operator[]
        //! @par Requires:<br> <tt>k'pos < static_size</tt>
        //! @par Returns:<br> reference to the element with key @c k
        //! @par Throws:<br> Nothing.
        JASEL_MUTABLE_CONSTEXPR reference operator[](key_type k)
        {
            size_type i = ordinal::pos(k);
            assert( i < static_size && "out of range" );
            return elems[i];
        }

        //! @par Requires:<br> <tt>k'pos < static_size</tt>
        //! @par Returns:<br> constant reference to the element with key k
        //! @par Throws:<br> Nothing.
        JASEL_CXX14_CONSTEXPR const_reference operator[](key_type k) const
        {
            assert( ordinal::pos(k) < static_size && "out of range" );
            return elems[ordinal::pos(k)];
        }

        // at() with range check
        //! @par Returns:<br> element with key k
        //! @par Throws:<br> std::range_error if i >= static_size
        JASEL_MUTABLE_CONSTEXPR reference at(key_type k)
        {
          return elems[rangecheck(k)];
        }
        constexpr const_reference at(key_type k) const
        {
          return elems[rangecheck(k)];
        }

        // front() and back()
        //! @par Returns:<br> reference to the first element
        //! @par Throws:<br> Nothing
        JASEL_MUTABLE_CONSTEXPR reference front() noexcept
        {
            return elems[0];
        }

        //! @par Returns:<br> const reference to the first element
        //! @par Throws:<br> Nothing
        constexpr const_reference front() const noexcept
        {
            return elems[0];
        }

        //! @par Returns:<br> reference to the last element
        //! @par Throws:<br> Nothing
        JASEL_MUTABLE_CONSTEXPR reference back() noexcept
        {
            return elems[static_size-1];
        }

        //! @par Returns:<br> const reference to the last element
        //! @par Throws:<br> Nothing
        constexpr const_reference back() const noexcept
        {
            return elems[static_size-1];
        }

        // size is constant
        //! @par Returns:<br> linear in meta::size<O>::value.
        constexpr size_type size() const noexcept
        {
          return static_size;
        }
        //! @par Returns:<br> false
        //! @par Throws:<br> Nothing
        constexpr  bool empty() const noexcept
        {
          return false;
        }

        //! @par Returns:<br> linear in meta::size<O>::value.
        constexpr size_type max_size() const noexcept
        {
          return static_size;
        }


        /*! @par Effects:<br> <tt>swap_ranges(begin(), end(), y.begin())</tt>
            @par Throws:<br> Nothing unless one of the element-wise swap calls throws an exception.
            @par Note:<br> Unlike the @c swap function for other containers, @c ordinal_array::swap
            takes linear time, may exit via an exception, and does not cause
            iterators to become associated with the other container.
         */
        void swap (ordinal_array<T,O>& y) // noexcept(std::is_nothrow_swappable_v<T>)
        {
            for (size_type i = 0; i < static_size; ++i)
              swappable::swap(elems[i],y.elems[i]);
        }

        //! direct access to data (read-only)
        //! @par Returns:<br> elems.
        constexpr const T* data() const {
          return elems;
        }
        //! @par Returns:<br> elems.
        constexpr T* data() {
          return elems;
        }

        //! use @c ordinal_array as C-array (direct read/write access to data)
        //! @par Returns:<br> elems.
        T* c_array() {
          return elems;
        }

        //! assignment with type conversion
        template <typename T2>
        ordinal_array<T,O>& operator= (const ordinal_array<T2,O>& rhs) {
            std::copy(rhs.begin(),rhs.end(), begin());
            return *this;
        }

        //! A synonym for fill - Assigns one value to all elements
        //! @par Effects:<br> <tt>std::fill_n(begin(), static_size, value)</tt>
        void assign (const T& value) {
          fill ( value );
        }

        //! @par Effects:<br> <tt>fill_n(begin(), static_size, u)</tt>
        void fill   (const T& value)
        {
            std::fill_n(begin(),size(),value);
        }

        //! check range (may be private because it is static)
        static size_type rangecheck (key_type k) {
            size_type i = ordinal::pos(k);
            if (i >= static_size) {
                std::out_of_range e("array<>: index out of range");
                throw (e);
            }
            return i;
        }

    };



    // comparisons
    //! @par Returns:<br> <tt>std::equal(x.begin(), x.end(), y.begin())</tt>
    template<class T, typename O>
    bool operator== (const ordinal_array<T,O>& x, const ordinal_array<T,O>& y)
    {
        return std::equal(x.begin(), x.end(), y.begin());
    }

    //! @par Returns:<br> <tt>std::lexicographical_compare(x.begin(), x.end(), y.begin(), y.end())</tt>
    template<class T, typename O>
    bool operator< (const ordinal_array<T,O>& x, const ordinal_array<T,O>& y)
    {
        return std::lexicographical_compare(x.begin(),x.end(),y.begin(),y.end());
    }

    //! @par Returns:<br> <tt>!(x == y)</tt>
    template<class T, typename O>
    bool operator!= (const ordinal_array<T,O>& x, const ordinal_array<T,O>& y)
    {
        return !(x==y);
    }

    //! @par Returns:<br> <tt>y < x</tt>
    template<class T, typename O>
    bool operator> (const ordinal_array<T,O>& x, const ordinal_array<T,O>& y)
    {
        return y<x;
    }
    //! @par Returns:<br> <tt>!(y<x)</tt>
    template<class T, typename O>
    bool operator<= (const ordinal_array<T,O>& x, const ordinal_array<T,O>& y)
    {
        return !(y<x);
    }
    //! @par Returns:<br> <tt>!(x<y)</tt>
    template<class T, typename O>
    bool operator>= (const ordinal_array<T,O>& x, const ordinal_array<T,O>& y)
    {
        return !(x<y);
    }

    //! ordinal_array's swap

    //! @par Effects:<br> As
    //! \code
    //!  x.swap(y);
    //! \endcode
    //! @par Complexity:<br> linear in \c meta::size<O>::value.

    template<class T, typename O>
    inline void swap (ordinal_array<T,O>& x, ordinal_array<T,O>& y)
    {
        x.swap(y);
    }

#if 0

    template <typename T, typename O>
    T(&)[meta::size<O>::value] get_c_array(ordinal_array<T,O>& arg)
    {
        return arg.elems;
    }

    template <typename T, typename O>
    const T(&)[meta::size<O>::value] get_c_array(ordinal_array<T,O> const& arg)
    {
        return arg.elems;
    }
#endif

}}

// todo add specialization for tuple-like type
#if 0

    template <class T, typename O>
    class  tuple_size<ordinal_array<T, O> >
        : public integral_constant<size_t, ordinal::meta::size<O>::value>
          {};


    template <class T, typename O, size_t K>
    class  tuple_element<ordinal_array<T, O>, K >
    {
    public:
        typedef T type;
    };

    template <class T, typename O, size_t K>
    inline    T&    get(ordinal_array<T, O>& a)
    {
        return a[ordinal::meta::val<O, K>::value];
    }

    template <class T, typename O, size_t K>
    inline    T&    get(ordinal_array<T, O> const& a)
    {
        return a[ordinal::meta::val<O, K>::value];
    }

    template <class T, typename O, size_t K>
    inline    T&&    get(ordinal_array<T, O>&& a)
    {
        return a[ordinal::meta::val<O, K>::value];
    }

#endif
}


#endif // header
