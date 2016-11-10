// Copyright (C) 2016 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef JASEL_EXAMPLE_FRAMEWORK_SERIALIZABLEE_HPP
#define JASEL_EXAMPLE_FRAMEWORK_SERIALIZABLEE_HPP

#if __cplusplus >= 201402L

#include <type_traits>
#include <cstddef>
#include <utility>
#include <iterator>
#include <vector>
#include <experimental/meta.hpp>
#include <experimental/product_type.hpp>
#include <iostream>
extern bool trace;

namespace std
{
namespace experimental
{

inline namespace fundamental_v3
{
  namespace archives
  {

  class binary_archive {
    using storage = vector<char>;
  public:
    template <class T>
    constexpr typename std::enable_if<is_trivial<decay_t<T>>::value>::type
    save(T&& v) {
      const char* ptr = reinterpret_cast<const char*>(&v);
      copy(ptr, ptr+sizeof(T), back_insert_iterator<storage>(_data));
    }
    size_t size() const { return _data.size(); }
    const char* data() const { return _data.data(); }
  private:
    vector<char> _data;
  };
  }
namespace serializable
{
  namespace adl_serializable
  {
#if 1
    // trivial types are serialized directly by the archive
    template <typename Archine, typename T>
    //constexpr typename std::enable_if<is_trivial<decay_t<T>>::value>::type
    constexpr typename std::enable_if<is_trivial< remove_cv_t<remove_reference_t<T>> >::value>::type
    //void
    save(Archine& ar, T&& v)
    {
      if (trace) std::cout << "is_trivial\n";
      return ar.save(forward<T>(v));
    }
#else
    template <typename Archine, typename T>
    constexpr typename std::enable_if<is_trivial<T>::value>::type
    //void
    save(Archine& ar, T const& v)
    { return ar.save(v);}

#endif

#if __cplusplus > 201402L

    namespace detail {
      template <class PT, typename Archine, std::size_t... I>
      constexpr decltype(auto) save_impl( Archine& ar, PT&& pt, index_sequence<I...> ) noexcept
      {
        swallow ( (save(ar, product_type::get<I>(forward<PT>(pt)) ), 0) ... );
      }
    }
    template <typename PT, typename Archine>
    constexpr
    enable_if_t<
    //conjunction_v<
          not is_trivial<remove_cv_t<remove_reference_t<PT>>>::value
          and
          is_product_type_v<remove_cv_t<remove_reference_t<PT>>>
      //, all_of<is_adl_serializable<product_type::element<I,PT>>
      //>
      >
    //decltype(auto)
    save(Archine& ar, PT&& pt) noexcept
    {
      if (trace) std::cout << "is_product_type_v\n";
      detail::save_impl(ar, forward<PT>(pt), product_type::element_sequence_for<PT>{});
    }
#endif

    struct save_fn
    {
      template <typename T, typename Archine>
      constexpr auto operator()(Archine& ar, T&& v)  const noexcept -> decltype(save(ar, forward<T>(v)))
      {
        if (trace) std::cout << "save_fn\n";
        return save(ar, forward<T>(v));
      }
    };
  } // adl
  // To avoid ODR violations:
    template <class T>  constexpr T __static_const{};
    // std::begin is a global function object
    namespace {
      constexpr auto const & save = __static_const<adl_serializable::save_fn>;
    }
} // serializable
} // funcdamental_v3
} // experimental
} // std

#endif

#endif
