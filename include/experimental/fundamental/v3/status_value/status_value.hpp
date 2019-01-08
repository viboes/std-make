// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// (C) Copyright 2019 Vicente J. Botet Escriba

#ifndef JASEL_EXPERIMENTAL_STATUS_VALUE_STATUS_VALUE_HPP
#define JASEL_EXPERIMENTAL_STATUS_VALUE_STATUS_VALUE_HPP

#if __cplusplus >= 201402L

#include <stdexcept>
#include <experimental/optional.hpp>
#include <experimental/fundamental/v2/config.hpp>

namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{
template <typename Status>
class bad_status_value_access : public exception
{
public:
  // constructors
  bad_status_value_access() = delete;
  explicit bad_status_value_access(Status s) : status_(s) {}

  // destructor
  ~bad_status_value_access() {}

  // status observers
  const Status& status() const noexcept { return status_; }

  const char* what() const noexcept { return "Bad Expected Access"; }

private:
  Status status_;
};

template <class Status, class Value>
class status_value
{
public:
    // ?.?.3.1 constructors
    status_value() = delete;
    constexpr status_value(Status s)
        : status_(s) {}
    constexpr status_value(Status s, const Value& v)
        : status_(s), optvalue_(v) {}
    constexpr status_value(Status s, Value&& v)
        : status_(s), optvalue_(move(v)) {}
    constexpr status_value(status_value&& sv) noexcept(is_nothrow_copy_constructible_v<Status> && is_nothrow_move_constructible_v<Value>)
        : status_(sv.status_), optvalue_(move(sv.optvalue_)) {}

    // ?.?.3.2 destructor
    ~status_value() {}

    // assignment
    status_value& operator=(const status_value&) = delete;

    // ?.?.3.3 status observers
    constexpr const Status& status() const noexcept { return status_; }

    // ?.?.3.4 state observers
    constexpr bool has_value() const noexcept { return optvalue_.has_value(); }
    constexpr explicit operator bool() const noexcept { return optvalue_.has_value(); }

    // ?.?.3.5 value observers
    constexpr const Value&  value() const & { if (optvalue_) return *optvalue_; throw bad_status_value_access<Status>(status_); }
    constexpr       Value&  value()       & { if (optvalue_) return *optvalue_; throw bad_status_value_access<Status>(status_); }
    constexpr       Value&& value()       && { if (optvalue_) return move(*optvalue_); throw bad_status_value_access<Status>(status_); }
    constexpr const Value&& value() const && { if (optvalue_) return move(*optvalue_); throw bad_status_value_access<Status>(status_); }
    constexpr const Value*  operator->() const { return &optvalue_.value(); }
    constexpr       Value*  operator->() { return &optvalue_.value(); }
    constexpr const Value&  operator*() const & { return optvalue_.value(); }
    constexpr       Value&  operator*()       & { return optvalue_.value(); }
    constexpr const Value&& operator*() const && { return optvalue_.value(); }
    constexpr       Value&& operator*()       && { return optvalue_.value(); }

private:
    Status status_;
    optional<Value> optvalue_;
};

}
}
}
#endif
#endif // header
