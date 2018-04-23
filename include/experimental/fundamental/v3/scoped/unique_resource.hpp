#ifndef JASEL_EXPERIMENTAL_SCOPED_UNIQUE_RESOURCE_HPP
#define JASEL_EXPERIMENTAL_SCOPED_UNIQUE_RESOURCE_HPP

// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// (C) Copyright 2017 Vicente J. Botet Escriba
// modeled slightly after Andrescu's talk and article(s)


/*
 * MIT License
Copyright (c) 2016/2017 Eric Niebler, slightly adapted by Peter Sommerlad
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
 */

#if (__cplusplus > 201402L && defined __clang__) || defined JASEL_DOXYGEN_INVOKED

#include <experimental/fundamental/v3/scoped/detail/box.hpp>
#include <experimental/fundamental/v3/scoped/scope.hpp>
#include <experimental/type_traits.hpp>

#include <utility>

namespace std{
namespace experimental {
inline namespace fundamental_v3
{

template<typename R, typename D>
class unique_resource
{
    static_assert((is_move_constructible_v<R> && is_nothrow_move_constructible_v<R>) ||
                  is_copy_constructible_v<R>,
                  "resource must be nothrow_move_constructible or copy_constructible");
    static_assert((is_move_constructible_v<R> && is_nothrow_move_constructible_v<D>) ||
                  is_copy_constructible_v<D>,
                  "deleter must be nothrow_move_constructible or copy_constructible");

    static const unique_resource &this_; // never ODR used! Just for getting no_except() expr

    scope_detail::_box<R> resource;
    scope_detail::_box<D> deleter;
    bool execute_on_destruction { true };

    static constexpr auto is_nothrow_delete_v=bool_constant<noexcept(std::declval<D &>()(std::declval<R &>()))>::value;

public://should be private
    template<typename RR, typename DD,
        typename = enable_if_t<is_constructible_v<scope_detail::_box<R>, RR, scope_detail::_empty_scope_exit> &&
                                    is_constructible_v<scope_detail::_box<D>, DD, scope_detail::_empty_scope_exit>>>
    unique_resource(RR &&r, DD &&d, bool should_run)
    noexcept(noexcept(scope_detail::_box<R>(std::forward<RR>(r), scope_detail::_empty_scope_exit {})) &&
            noexcept(scope_detail::_box<D>(std::forward<DD>(d), scope_detail::_empty_scope_exit {})))
      : resource(std::forward<RR>(r), make_scope_exit([&] {if (should_run) d(r);}))
      , deleter(std::forward<DD>(d),  make_scope_exit([&, this] {if (should_run) d(get());}))
      , execute_on_destruction { should_run }
    {}
    // the following ICEs my g++
//    template<typename RM, typename DM, typename S>
//    friend
//    auto make_unique_resource_checked(RM &&r, const S &invalid, DM &&d)
//    noexcept(noexcept(make_unique_resource(std::forward<RM>(r), std::forward<DM>(d))));
    // the following as well:
//    template<typename MR, typename MD, typename S>
//    friend
//  auto make_unique_resource_checked(MR &&r, const S &invalid, MD &&d)
//    noexcept(is_nothrow_constructible_v<decay_t<MR>,MR> &&
//          is_nothrow_constructible_v<decay_t<MD>,MD>)
//    ->unique_resource<decay_t<MR>,decay_t<MD>>;


public:
    template<typename RR, typename DD,
        typename = enable_if_t<is_constructible<scope_detail::_box<R>, RR, scope_detail::_empty_scope_exit>::value &&
                                    is_constructible<scope_detail::_box<D>, DD, scope_detail::_empty_scope_exit>::value >
    >
    explicit unique_resource(RR &&r, DD &&d)
    noexcept(noexcept(scope_detail::_box<R>(std::forward<RR>(r), scope_detail::_empty_scope_exit {})) &&
             noexcept(scope_detail::_box<D>(std::forward<DD>(d), scope_detail::_empty_scope_exit {})))
      : resource(std::forward<RR>(r), make_scope_exit([&] {d(r);}))
      , deleter(std::forward<DD>(d), make_scope_exit([&, this] {d(get());}))
    {}
    unique_resource(    unique_resource&& that)
            noexcept(noexcept(scope_detail::_box<R>(that.resource.move(), scope_detail::_empty_scope_exit {})) &&
                     noexcept(scope_detail::_box<D>(that.deleter.move(), scope_detail::_empty_scope_exit {})))
        : resource(that.resource.move(), scope_detail::_empty_scope_exit { })
        , deleter(that.deleter.move(), make_scope_exit([&, this] { that.get_deleter()(get());that.release(); }))
        , execute_on_destruction(exchange(that.execute_on_destruction, false))
        { }

    unique_resource &operator=(unique_resource &&that)
        noexcept(is_nothrow_delete_v &&
                 is_nothrow_move_assignable_v<R> &&
                 is_nothrow_move_assignable_v<D>)
    {
        static_assert(is_nothrow_move_assignable<R>::value ||
                      is_copy_assignable<R>::value,
            "The resource must be nothrow-move assignable, or copy assignable");
        static_assert(is_nothrow_move_assignable<D>::value ||
                      is_copy_assignable<D>::value,
            "The deleter must be nothrow-move assignable, or copy assignable");
        if(&that == this)
            return *this;
        reset();
        if constexpr (is_nothrow_move_assignable_v<scope_detail::_box<R>>)
            if constexpr (is_nothrow_move_assignable_v<scope_detail::_box<D>>)
                {
                    resource = std::move(that.resource);//std::forward<scope_detail::_box<R>>(that.resource);
                    deleter = std::move(that.deleter);//std::forward<scope_detail::_box<D>>(that.deleter);
                }
            else
            {
                    deleter = _as_const(that.deleter);
                    resource = std::move(that.resource);//std::forward<scope_detail::_box<R>>(that.resource);
            }
        else
                if constexpr (is_nothrow_move_assignable_v<scope_detail::_box<D>>)
            {
                resource = _as_const(that.resource);
                deleter = std::move(that.deleter);//std::forward<scope_detail::_box<D>>(that.deleter);
            }
            else
            {
                resource = _as_const(that.resource);
                deleter = _as_const(that.deleter);
            }
        execute_on_destruction = exchange(that.execute_on_destruction, false);
        return *this;
    }
    ~unique_resource() //noexcept(is_nowthrow_delete_v) // removed deleter must not throw
    {
        reset();
    }

    void reset() noexcept
    {
        if(execute_on_destruction)
        {
            execute_on_destruction = false;
            get_deleter()(get());
        }
    }
    template<typename RR>
    auto reset(RR &&r)
        noexcept(noexcept(resource.reset(std::forward<RR>(r))))
    -> decltype(resource.reset(std::forward<RR>(r)), void())
    {
        auto &&guard = scope_fail([&, this]{ get_deleter()(r); });
        reset();
        resource.reset(std::forward<RR>(r));
        execute_on_destruction = true;
    }
    void release() noexcept
    {
        execute_on_destruction = false;
    }
    decltype(auto) get() const noexcept
    {
        return resource.get();
    }
    decltype(auto) get_deleter() const noexcept
    {
        return deleter.get();
    }
    template<typename RR=R>
    auto operator->() const noexcept//(noexcept(scope_detail::for_noexcept_on_copy_construction(this_.get())))
    -> enable_if_t<is_pointer_v<RR>,decltype(get())>
    {
        return get();
    }
    template<typename RR=R>
    auto operator*() const noexcept
        -> enable_if_t<is_pointer_v<RR> && ! is_void_v<remove_pointer_t<RR>>,
        add_lvalue_reference_t<remove_pointer_t<R>>>
    {
        return *get();
    }

    unique_resource& operator=(const unique_resource &) = delete;
    unique_resource(const unique_resource &) = delete;

};

#if defined JASEL_HAS_CONSTRUCTOR_GUIDES || defined JASEL_DOXYGEN_INVOKED
template<typename R, typename D>
unique_resource(R, D)
-> unique_resource<R, D>;
template<typename R, typename D>
unique_resource(R, D, bool)
-> unique_resource<R, D>;
#endif

template<typename R, typename D, typename S>
auto make_unique_resource_checked(R &&r, const S &invalid, D &&d)
noexcept(is_nothrow_constructible_v<decay_t<R>,R> &&
        is_nothrow_constructible_v<decay_t<D>,D>)
//->unique_resource<decay_t<R>,decay_t<D>>
{
    bool const mustrelease(r == invalid);
    //return //unique_resource
//          unique_resource<decay_t<R>,decay_t<D>>{std::forward<R>(r), std::forward<D>(d), ! bool(r == invalid)};
//  unique_resource<R,decay_t<D>>
    unique_resource resource{std::forward<R>(r), std::forward<D>(d),!mustrelease};
    return resource;

}

// end of (c) Eric Niebler part
}
}}

#endif
#endif /* UNIQUE_RESOURCE_H_ */
