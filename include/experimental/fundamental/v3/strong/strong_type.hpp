// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Copyright (C) 2010,2017 Vicente J. Botet Escriba

#ifndef JASEL_FUNDAMENTAL_V3_STRONG_STRONG_TYPE_HPP
#define JASEL_FUNDAMENTAL_V3_STRONG_STRONG_TYPE_HPP

#include <experimental/fundamental/v3/strong/underlying_type.hpp>
#include <experimental/fundamental/v3/strong/tagged.hpp>

namespace std
{
namespace experimental
{
inline  namespace fundamental_v3
{

    //! strong_type wraps an underlying type providing access to the underlying value.  with a specific @c Final tag.
    //!
    //! The main goal of the Final tag is double : make two strong_type types with different Final tag different types and have access to the Final
    //!
    //! @Requires
    //! @li @c Final must publicly inherit from this class to make it a model of <em>FinalUnderlying</em>
    //! @li @c UT must be <em>CopyConstructible</em> and <em>Assignable</em>.
    //!
    //! @tparam Tag the tag type
    //! @tparam UT the underlying type

    template <class Final, class UT>
    class strong_type
    : tagged<Final, UT>
    {
      using base_type = tagged<Final, UT>;
      using base_type::base_type;

      using typename base_type::underlying_t;

    //private:
      // The following should be accessible only to mixins of the Final class.

      //! explicit conversion from a base const class to the @c Final class.
      template<typename F>
      static Final const& _final(F const* f)  {
        return static_cast<Final const&>(*f);
      }
      //! explicit conversion from a base class to the @c Final class.
      template<typename F>
      static Final& _final(F* f) {
        return static_cast<Final&>(*f);
      }

      //! explicit conversion from a @c Final class to the underlying type.
      template<typename F>
      static underlying_t& _underlying(F* f){
        return final(f).value;
      }

      //! explicit conversion from a <c>Final const</c> class to the underlying type.
      template<typename F>
      static underlying_t const& _underlying(F const* f){
        return final(f).value;
      }
    };

    //! public_strong_type is a strong_type tagged that provides implicit conversion to the underlying type
    //!
    //! @tparam Tag the tag type
    //! @tparam UT the underlying type
    template <class Final, class UT>
    struct public_strong_type
    : public_tagged<Final, UT>
    {
      using base_type = public_tagged<Final, UT>;
      using base_type::base_type;
    };

    //! protected_strong_type is a strong_type tagged that provides explicit conversion to the underlying type
    //!
    //! @tparam Tag the tag type
    //! @tparam UT the underlying type
    template <class Final, class UT>
    struct protected_strong_type
    : protected_tagged<Final, UT>
    {
      using base_type = protected_tagged<Final, UT>;
      using base_type::base_type;
    };

    //! private_strong_type is a strong_type tagged that provides no conversion to the underlying type
    //!
    //! @tparam Tag the tag type
    //! @tparam UT the underlying type
    template <class Final, class UT>
    struct private_strong_type
    : private_tagged<Final, UT>
    {
      using base_type = private_tagged<Final, UT>;
      using base_type::base_type;
    };

}
}
}

#endif // header
