//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2011,2016.
// Distributed under the Boost
// Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or
// copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

/*!
 \file
 \brief
 The header \c <experimental/ordinal_range.hpp> defines a class template \c ordinal_range for viewing an ordinal as a range.
 */

#ifndef JASEL_FUNDAMENTAL_V3_ORDINAL_ORDINAL_RANGE_HPP
#define JASEL_FUNDAMENTAL_V3_ORDINAL_ORDINAL_RANGE_HPP

#include <experimental/ordinal.hpp>

#include <boost/iterator/iterator_facade.hpp>
#include <boost/range/iterator_range.hpp>
#include <iosfwd>
#include <iostream>

namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{

    #ifndef JASEL_DOXYGEN_INVOKED
    namespace ordinal_detail {
        // ordinal_iterator is an iterator over an ordinal  that
        // is bounded only by the limits of the ordinal.
        //
        // This is useful for implementing the ordinal_range<E>()
        // function.
        //
        // @par Note:
        // The use of this iterator and ordinal_range<E>() is appreciably less
        // performant than the corresponding hand-written integer
        // loop on many compilers.

        //! ordinal_iterator is a model of RandomAccessIterator
        template<typename T /* , typename Traits=ordinal_range_traits<T> */ >
        class ordinal_iterator
            : public boost::iterator_facade<
                        ordinal_iterator<T>,
                        T,
                        boost::random_access_traversal_tag,
                        T,
                        std::ptrdiff_t
                    >
        {
            typedef boost::iterator_facade<
                        ordinal_iterator<T>,
                        T,
                        boost::random_access_traversal_tag,
                        T,
                        std::ptrdiff_t
                    > base_t;
        public:
            typedef typename base_t::value_type value_type;
            typedef typename base_t::difference_type difference_type;
            typedef typename base_t::reference reference;

            ordinal_iterator() : index_(0) {}
            explicit ordinal_iterator(int x) : index_(x) {}

        private:
            void increment()
            {
                ++index_;
            }

            void decrement()
            {
                --index_;
            }

            void advance(difference_type offset)
            {
                index_ += offset;
            }

            difference_type distance_to(const ordinal_iterator& other) const
            {
                return other.index_ - index_;
            }

            bool equal(const ordinal_iterator& other) const
            {
                return index_ == other.index_;
            }

            reference dereference() const
            {
                return ordinal::val<T>(index_);
            }

            friend class ::boost::iterator_core_access;
            int index_;
        };
    } // namespace ordinal_detail
    #endif


    /**
     @tparam T set's element ordinal type

     @par Requires: @c T must be a model of <em>Ordinal</em>.

     \c ordinal_range is a model of the <em>RandomAccessRange</em> Concept associated to the ordinal \c T.
     */

    template<typename T/* , typename Traits=ordinal_range_traits<T> */ >
    class ordinal_range
        : public boost::iterator_range< ordinal_detail::ordinal_iterator<T/*, Traits*/> >
    {
        typedef ordinal_detail::ordinal_iterator<T/*, Traits*/> iterator_t;
        typedef boost::iterator_range<iterator_t> base_t;
    public:
        //! builds a ordinal range
        ordinal_range()
            : base_t(iterator_t(0), iterator_t(ordinal::meta::size<T>::value))
        {
        }

        //! builds a ordinal sub-range
        ordinal_range(T first, T last)
            : base_t(iterator_t(ordinal::pos(first)),
                     iterator_t(ordinal::pos(last)+1))
        {
        }
    };

    //! function to generate an ordinal range.
    //! \c make_range allows treating ordinals as a model of the \e RandomAccessRange Concept.

    //! @par Requires:<br> \c T is a model of the \e Ordinal Concept.
    //! @par Returns: an ordinal range from @c T'first to @c T'last inclusive.
    template<typename T /*, typename Traits*/ >
    ordinal_range<T>
    make_range()
    {
        return ordinal_range<T/*,Traits*/>();
    }

    //! function to generate an ordinal sub-range.
    //! \c make_range allows treating ordinal as a model of the \e RandomAccessRange Concept.
    //! It should be noted that the first and last parameters denoted a closed range.
    //! @par Requires: \c T is a model of the \e Ordinal Concept.
    //! @param first element of the range
    //! @param last element of the range
    //! @par Returns: an ordinal range from @c first to @c last inclusive.

    template<typename T /*, typename Traits */ >
    ordinal_range<T>
    make_range(T first, T last)
    {
        return ordinal_range<T /*,Traits*/ >(first,last);
    }

}} // namespace experimental::fundamental_v3
} // std

#endif // header
