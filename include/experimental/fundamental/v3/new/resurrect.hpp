//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2019.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef JASEL_FUNDAMENTAL_V3_NEW_RESURRECT_HPP
#define JASEL_FUNDAMENTAL_V3_NEW_RESURRECT_HPP

namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{

// This is similar to the std::bless function, except that it is used only to
// resurrect objects hibernating on a buffer
template <class T>
T *resurrect(char *buf)
{
	return reinterpret_cast<T*>(u));
}

} // namespace fundamental_v3
} // namespace experimental
} // namespace std

#endif // header
