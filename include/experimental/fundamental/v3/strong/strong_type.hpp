// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Copyright (C) 2010,2017 Vicente J. Botet Escriba

#ifndef JASEL_FUNDAMENTAL_V3_STRONG_STRONG_TYPE_HPP
#define JASEL_FUNDAMENTAL_V3_STRONG_STRONG_TYPE_HPP

#include <experimental/fundamental/v3/strong/tagged.hpp>
#include <experimental/fundamental/v2/config.hpp>

namespace std
{
namespace experimental
{
inline  namespace fundamental_v3
{


    //! strong_type wraps an underlying type providing access to the underlying value with a specific @c Final tag.
    //!
    //! The main goal of the Final tag is double : make two strong_type types with different Final tag different types and have access to the Final
    //!
    //! @Requires
    //! @li @c Final must publicly inherit from this class to make it a model of <em>FinalUnderlying</em>
    //! @li @c UT must be <em>CopyConstructible</em> and <em>Assignable</em>.
    //!
    //! @tparam Tag the tag type
    //! @tparam UT the underlying type

    //! strong_type provides a explicit constructor from convertibles to the underlying type.
    //! Some StronType types could need
    //! * implicit construction
    //!    => They must add an implicit constructor and forward (e.g. safe_bool)
    //! * construction only from the underlying type (e.g. byte)
    //!    => They must add an SFINAE constructor

    template <class Final, class UT>
    struct strong_type
    : tagged<Final, UT>
    {
      using base_type = tagged<Final, UT>;
      using base_type::base_type;

      using typename base_type::underlying_t;

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

    private:
      friend struct backdoor;
      friend struct const_backdoor;
      // The following should be accessible only to mixins of the Final class.
      class backdoor {
      public:
        constexpr backdoor(Final& f)
        : that(&f) {}

        //! explicit conversion to the underlying type.
        //template<typename F>
        JASEL_CXX14_CONSTEXPR underlying_t& _underlying() const {
          return that->value;
        }
      private:
        Final* that;
      };
      class const_backdoor {
      public:
        constexpr const_backdoor(Final const& f)
        : that(&f) {}

        //! explicit conversion to the underlying type.
        //template<typename F>
        JASEL_CXX14_CONSTEXPR underlying_t const& _underlying() const {
          return that->value;
        }
      private:
        Final const* that;
      };

    public:
      template<typename F>
      static const_backdoor _backdoor(F const* f)  {
        return const_backdoor(_final(f));
      }
      const_backdoor _backdoor() const {
        return const_backdoor(_final(this));
      }
      template<typename F>
      static backdoor _backdoor(F* f) {
        return backdoor(_final(f));
      }
      backdoor _backdoor() {
        return backdoor(_final(this));
      }
    };

    template <
      typename Final,
      typename UT,
      typename ...MetaMixins
    >
    struct new_class : strong_type<Final, UT>, MetaMixins::template type<Final>...
    {
      using base_type = strong_type<Final, UT>;
      using base_type::base_type;
    };

    template <
      typename Tag,
      typename UT,
      typename ...MetaMixins
    >
    struct new_type final : new_class<new_type<Tag, UT, MetaMixins...>, UT, MetaMixins...>
    {
      using base_type = new_class<new_type<Tag, UT, MetaMixins...>, UT, MetaMixins...>;
      using base_type::base_type;
    };

}
}
}

#endif // header
