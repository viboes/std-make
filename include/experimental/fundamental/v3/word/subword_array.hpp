//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2017.
// Distributed under the Boost
// Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or
// copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Based on the <array> interface
//
//////////////////////////////////////////////////////////////////////////////

#error "Not ready yet"

/*!
\file
\brief
The header \c <experimantal/compact_array.hpp> defines a class template for storing
sequences of objects fixed by the number of significant bits of the mapped type.
We can say that
\c compact_array is a generalization of std::bitset and \c std::array
To be used when there are space constraints and the the difference between the number of bits of the representation
and  number of significant bits is enough to use only a compact form with the significant bits.
*/

#ifndef JASEL_FUNDAMENTAL_V3_BITS_COMPACT_ARRAY_HPP
#define JASEL_FUNDAMENTAL_V3_BITS_COMPACT_ARRAY_HPP

#include <experimental/swappable.hpp>
#include <experimental/bit.hpp>
#include <experimental/subword.hpp>
#include <cassert>
#include <iterator>
#include <algorithm>
#include <stdexcept>

namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{

  template<class T, class WordType>
  class nbits_reference
  {
    static_assert(binary_digits<WordType>::value, "");

  public:
    using word_type = WordType;
    using value_type = T;
    using index_type = size_t;

    constexpr reference() noexcept = delete;
    constexpr reference(reference const&) noexcept = default;
    reference& operator=(reference const&) noexcept = default;
    ~reference() noexcept = default;

    constexpr reference(word_type& ref, index_type pos) noexcept
      : ptr_(&ref), pos_(pos)
    { }

    //! assignment from T
    JASEL_MUTABLE_CONSTEXPR reference& operator=(T const& x) noexcept
    {
      ptr_->set_value(pos_, x);
      return *this;
    }

    //! implicit conversion to T
    constexpr operator T() const noexcept
    {
      return ptr_->get_value(pos_);
    }

    constexpr nbits_pointer<T, WordType> operator&() const noexcept;

    void swap(nbits_reference other);
    void swap(T& other);

    constexpr word_type* address() const noexcept;
    constexpr size_type position() const noexcept;
    constexpr word_type mask1() const noexcept;
    constexpr word_type mask2() const noexcept;

  private:
    word_type* ptr_;
    index_type pos_;
  };

    /**
    An `compact_array` supports random access iterators. An instance of `compact_array<T, N, B>`
    stores `N` elements of a trivial type `T` using the significant bits of `T` defined by binary_digits_v<T>.

    The elements of an `compact_array` are stored contiguously, but at the bit level, not at the address level,
    meaning that if `a` is a `compact_array<T, N, B>`, then it doesn' obeys the identity

     \code
     &a[n] == &a[0] + n for all 0 <= n <  N().
     \endcode

    An `compact_array` is not an aggregate but can be initialized using the syntax

     \code
      compact_array<T, N, B> a = { initializer-list };
     \endcode

    where \e initializer-list is a comma separated list of up to `N`
    elements of type `U` convertible to `T`.

    An `compact_array` satisfies some of the requirements of a container and of a
    reversible container, except
    - that a default constructed array object is not empty and that swap does not have constant complexity.

    An `compact_array` satisfies some of the requirements of a sequence container. Descriptions are provided
    here only for operations on `compact_array` that are not described in one of these tables and for operations
    where there is additional semantic information.

    The conditions for an aggregate are met. Class `compact_array` relies on
    the implicitly-declared special member functions to conform to the
    container requirements

    @par Note:
    The member variable `elems` is shown for exposition only, to emphasize
    that `compact_array` is a class aggregate. The name elems is not part of
    compact_array's interface.

    @tparam T array's element type
    @tparam N array's numlber of elements
  */

    template<class T, size_t N, class Block=uint64_t>
    class compact_array {
    private:
        constexpr unsigned element_digits = binary_digits<T>::value;
        constexpr unsigned block_digits = sizeof(Block);
        static_assert(is_integral<Block>::value, "Block must be integral");
        static_assert(is_same<Block, bool>::value, "Block can not be bool");
        static_assert(is_signed<Block>::value, "Block must be signed");
        static_assert(element_digits<= sizeof(Block)), "The number of significant bits of T must be LT the size of the Block");

        constexpr size_type num_of_blocks = (element_digits*N) / block_digits;
        constexpr index_type period = static_mcm<element_digits, 2*block_digits>/element_digits;

        Block blocks[num_of_blocks];   //! storage

    public:

        //! proxy class representing a reference to an compressed element
        //!
        //! It contains an assignment operator, a conversion to @c T,
        //! It exists only as a helper class for @c compact_array's @c operator[].
        class reference {
          using compact_array_type = compact_array<T, N, Block>;
          friend class compact_array<T, N, Block>;
          compact_array_type* ref_;
          T pos_;
          constexpr reference(compact_array_type& ref, T pos) noexcept
            : ref_(&ref), pos_(pos)
          { }
        public:
          constexpr reference() noexcept = delete;
          constexpr reference(reference const&) noexcept = default;
          reference& operator=(reference const&) noexcept = default;
          ~reference() noexcept = default;

          //! assignment from T
          JASEL_MUTABLE_CONSTEXPR reference& operator=(T const& x) noexcept
          {
            ref_->set_value(pos_, x);
            return *this;
          }

          //! implicit conversion to T
          constexpr operator T() const noexcept
          {
            return T(ref_->get_value(pos_));
          }

          // fixme do we want reference to behave like a bitmask<bitsof<T>, Block>?
        };
        //! proxy class representing a const reference to an compressed element
        //!
        //! It contains an assignment operator, a conversion to @c T,
        //! It exists only as a helper class for @c compact_array's @c operator[].
        class const_reference {
          using compact_array_type = compact_array<T, N, Block>;
          friend class compact_array<T, N, Block>;
          compact_array_type const* ref_;
          T pos_;
          constexpr reference(compact_array_type const& ref, T pos) noexcept
            : ref_(&ref), pos_(pos)
          { }
        public:
          constexpr reference() noexcept = delete;
          constexpr reference(reference const&) noexcept = default;
          reference& operator=(reference const&) noexcept = default;
          ~reference() noexcept = default;

          //! implicit conversion to T
          constexpr operator T() const noexcept
          {
            return T(ref_->get_value(pos_));
          }
        };
        // type definitions
        typedef T                 value_type;

        //typedef value_type*       pointer;
        //typedef const value_type* const_pointer;
        typedef int               key_type;
        typedef std::size_t       size_type;
        typedef std::size_t       index_type;
        typedef std::ptrdiff_t    difference_type;
        static const std::size_t  static_size = N;

        JASEL_MUTABLE_CONSTEXPR iterator        elems_ptr(key_type k) noexcept { return iterator(this, k); }
        JASEL_MUTABLE_CONSTEXPR const_iterator  elems_ptr(key_type k) const noexcept { return const_iterator(this, k); }

        JASEL_MUTABLE_CONSTEXPR reference        elems_ref(key_type k) noexcept { return reference(this, k); }
        JASEL_MUTABLE_CONSTEXPR const_reference  elems_ref(key_type k) const noexcept { return const_reference(this, k); }


        // iterator support
        //! \n<b>Returns:</b> iterator for the first element
        //! \n<b>Throws:</b> Nothing
        JASEL_MUTABLE_CONSTEXPR iterator        begin() noexcept     { return elems_ptr(0); }

        //! \n<b>Returns:</b> const iterator for the first element
        constexpr const_iterator  begin() const noexcept { return elems_ptr(0); }
        constexpr const_iterator cbegin() const noexcept { return elems_ptr(0); }

        //! \n<b>Returns:</b> iterator for position after the last element
        JASEL_MUTABLE_CONSTEXPR iterator        end() noexcept      { return elems_ptr(N); }

        //! \n<b>Returns:</b> const iterator for position after the last element
        constexpr const_iterator  end() const noexcept { return elems_ptr(N); }
        constexpr const_iterator cend() const noexcept { return elems_ptr(N); }

        // reverse iterator support
        typedef std::reverse_iterator<iterator> reverse_iterator;
        typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

        //! \n<b>Returns:</b> reverse iterator for the first element of reverse iteration
        JASEL_MUTABLE_CONSTEXPR reverse_iterator rbegin() noexcept {
          return reverse_iterator(end());
        }
        constexpr const_reverse_iterator rbegin() const noexcept {
            return const_reverse_iterator(end());
        }
        constexpr const_reverse_iterator crbegin() const noexcept {
            return const_reverse_iterator(end());
        }

        //! \n<b>Returns:</b> reverse iterator for position after the last element in reverse iteration
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
        //! \n<b>Pre-condition:</b> `k < static_size`
        //! \n<b>Returns:</b> reference to the element with key `k`
        //! \n<b>Throws:</b> Nothing.
        JASEL_MUTABLE_CONSTEXPR reference operator[](key_type k)
        {
            assert( k < static_size && "out of range" );
            return elems_ref(k);
        }

        //! \n<b>Pre-condition:</b> <tt>k < static_size</tt>
        //! \n<b>Returns:</b> constant reference to the element with key k
        //! \n<b>Throws:</b> Nothing.
        JASEL_CXX14_CONSTEXPR const_reference operator[](key_type k) const
        {
            assert( k < static_size && "out of range" );
            return elems_ref(k);
        }


        // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|
        // |xxxxxxxx|xx000000|00000000|00000000|00000000|00000000|00000000|00000000|
        // |00000000|00xxxxxx|xxxx0000|00000000|00000000|00000000|00000000|00000000|
        // |00000000|00000000|0000xxxx|xxxxxx00|00000000|00000000|00000000|00000000|
        // |00000000|00000000|00000000|000000xx|xxxxxxxx|00000000|00000000|00000000|
        // |00000000|00000000|00000000|00000000|00000000|xxxxxxxx|xx000000|00000000|
        // |00000000|00000000|00000000|00000000|00000000|00000000|00xxxxxx|xxxx0000|

        // |00000000|00000000|00000000|00000000|00000000|00000000|00000000|0000xxxx|
        // |xxxxxx00|00000000|00000000|00000000|00000000|00000000|00000000|00000000|
        // |000000xx|xxxxxxxx|00000000|00000000|00000000|00000000|00000000|00000000|
        // |00000000|00000000|xxxxxxxx|xx000000|00000000|00000000|00000000|00000000|
        // |00000000|00000000|00000000|00xxxxxx|xxxx0000|00000000|00000000|00000000|
        // |00000000|00000000|00000000|00000000|0000xxxx|xxxxxx00|00000000|00000000|
        // |00000000|00000000|00000000|00000000|00000000|000000xx|xxxxxxxx|00000000|

        // |00000000|00000000|00000000|00000000|00000000|00000000|00000000|xxxxxxxx|
        // |xx000000|00000000|00000000|00000000|00000000|00000000|00000000|00000000|
        // |00xxxxxx|xxxx0000|00000000|00000000|00000000|00000000|00000000|00000000|
        // |00000000|0000xxxx|xxxxxx00|00000000|00000000|00000000|00000000|00000000|
        // |00000000|00000000|000000xx|xxxxxxxx|00000000|00000000|00000000|00000000|
        // |00000000|00000000|00000000|00000000|xxxxxxxx|xx000000|00000000|00000000|
        // |00000000|00000000|00000000|00000000|00000000|00xxxxxx|xxxx0000|00000000|
        // |00000000|00000000|00000000|00000000|00000000|00000000|0000xxxx|xxxxxx00|

        // |00000000|00000000|00000000|00000000|00000000|00000000|00000000|000000xx|
        // |xxxxxxxx|00000000|00000000|00000000|00000000|00000000|00000000|00000000|
        // |00000000|xxxxxxxx|xx000000|00000000|00000000|00000000|00000000|00000000|
        // |00000000|00000000|00xxxxxx|xxxx0000|00000000|00000000|00000000|00000000|
        // |00000000|00000000|00000000|0000xxxx|xxxxxx00|00000000|00000000|00000000|
        // |00000000|00000000|00000000|00000000|000000xx|xxxxxxxx|00000000|00000000|
        // |00000000|00000000|00000000|00000000|00000000|00000000|xxxxxxxx|xx000000|

        // |00000000|00000000|00000000|00000000|00000000|00000000|00000000|00xxxxxx|
        // |xxxx0000|00000000|00000000|00000000|00000000|00000000|00000000|00000000|
        // |0000xxxx|xxxxxx00|00000000|00000000|00000000|00000000|00000000|00000000|
        // |00000000|000000xx|xxxxxxxx|00000000|00000000|00000000|00000000|00000000|
        // |00000000|00000000|00000000|xxxxxxxx|xx000000|00000000|00000000|00000000|
        // |00000000|00000000|00000000|00000000|00xxxxxx|xxxx0000|00000000|00000000|
        // |00000000|00000000|00000000|00000000|00000000|0000xxxx|xxxxxx00|00000000|
        // |00000000|00000000|00000000|00000000|00000000|00000000|000000xx|xxxxxxxx|

        class indexes_masks {
          constexpr unsigned element_digits = binary_digits<T>::value;
          constexpr unsigned block_digits = sizeof(Block);
          constexpr index_type period = static_mcm<element_digits, 2*block_digits>::value/element_digits;

          class cache_type {
          public:
            // @par Pre-condition k < cache_size
            cache_type(key_type k)
              : m_first_leading_zeros(element_digits*k - (element_digits*k)/block_digits*block_digits)
            {
              //00 - 00= 00- 00 - 8 2
              //01 - 10= 10- 00 - 0 6 4
              //02 - 20= 20- 00 - 0 0 4 6
              //03 - 30= 30- 00 + 0 0 0 2 | 8
              //04 - 08= 40- 32 - 0 8 2
              //05 - 18= 50- 32 - 0 0 6 4
              //06 - 28= 60- 32 + 0 0 0 4 | 6
              //07 - 06= 70- 64 - 2 8
              //08 - 16= 80- 64 - 0 0 8 2
              //09 - 26= 90- 64 + 0 0 0 6 | 4
              //10 - 04=100- 96 - 4 6
              //11 - 14=110- 96 - 0 2 8
              //12 - 24=120- 96 + 0 0 0 8 | 2
              //13 - 02=130-128 - 6 4
            }

            index_type first_leading_zeros() const { return m_first_leading_zeros(); }
            index_type first_last_one() const { return first_leading_zeros() + element_digits -1; }
            bool only_one() const { return first_last_one > block_digits; }
            index_type first_trailing_ones() const { return block_digits - first_last_one(); }
            Block mask() const { return bit_ops::up_to<element_digits, Block>(); }
            Block first_mask() const { return mask() << first_leading_zeros(); }
            Block second_mask() const { return mask() >> first_trailing_ones(); }
          private:
            index_type m_first_leading_zeros;
          };

        public:
          indexes_masks(key_type k)
          {
            assert( k < static_size && "out of range" );

            const index_type bits = (element_digits*(k+1));
            m_first_index = bits / block_digits;

            const index_type cache_index = k % period;
            m_cache = cache_type(cache_index);
          }

          index_type first_index() const { return m_first_index; }
          //index_type second_index() const  { return m_first_index + only_one()?0:1; }
          index_type second_index() const  { return m_first_index + 1; }

          index_type first_leading_zeros() const { return m_cache->first_leading_zeros(); }
          index_type first_last_one() const { return m_cache->first_last_one(); }
          bool only_one() const { return only_one() ; }
          index_type first_trailing_ones() const { return m_cache->first_trailing_ones(); }
          Block first_mask() const { return m_cache->first_mask(); }
          Block second_mask() const { return m_cache->second_mask(); }

        private:
          index_type m_first_index;
          cache_type m_cache;
        };

        // set
        //! \n<b>Pre-condition:</b> `k < static_size`
        //! \n<b>Returns:</b> reference to the element with key `k`
        //! \n<b>Throws:</b> Nothing.
        JASEL_MUTABLE_CONSTEXPR void set_value(key_type k, value_type value)
        {
            const indexes_masks ctx(k);

            blocks[ctx.first_index()] |= (value >> ctx.first_leading_zeros()) & ~ctx.first_mask();
            if ( ctx.only_one() ) return;
            blocks[ctx.second_index()] |= (value << ctx.first_trailing_ones() ) & ~ctx.second_mask();
        }

        // get()
        //! \n<b>Pre-condition:</b> `k < static_size`
        //! \n<b>Returns:</b> element with key k
        JASEL_MUTABLE_CONSTEXPR value_type get_value(key_type k) const
        {
          const indexes_masks ctx(k);

          const Block first = (blocks[ctx.first_index()] & ctx.first_mask()) << ctx.first_leading_zeros() ;
          if ( ctx.only_one() ) return first;

          const Block second = (blocks[ctx.second_index()] & ctx.second_mask()) >> ctx.first_trailing_ones() ;
          return first | second;
        }

        // at() with range check
        //! \n<b>Returns:</b> reference element with key k
        //! \n<b>Throws:</b> std::out_of_range if i >= static_size
        JASEL_MUTABLE_CONSTEXPR reference at(key_type k)
        {
          return elems_ref(rangecheck(k));
        }
        constexpr const_reference at(key_type k) const
        {
          return elems_ref(rangecheck(k));
        }

        // front() and back()
        //! \n<b>Returns:</b> reference to the first element
        //! \n<b>Throws:</b> Nothing
        JASEL_MUTABLE_CONSTEXPR reference front() noexcept
        {
          return elems_ref(0);
        }

        //! \n<b>Returns:</b> const reference to the first element
        //! \n<b>Throws:</b> Nothing
        constexpr const_reference front() const noexcept
        {
          return elems_ref(0);
        }

        //! \n<b>Returns:</b> reference to the last element
        //! \n<b>Throws:</b> Nothing
        JASEL_MUTABLE_CONSTEXPR reference back() noexcept
        {
          return elems_ref(static_size-1);
        }

        //! \n<b>Returns:</b> const reference to the last element
        //! \n<b>Throws:</b> Nothing
        constexpr const_reference back() const noexcept
        {
            return elems_ref(static_size-1);
        }

        // size is constant
        //! \n<b>Returns:</b> N.
        constexpr size_type size() const noexcept
        {
          return static_size;
        }
        //! \n<b>Returns:</b> false
        //! \n<b>Throws:</b> Nothing
        constexpr  bool empty() const noexcept
        {
          return false;
        }

        //! \n<b>Returns:</b> N.
        constexpr size_type max_size() const noexcept
        {
          return static_size;
        }


        /*! \n<b>Effects:</b> <tt>swap_ranges(begin(), end(), y.begin())</tt>
            \n<b>Throws:</b> Nothing unless one of the element-wise swap calls throws an exception.
            \n<b>Note:</b> Unlike the @c swap function for other containers, @c compact_array::swap
            takes linear time, may exit via an exception, and does not cause
            iterators to become associated with the other container.
         */
        void swap (compact_array& y) // noexcept(std::is_nothrow_swappable_v<T>)
        {
            for (size_type i = 0; i < num_of_blocks; ++i)
            {
              std::swap(blocks[i],y.blocks[i]);
            }
        }

        //! direct access to block (read-only)
        //! \n<b>Returns:</b> blocks.
        JASEL_CXX14_CONSTEXPR const Block* block_begin() const {
          return blocks;
        }
        //! \n<b>Returns:</b> blocks.
        JASEL_CXX14_CONSTEXPR Block* block_begin() {
          return blocks;
        }
        JASEL_CXX14_CONSTEXPR const Block* block_end() const {
          return blocks+num_of_blocks;
        }
        JASEL_CXX14_CONSTEXPR Block* block_end() {
          return blocks+num_of_blocks;
        }

        //! assignment with type conversion
        template <typename T2>
        compact_array& operator= (const compact_array<T2, N, Block>& rhs) {
            std::copy(rhs.begin(),rhs.end(), begin());
            return *this;
        }

        //! A synonym for fill - Assigns one value to all elements
        //! \n<b>Effects:</b> <tt>std::fill_n(begin(), static_size, value)</tt>
        void assign (const T& value) {
          fill ( value );
        }

        //! \n<b>Effects:</b> <tt>fill_n(begin(), static_size, u)</tt>
        void fill   (const T& value)
        {
            std::fill_n(begin(),size(),value);
        }

        //! check range (may be private because it is static)
        static size_type rangecheck (key_type k) {
            if (k >= static_size) {
                std::out_of_range e("compact_array<>: index out of range");
                throw (e);
            }
            return k;
        }
    };

    // comparisons
    //! \n<b>Returns:</b> <tt>std::equal(x.begin(), x.end(), y.begin())</tt>
    template<class T, size_t N, class B>
    bool operator== (const compact_array<T, N, B>& x, const compact_array<T, N, B>& y)
    {
        return std::equal(x.block_begin(), x.block_end(), y.block_begin());
    }

    //! \n<b>Returns:</b> <tt>std::lexicographical_compare(x.begin(), x.end(), y.begin(), y.end())</tt>
    template<class T, size_t N, class B>
    bool operator< (const compact_array<T, N, B>& x, const compact_array<T, N, B>& y)
    {
        return std::lexicographical_compare(x.begin(),x.end(),y.begin(),y.end());
    }

    //! \n<b>Returns:</b> <tt>!(x == y)</tt>
    template<class T, size_t N, class B>
    bool operator!= (const compact_array<T, N, B>& x, const compact_array<T, N, B>& y)
    {
        return !(x==y);
    }

    //! \n<b>Returns:</b> <tt>y < x</tt>
    template<class T, size_t N, class B>
    bool operator> (const compact_array<T, N, B>& x, const compact_array<T, N, B>& y)
    {
        return y<x;
    }
    //! \n<b>Returns:</b> <tt>!(y<x)</tt>
    template<class T, size_t N, class B>
    bool operator<= (const compact_array<T, N, B>& x, const compact_array<T, N, B>& y)
    {
        return !(y<x);
    }
    //! \n<b>Returns:</b> <tt>!(x<y)</tt>
    template<class T, size_t N, class B>
    bool operator>= (const compact_array<T, N, B>& x, const compact_array<T, N, B>& y)
    {
        return !(x<y);
    }

    //! compact_array's swap

    //! \n<b>Effects:</b> As
    //! \code
    //!  x.swap(y);
    //! \endcode
    //! \n<b>Complexity:</b> linear in \c N.

    template<class T, size_t N, class B>
    inline void swap (compact_array<T, N, B>& x, compact_array<T, N, B>& y)
    {
        x.swap(y);
    }

}}

// todo add specialization for tuple-like type
#if 0

    template <class T, size_t N, class B>
    class  tuple_size<compact_array<T, N, B> >
        : public integral_constant<size_t, N>
          {};


    template <class T, size_t N, class B, size_t K>
    class  tuple_element<compact_array<T, N, B>, K >
    {
    public:
        typedef T type;
    };

    template <class T, size_t N, class B, size_t K>
    inline    T&    get(compact_array<T, N, B>& a)
    {
        return a[K];
    }

    template <class T, size_t N, class B, size_t K>
    inline    T&    get(compact_array<T, N, B> const& a)
    {
        return a[K];
    }

    template <class T, size_t N, class B, size_t K>
    inline    T&&    get(compact_array<T, N, B>&& a)
    {
        return a[K];
    }

#endif
}


#endif // header
