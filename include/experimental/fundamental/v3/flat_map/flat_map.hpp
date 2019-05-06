//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2019.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef JASEL_FUNDAMENTAL_V3_FLAT_MAP_FLAT_MAP_HPP
#define JASEL_FUNDAMENTAL_V3_FLAT_MAP_FLAT_MAP_HPP

#include <experimental/fundamental/v2/config.hpp>
#include <algorithm>
#include <cassert>
#include <initializer_list>
#include <iterator>
#include <stdexcept>
#include <vector>

#if 0

namespace std
{
// 26.6.8, class template flat_map
template <class Key, class T, class Compare = less<Key>, class KeyContainer = vector<Key>,
               class MappedContainer = vector<T>>
class flat_map;

template <class Key, class T, class Compare, class KeyContainer, class MappedContainer>
bool operator==(const flat_map<Key, T, Compare, KeyContainer, MappedContainer>& x,
               const flat_map<Key, T, Compare, KeyContainer, MappedContainer>& y);
template <class Key, class T, class Compare, class KeyContainer, class MappedContainer>
bool operator!=(const flat_map<Key, T, Compare, KeyContainer, MappedContainer>& x,
               const flat_map<Key, T, Compare, KeyContainer, MappedContainer>& y);
template <class Key, class T, class Compare, class KeyContainer, class MappedContainer>
bool operator<(const flat_map<Key, T, Compare, KeyContainer, MappedContainer>& x,
              const flat_map<Key, T, Compare, KeyContainer, MappedContainer>& y);
template <class Key, class T, class Compare, class KeyContainer, class MappedContainer>
bool operator>(const flat_map<Key, T, Compare, KeyContainer, MappedContainer>& x,
              const flat_map<Key, T, Compare, KeyContainer, MappedContainer>& y);
template <class Key, class T, class Compare, class KeyContainer, class MappedContainer>
bool operator<=(const flat_map<Key, T, Compare, KeyContainer, MappedContainer>& x,
               const flat_map<Key, T, Compare, KeyContainer, MappedContainer>& y);
template <class Key, class T, class Compare, class KeyContainer, class MappedContainer>
bool operator>=(const flat_map<Key, T, Compare, KeyContainer, MappedContainer>& x,
               const flat_map<Key, T, Compare, KeyContainer, MappedContainer>& y);

*template <class Key, class T, class Compare, class KeyContainer, class MappedContainer>
*void swap(flat_map<Key, T, Compare, KeyContainer, MappedContainer>& x,
*          flat_map<Key, T, Compare, KeyContainer, MappedContainer>& y) noexcept(noexcept(x.swap(y)));
*
*struct sorted_unique_t
*{
*    explicit sorted_unique_t() = default;
*};
*
inline constexpr sorted_unique_t sorted_unique {};

// 26.6.9, class template flat_multimap
template <class Key, class T, class Compare = less<Key>, class KeyContainer = vector<Key>,
               class MappedContainer = vector<T>>
class flat_multimap;

template <class Key, class T, class Compare, class KeyContainer, class MappedContainer>
bool operator==(const flat_multimap<Key, T, Compare, KeyContainer, MappedContainer>& x,
               const flat_multimap<Key, T, Compare, KeyContainer, MappedContainer>& y);
template <class Key, class T, class Compare, class KeyContainer, class MappedContainer>
bool operator!=(const flat_multimap<Key, T, Compare, KeyContainer, MappedContainer>& x,
               const flat_multimap<Key, T, Compare, KeyContainer, MappedContainer>& y);
template <class Key, class T, class Compare, class KeyContainer, class MappedContainer>
bool operator<(const flat_multimap<Key, T, Compare, KeyContainer, MappedContainer>& x,
              const flat_multimap<Key, T, Compare, KeyContainer, MappedContainer>& y);
template <class Key, class T, class Compare, class KeyContainer, class MappedContainer>
bool operator>(const flat_multimap<Key, T, Compare, KeyContainer, MappedContainer>& x,
              const flat_multimap<Key, T, Compare, KeyContainer, MappedContainer>& y);
template <class Key, class T, class Compare, class KeyContainer, class MappedContainer>
bool operator<=(const flat_multimap<Key, T, Compare, KeyContainer, MappedContainer>& x,
               const flat_multimap<Key, T, Compare, KeyContainer, MappedContainer>& y);
template <class Key, class T, class Compare, class KeyContainer, class MappedContainer>
bool operator>=(const flat_multimap<Key, T, Compare, KeyContainer, MappedContainer>& x,
               const flat_multimap<Key, T, Compare, KeyContainer, MappedContainer>& y);

template <class Key, class T, class Compare, class KeyContainer, class MappedContainer>
void swap(flat_multimap<Key, T, Compare, KeyContainer, MappedContainer>& x,
         flat_multimap<Key, T, Compare, KeyContainer, MappedContainer>& y) noexcept(noexcept(x.swap(y)));
struct sorted_equivalent_t
{
   explicit sorted_equivalent_t() = default;
};
inline constexpr sorted_equivalent_t sorted_equivalent {};

template <class Key, class T, class Compare = less<Key>, class KeyContainer = vector<Key>,
               class MappedContainer = vector<T>>
*class flat_map
*{
*public:
*// types:
*    using key_type = Key;
*    using mapped_type = T;
*    using value_type = pair<const key_type, mapped_type>;
*    using key_compare = Compare;
*    using reference = pair<const key_type&, mapped_type&>;
*    using const_reference = pair<const key_type&, const mapped_type&>;
*    using size_type = size_t;
*    using difference_type = ptrdiff_t;using iterator = implementation-defined; // see ??
*    using const_iterator = implementation-defined; // see ??
*    using reverse_iterator = std::reverse_iterator<iterator>;
*    using const_reverse_iterator = std::reverse_iterator<const_iterator>;
*    using key_container_type = KeyContainer;
*    using mapped_container_type = MappedContainer;
*    class value_compare
*    {
*        friend class flat_map;
*    protected:
*        key_compare comp;
*        value_compare(key_compare c)
*                        : comp(c)
*        {
*        }
*    public:
*        bool operator()(const_reference x, const_reference y) const
*        {
*            return comp(x.first, y.first);
*        }
*    };
*    struct containers
*    {
*        key_container_type keys;
*        mapped_container_type values;
*    };
*// 26.6.8.2, construct/copy/destroy
*    flat_map();
   flat_map(key_container_type&& key_cont, mapped_container_type&& mapped_cont);
   template <class Alloc>
   flat_map(key_container_type&& key_cont, mapped_container_type&& mapped_cont, const Alloc& a)
                   : flat_map(key_container_type(std::move(key_cont), a),
                              mapped_container_type(std::move(mapped_cont), a))
   {
   }
   template <class Container>
   explicit flat_map(const Container& cont)
                   : flat_map(cont.begin(), cont.end(), key_compare())
   {
   }
   template <class Container, class Alloc>
   flat_map(const Container& cont, const Alloc& a)
                   : flat_map(cont.begin(), cont.end(), key_compare(), a)
   {
   }
*    flat_map(sorted_unique_t, key_container_type&& key_cont, mapped_container_type&& mapped_cont);
   template <class Alloc>
   flat_map(sorted_unique_t s, key_container_type&& key_cont, mapped_container_type&& mapped_cont, const Alloc& a)
                   : flat_map(s, key_container_type(std::move(key_cont), a),
                              mapped_container_type(std::move(mapped_cont), a))
   {
   }
*    template <class Container>
*    flat_map(sorted_unique_t s, const Container& cont)
*                    : flat_map(s, cont.begin(), cont.end(), key_compare())
*    {
*    }
   template <class Container, class Alloc>
   flat_map(sorted_unique_t s, const Container& cont, const Alloc& a)

                   : flat_map(s, cont.begin(), cont.end(), key_compare(), a)
   {
   }
*    explicit flat_map(const key_compare& comp);
   template <class Alloc>
   flat_map(const key_compare& comp, const Alloc& a);
   template <class Alloc>
   explicit flat_map(const Alloc& a)
                   : flat_map(key_compare(), a)
   {
   }
   template <class InputIterator>
   flat_map(InputIterator first, InputIterator last, const key_compare& comp = key_compare());
   template <class InputIterator, class Alloc>
   flat_map(InputIterator first, InputIterator last, const key_compare& comp, const Alloc& a);
   template <class InputIterator, class Alloc>
   flat_map(InputIterator first, InputIterator last, const Alloc& a)
                   : flat_map(first, last, key_compare(), a)
   {
   }
*    template <class InputIterator>
*    flat_map(sorted_unique_t, InputIterator first, InputIterator last, const key_compare& comp = key_compare());
   template <class InputIterator, class Alloc>
   flat_map(sorted_unique_t, InputIterator first, InputIterator last, const key_compare& comp, const Alloc& a);
   template <class InputIterator, class Alloc>
   flat_map(sorted_unique_t s, InputIterator first, InputIterator last, const Alloc& a)
                   : flat_map(s, first, last, key_compare(), a)
   {
   }
   template <class Alloc>
   flat_map(flat_map&& m, const Alloc& a)
                   : compare { std::move(m.compare) }, c { key_container_type(std::move(m.c.keys), a),
                                                           mapped_container_type(std::move(m.c.values), a) }
   {
   }
   template <class Alloc>
   flat_map(const flat_map& m, const Alloc& a)
                   : compare { m.compare }, c { key_container_type(m.c.keys, a), mapped_container_type(m.c.values, a) }
   {
   }
   flat_map(initializer_list<pair<key_type, mapped_type>> && il, const key_compare& comp = key_compare())
                   : flat_map(il, comp)
   {
   }
   template <class Alloc>
   flat_map(initializer_list<pair<key_type, mapped_type>> && il, const key_compare& comp, const Alloc& a);
   template <class Alloc>
   flat_map(initializer_list<pair<key_type, mapped_type>> && il, const Alloc& a)
                   : flat_map(il, key_compare(), a)
   {
   }
*    flat_map(sorted_unique_t s, initializer_list<pair<key_type, mapped_type>> && il, const key_compare& comp =
*                             key_compare());
   template <class Alloc>
   flat_map(sorted_unique_t s, initializer_list<pair<key_type, mapped_type>> && il, const key_compare& comp,
            const Alloc& a);
   template <class Alloc>
   flat_map(sorted_unique_t s, initializer_list<pair<key_type, mapped_type>> && il, const Alloc& a)
                   : flat_map(s, il, key_compare(), a)
   {
   }
   flat_map& operator=(initializer_list<pair<key_type, mapped_type>> il);
*// iterators
*    iterator begin() noexcept;
*    const_iterator begin() const noexcept;
*    iterator end() noexcept;
*    const_iterator end() const noexcept;
*    reverse_iterator rbegin() noexcept;
*    const_reverse_iterator rbegin() const noexcept;
*    reverse_iterator rend() noexcept;
*    const_reverse_iterator rend() const noexcept;
*    const_iterator cbegin() const noexcept;
*    const_iterator cend() const noexcept;
*    const_reverse_iterator crbegin() const noexcept;
*    const_reverse_iterator crend() const noexcept;
*// 26.6.8.4, capacity
*    [[nodiscard]]
*    bool empty() const noexcept;
*    size_type size() const noexcept;
*    size_type max_size() const noexcept;
*// 26.6.8.5, element access
*    mapped_type& operator[](const key_type& x);
*    mapped_type& operator[](key_type&& x);
*    mapped_type& at(const key_type& x);
*    const mapped_type& at(const key_type& x) const;
*// 26.6.8.6, modifiers
*    template <class ... Args> pair<iterator, bool> emplace(Args&&... args);
*    template <class ... Args>
*    iterator emplace_hint(const_iterator position, Args&&... args);
*    pair<iterator, bool> insert(const value_type& x);
*    pair<iterator, bool> insert(value_type&& x);
*    template <class P> pair<iterator, bool> insert(P&& x);
*    iterator insert(const_iterator position, const value_type& x);
*    iterator insert(const_iterator position, value_type&& x);
*    template <class P>
*    iterator insert(const_iterator position, P&&);
*    template <class InputIterator>
*    void insert(InputIterator first, InputIterator last);
*    template <class InputIterator>
*    void insert(sorted_unique_t, InputIterator first, InputIterator last);
*    void insert(initializer_list<pair<key_type, mapped_type>>);
*    void insert(sorted_unique_t, initializer_list<pair<key_type, mapped_type>> il);
*    containers extract() &&;
*    void replace(key_container_type&& key_cont, mapped_container_type&& mapped_cont);
*    template <class ... Args>
*    pair<iterator, bool> try_emplace(const key_type& k, Args&&... args);
*    template <class ... Args>
*    pair<iterator, bool> try_emplace(key_type&& k, Args&&... args);
   template <class ... Args>
   iterator try_emplace(const_iterator hint, const key_type& k, Args&&... args);
   template <class ... Args>
   iterator try_emplace(const_iterator hint, key_type&& k, Args&&... args);
*    template <class M>
*    pair<iterator, bool> insert_or_assign(const key_type& k, M&& obj);
*    template <class M>
*    pair<iterator, bool> insert_or_assign(key_type&& k, M&& obj);
   template <class M>
   iterator insert_or_assign(const_iterator hint, const key_type& k, M&& obj);
   template <class M>
   iterator insert_or_assign(const_iterator hint, key_type&& k, M&& obj);
*    iterator erase(iterator position);
*    iterator erase(const_iterator position);
*    size_type erase(const key_type& x);
   iterator erase(const_iterator first, const_iterator last);
*    void swap(flat_map& fm) noexcept;
*    void clear() noexcept;
   template <class C2>
   void merge(flat_map<key_type, mapped_type, C2, key_container_type, mapped_container_type>& source);
   template <class C2>
   void merge(flat_map<key_type, mapped_type, C2, key_container_type, mapped_container_type> && source);
*// observers
*    key_compare key_comp() const;
*    value_compare value_comp() const;
*    const key_container_type& keys() const
*    {
*        return c.keys;
*    }
*    const mapped_container_type& values() const
*    {
*        return c.values;
*    }
*// map operations
*    iterator find(const key_type& x);
*    const_iterator find(const key_type& x) const;
*
   template <class K> iterator find(const K& x);
   template <class K> const_iterator find(const K& x) const;
*    size_type count(const key_type& x) const;
   template <class K> size_type count(const K& x) const;
*    bool contains(const key_type& x) const;
   template <class K> bool contains(const K& x) const;
*    iterator lower_bound(const key_type& x);
*    const_iterator lower_bound(const key_type& x) const;
   template <class K> iterator lower_bound(const K& x);
   template <class K> const_iterator lower_bound(const K& x) const;
*    iterator upper_bound(const key_type& x);
*    const_iterator upper_bound(const key_type& x) const;
   template <class K> iterator upper_bound(const K& x);
   template <class K> const_iterator upper_bound(const K& x) const;
*    pair<iterator, iterator> equal_range(const key_type& x);
*    pair<const_iterator, const_iterator> equal_range(const key_type& x) const;
   template <class K>
   pair<iterator, iterator> equal_range(const K& x);
   template <class K>
   pair<const_iterator, const_iterator> equal_range(const K& x) const;
private:
   containers c; // exposition only
   key_compare compare; // exposition only
};

template<class Container>
using cont-key-type =
remove_const_t<typename Container::value_type::first_type>; // exposition only
template<class Container>
using cont-mapped-type =
typename Container::value_type::second_type;// exposition only
template<class InputIterator>
using iter-key-type = remove_const_t<
typename iterator_traits<InputIterator>::value_type::first_type>;// exposition only
template<class InputIterator>
using iter-mapped-type =
typename iterator_traits<InputIterator>::value_type::second_type;// exposition only

#if 0
// guides
template <class Container>
flat_map (Container)
-> flat_map<cont-key-type <Container>, cont-mapped-type <Container>,
less<cont-key-type <Container>>,
vector<cont-key-type <Container>>,
vector<cont-mapped-type <Container>>>;

template <class KeyContainer, class MappedContainer>
flat_map(KeyContainer, MappedContainer)
-> flat_map<typename KeyContainer::value_type,
typename MappedContainer::value_type,
less<typename KeyContainer::value_type>,
KeyContainer, MappedContainer>;

template <class Container, class Alloc>
flat_map(Container, Alloc)
-> flat_map<cont-key-type <Container>, cont-mapped-type <Container>,
less<cont-key-type <Container>>,
vector<cont-key-type <Container>>,
vector<cont-mapped-type <Container>>>;

template <class KeyContainer, class MappedContainer, class Alloc>
flat_map(KeyContainer, MappedContainer, Alloc)
-> flat_map<typename KeyContainer::value_type,
typename MappedContainer::value_type,
less<typename KeyContainer::value_type>,
KeyContainer, MappedContainer>;

template <class Container>
flat_map(sorted_unique_t, Container)
-> flat_map<cont-key-type <Container>, cont-mapped-type <Container>,
less<cont-key-type <Container>>,
vector<cont-key-type <Container>>,
vector<cont-mapped-type <Container>>>;

template <class KeyContainer, class MappedContainer>
flat_map(sorted_unique_t, KeyContainer, MappedContainer)
-> flat_map<typename KeyContainer::value_type,
typename MappedContainer::value_type,
less<typename KeyContainer::value_type>,
KeyContainer, MappedContainer>;

template <class Container, class Alloc>
flat_map(sorted_unique_t, Container, Alloc)
-> flat_map<cont-key-type <Container>, cont-mapped-type <Container>,
less<cont-key-type <Container>>,
vector<cont-key-type <Container>>,
vector<cont-mapped-type <Container>>>;

template <class KeyContainer, class MappedContainer, class Alloc>
flat_map(sorted_unique_t, KeyContainer, MappedContainer, Alloc)
-> flat_map<typename KeyContainer::value_type,
typename MappedContainer::value_type,
less<typename KeyContainer::value_type>,
KeyContainer, MappedContainer>;

template <class InputIterator, class Compare = less<iter_key_t<InputIterator>>>
flat_map(InputIterator, InputIterator, Compare = Compare())
-> flat_map<iter_key_t<InputIterator>, iter-mapped-type <InputIterator>,
less<iter_key_t<InputIterator>>,
vector<iter_key_t<InputIterator>>,
vector<iter-mapped-type <InputIterator>>>;

template<class InputIterator, class Compare, class Alloc>
flat_map(InputIterator, InputIterator, Compare, Alloc)
-> flat_map<iter_key_t<InputIterator>, iter-mapped-type <InputIterator>, Compare,

vector<iter_key_t<InputIterator>>,
vector<iter-mapped-type <InputIterator>>>;
template<class InputIterator, class Alloc>
flat_map(InputIterator, InputIterator, Alloc)
-> flat_map<iter_key_t<InputIterator>, iter-mapped-type <InputIterator>,
less<iter_key_t<InputIterator>>,
vector<iter_key_t<InputIterator>>,
vector<iter-mapped-type <InputIterator>>>;

template <class InputIterator, class Compare = less<iter_key_t<InputIterator>>>
flat_map(sorted_unique_t, InputIterator, InputIterator, Compare = Compare())
-> flat_map<iter_key_t<InputIterator>, iter-mapped-type <InputIterator>,
less<iter_key_t<InputIterator>>,
vector<iter_key_t<InputIterator>>,
vector<iter-mapped-type <InputIterator>>>;

template<class InputIterator, class Compare, class Alloc>
flat_map(sorted_unique_t, InputIterator, InputIterator, Compare, Alloc)
-> flat_map<iter_key_t<InputIterator>, iter-mapped-type <InputIterator>, Compare,
vector<iter_key_t<InputIterator>>,
vector<iter-mapped-type <InputIterator>>>;

template<class InputIterator, class Alloc>
flat_map(sorted_unique_t, InputIterator, InputIterator, Alloc)
-> flat_map<iter_key_t<InputIterator>, iter-mapped-type <InputIterator>,
less<iter_key_t<InputIterator>>,
vector<iter_key_t<InputIterator>>,
vector<iter-mapped-type <InputIterator>>>;

template<class Key, class T, class Compare = less<Key>>
flat_map(initializer_list<pair<Key, T>>, Compare = Compare())
-> flat_map<Key, T, Compare, vector<Key>, vector<T>>;

template<class Key, class T, class Compare, class Alloc>
flat_map(initializer_list<pair<Key, T>>, Compare, Alloc)
-> flat_map<Key, T, Compare, vector<Key>, vector<T>>;

template<class Key, class T, class Alloc>
flat_map(initializer_list<pair<Key, T>>, Alloc)
-> flat_map<Key, T, less<Key>, vector<Key>, vector<T>>;
template<class Key, class T, class Compare = less<Key>>
flat_map(sorted_unique_t, initializer_list<pair<Key, T>>, Compare = Compare())
-> flat_map<Key, T, Compare, vector<Key>, vector<T>>;
template<class Key, class T, class Compare, class Alloc>
flat_map(sorted_unique_t, initializer_list<pair<Key, T>>, Compare, Alloc)
-> flat_map<Key, T, Compare, vector<Key>, vector<T>>;
template<class Key, class T, class Alloc>
flat_map(sorted_unique_t, initializer_list<pair<Key, T>>, Alloc)
-> flat_map<Key, T, less<Key>, vector<Key>, vector<T>>;
#endif

template <class Key, class T, class Compare = less<Key>, class KeyContainer = vector<Key>,
               class MappedContainer = vector<T>>
class flat_multimap
{

public:
// types:
   using key_type = Key;
   using mapped_type = T;
   using value_type = pair<const key_type, mapped_type>;
   using key_compare = Compare;
   using reference = pair<const key_type&, mapped_type&>;
   using const_reference = pair<const key_type&, const mapped_type&>;
   using size_type = size_t;
   using difference_type = ptrdiff_t;using iterator = implementation-defined; // see ??
   using const_iterator = implementation-defined; // see ??
   using reverse_iterator = std::reverse_iterator<iterator>;
   using const_reverse_iterator = std::reverse_iterator<const_iterator>;
   using key_container_type = KeyContainer;
   using mapped_container_type = MappedContainer;
   class value_compare
   {
       friend class flat_multimap;
   protected:
       key_compare comp;
       value_compare(key_compare c)
                       : comp(c)
       {
       }
   public:
       bool operator()(const_reference x, const_reference y) const
       {
           return comp(x.first, y.first);
       }
   };
   struct containers
   {
       key_container_type keys;
       mapped_container_type values;
   };
// 26.6.9.2, construct/copy/destroy
   flat_multimap();
   flat_multimap(key_container_type&& key_cont, mapped_container_type&& mapped_cont);
   template <class Alloc>
   flat_multimap(key_container_type&& key_cont, mapped_container_type&& mapped_cont, const Alloc& a)
                   : flat_map(key_container_type(std::move(key_cont), a),
                              mapped_container_type(std::move(mapped_cont), a))
   {
   }
   template <class Container>
   explicit flat_multimap(const Container& cont)
                   : flat_multimap(cont.begin(), cont.end(), key_compare())
   {
   }
   template <class Container, class Alloc>
   flat_multimap(const Container& cont, const Alloc& a)
                   : flat_multimap(cont.begin(), cont.end(), key_compare(), a)
   {
   }
   flat_multimap(sorted_equivalent_t, key_container_type&& key_cont, mapped_container_type&& mapped_cont);
   template <class Alloc>
   flat_multimap(sorted_equivalent_t, key_container_type&& key_cont, mapped_container_type&& mapped_cont,
                 const Alloc& a)
                   : flat_map(key_container_type(std::move(key_cont), a),
                              mapped_container_type(std::move(mapped_cont), a))
   {
   }
   template <class Container>
   flat_multimap(sorted_equivalent_t s, const Container& cont)
                   : flat_multimap(s, cont.begin(), cont.end(), key_compare())
   {
   }
   template <class Container, class Alloc>
   flat_multimap(sorted_equivalent_t s, const Container& cont, const Alloc& a)
                   : flat_multimap(s, cont.begin(), cont.end(), key_compare(), a)
   {
   }
   explicit flat_multimap(const key_compare& comp);
   template <class Alloc>
   flat_multimap(const key_compare& comp, const Alloc& a);
   template <class Alloc>
   explicit flat_multimap(const Alloc& a)
                   : flat_multimap(key_compare(), a)
   {
   }
   template <class InputIterator>
   flat_multimap(InputIterator first, InputIterator last, const key_compare& comp = key_compare());
   template <class InputIterator, class Alloc>
   flat_multimap(InputIterator first, InputIterator last, const key_compare& comp, const Alloc& a);
   template <class InputIterator, class Alloc>
   flat_multimap(InputIterator first, InputIterator last, const Alloc& a)
                   : flat_multimap(first, last, key_compare(), a)
   {
   }
   template <class InputIterator>
   flat_multimap(sorted_equivalent_t, InputIterator first, InputIterator last, const key_compare& comp =
                                 key_compare());
   template <class InputIterator, class Alloc>
   flat_multimap(sorted_equivalent_t, InputIterator first, InputIterator last, const key_compare& comp,
                 const Alloc& a);
   template <class InputIterator, class Alloc>
   flat_multimap(sorted_equivalent_t s, InputIterator first, InputIterator last, const Alloc& a)
                   : flat_multimap(s, first, last, key_compare(), a)
   {
   }
   template <class Alloc>
   flat_multimap(flat_multimap&& m, const Alloc& a)
                   : compare { std::move(m.compare) }, c { key_container_type(std::move(m.c.keys), a),
                                                           mapped_container_type(std::move(m.c.values), a) }
   {
   }
   template <class Alloc>
   flat_multimap(const flat_multimap& m, const Alloc& a)
                   : compare { m.compare }, c { key_container_type(m.c.keys, a), mapped_container_type(m.c.values, a) }
   {
   }
   flat_multimap(initializer_list<pair<key_type, mapped_type>> && il, const key_compare& comp = key_compare())
                   : flat_multimap(il, comp)
   {
   }

   template <class Alloc>
   flat_multimap(initializer_list<pair<key_type, mapped_type>> && il, const key_compare& comp, const Alloc& a)
                   : flat_multimap(il, comp, a)
   {
   }
   template <class Alloc>
   flat_multimap(initializer_list<pair<key_type, mapped_type>> && il, const Alloc& a)
                   : flat_multimap(il, key_compare(), a)
   {
   }
   flat_multimap(sorted_equivalent_t s, initializer_list<pair<key_type, mapped_type>> && il, const key_compare& comp =
                                 key_compare())
                   : flat_multimap(s, il, comp)
   {
   }
   template <class Alloc>
   flat_multimap(sorted_equivalent_t s, initializer_list<pair<key_type, mapped_type>> && il, const key_compare& comp,
                 const Alloc& a)
                   : flat_multimap(s, il, comp, a)
   {
   }
   template <class Alloc>
   flat_multimap(sorted_equivalent_t s, initializer_list<pair<key_type, mapped_type>> && il, const Alloc& a)
                   : flat_multimap(s, il, key_compare(), a)
   {
   }
   flat_multimap& operator=(initializer_list<pair<key_type, mapped_type>> il);
// iterators
   iterator begin() noexcept;
   const_iterator begin() const noexcept;
   iterator end() noexcept;
   const_iterator end() const noexcept;
   reverse_iterator rbegin() noexcept;
   const_reverse_iterator rbegin() const noexcept;
   reverse_iterator rend() noexcept;
   const_reverse_iterator rend() const noexcept;
   const_iterator cbegin() const noexcept;
   const_iterator cend() const noexcept;
   const_reverse_iterator crbegin() const noexcept;
   const_reverse_iterator crend() const noexcept;
// capacity
   [[nodiscard]]
   bool empty() const noexcept;
   size_type size() const noexcept;
   size_type max_size() const noexcept;
// 26.6.9.4, modifiers
   template <class ... Args> pair<iterator, bool> emplace(Args&&... args);
   template <class ... Args>
   iterator emplace_hint(const_iterator position, Args&&... args);
   pair<iterator, bool> insert(const value_type& x);
   pair<iterator, bool> insert(value_type&& x);
   template <class P> pair<iterator, bool> insert(P&& x);
   iterator insert(const_iterator position, const value_type& x);
   iterator insert(const_iterator position, value_type&& x);
   template <class P>
   iterator insert(const_iterator position, P&&);
   template <class InputIterator>
   void insert(InputIterator first, InputIterator last);
   template <class InputIterator>
   void insert(sorted_equivalent_t, InputIterator first, InputIterator last);
   void insert(initializer_list<pair<key_type, mapped_type>>);
   void insert(sorted_equivalent_t, initializer_list<pair<key_type, mapped_type>> il);
   containers extract() &&;
   void replace(key_container_type&& key_cont, mapped_container_type&& mapped_cont);
   iterator erase(iterator position);
   iterator erase(const_iterator position);
   size_type erase(const key_type& x);
   iterator erase(const_iterator first, const_iterator last);
   void swap(flat_multimap& fm) noexcept;
   void clear() noexcept;
   template <class C2>
   void merge(flat_multimap<key_type, mapped_type, C2, key_container_type, mapped_container_type>& source);
   template <class C2>
   void merge(flat_multimap<key_type, mapped_type, C2, key_container_type, mapped_container_type> && source);
// observers
   key_compare key_comp() const;
   value_compare value_comp() const;
   const key_container_type& keys() const
   {
       return c.keys;
   }
   const mapped_container_type& values() const
   {
       return c.values;
   }
// map operations
   iterator find(const key_type& x);
   const_iterator find(const key_type& x) const;
   template <class K> iterator find(const K& x);
   template <class K> const_iterator find(const K& x) const;
   size_type count(const key_type& x) const;
   template <class K> size_type count(const K& x) const;
   bool contains(const key_type& x) const;
   template <class K> bool contains(const K& x) const;
   iterator lower_bound(const key_type& x);
   const_iterator lower_bound(const key_type& x) const;
   template <class K> iterator lower_bound(const K& x);
   template <class K> const_iterator lower_bound(const K& x) const;
   iterator upper_bound(const key_type& x);
   const_iterator upper_bound(const key_type& x) const;
   template <class K> iterator upper_bound(const K& x);
   template <class K> const_iterator upper_bound(const K& x) const;

   pair<iterator, iterator> equal_range(const key_type& x);
   pair<const_iterator, const_iterator> equal_range(const key_type& x) const;
   template <class K>
   pair<iterator, iterator> equal_range(const K& x);
   template <class K>
   pair<const_iterator, const_iterator> equal_range(const K& x) const;
private:
   containers c; // exposition only
   key_compare compare; // exposition only
};

template<class Container>
using cont-key-type =
remove_const_t<typename Container::value_type::first_type>; // exposition only
template<class Container>
using cont-mapped-type =
typename Container::value_type::second_type;// exposition only
template<class InputIterator>
using iter-key-type = remove_const_t<
typename iterator_traits<InputIterator>::value_type::first_type>;// exposition only
template<class InputIterator>
using iter-mapped-type =
typename iterator_traits<InputIterator>::value_type::second_type;// exposition only

#if 0
// guides
template <class Container>
flat_multimap (Container)
-> flat_multimap<cont-key-type <Container>, cont-mapped-type <Container>,
less<cont-key-type <Container>>,
vector<cont-key-type <Container>>,
vector<cont-mapped-type <Container>>>;
template <class KeyContainer, class MappedContainer>
flat_multimap(KeyContainer, MappedContainer)
-> flat_multimap<typename KeyContainer::value_type,
typename MappedContainer::value_type,
less<typename KeyContainer::value_type>,
KeyContainer, MappedContainer>;
template <class Container, class Alloc>
flat_multimap(Container, Alloc)
-> flat_multimap<cont-key-type <Container>, cont-mapped-type <Container>,
less<cont-key-type <Container>>,
vector<cont-key-type <Container>>,
vector<cont-mapped-type <Container>>>;
template <class KeyContainer, class MappedContainer, class Alloc>
flat_multimap(KeyContainer, MappedContainer, Alloc)
-> flat_multimap<typename KeyContainer::value_type,
typename MappedContainer::value_type,
less<typename KeyContainer::value_type>,
KeyContainer, MappedContainer>;
template <class Container>
flat_multimap(sorted_equivalent_t, Container)
-> flat_multimap<cont-key-type <Container>, cont-mapped-type <Container>,
less<cont-key-type <Container>>,
vector<cont-key-type <Container>>,
vector<cont-mapped-type <Container>>>;
template <class KeyContainer, class MappedContainer>
flat_multimap(sorted_equivalent_t, KeyContainer, MappedContainer)
-> flat_multimap<typename KeyContainer::value_type,
typename MappedContainer::value_type,
less<typename KeyContainer::value_type>,
KeyContainer, MappedContainer>;
template <class Container, class Alloc>
flat_multimap(sorted_equivalent_t, Container, Alloc)
-> flat_multimap<cont-key-type <Container>, cont-mapped-type <Container>,
less<cont-key-type <Container>>,
vector<cont-key-type <Container>>,
vector<cont-mapped-type <Container>>>;
template <class KeyContainer, class MappedContainer, class Alloc>
flat_multimap(sorted_equivalent_t, KeyContainer, MappedContainer, Alloc)
-> flat_multimap<typename KeyContainer::value_type,
typename MappedContainer::value_type,
less<typename KeyContainer::value_type>,
KeyContainer, MappedContainer>;
template <class InputIterator, class Compare = less<iter-key-type <InputIterator>>>
flat_multimap(InputIterator, InputIterator, Compare = Compare())
-> flat_multimap<iter-key-type <InputIterator>, iter-mapped-type <InputIterator>,
less<iter-key-type <InputIterator>>,
vector<iter-key-type <InputIterator>>,
vector<iter-mapped-type <InputIterator>>>;
template<class InputIterator, class Compare, class Alloc>
flat_multimap(InputIterator, InputIterator, Compare, Alloc)
-> flat_multimap<iter-key-type <InputIterator>, iter-mapped-type <InputIterator>,
Compare, vector<iter-key-type <InputIterator>>,
vector<iter-mapped-type <InputIterator>>>;
template<class InputIterator, class Alloc>
flat_multimap(InputIterator, InputIterator, Alloc)
-> flat_multimap<iter-key-type <InputIterator>, iter-mapped-type <InputIterator>,
less<iter-key-type <InputIterator>>,
vector<iter-key-type <InputIterator>>,
vector<iter-mapped-type <InputIterator>>>;
template <class InputIterator, class Compare = less<iter-key-type <InputIterator>>>
flat_multimap(sorted_equivalent_t, InputIterator, InputIterator,
               Compare = Compare())
-> flat_multimap<iter-key-type <InputIterator>, iter-mapped-type <InputIterator>,
less<iter-key-type <InputIterator>>,
vector<iter-key-type <InputIterator>>,
vector<iter-mapped-type <InputIterator>>>;
template<class InputIterator, class Compare, class Alloc>
flat_multimap(sorted_equivalent_t, InputIterator, InputIterator, Compare, Alloc)
-> flat_multimap<iter-key-type <InputIterator>, iter-mapped-type <InputIterator>,
Compare, vector<iter-key-type <InputIterator>>,
vector<iter-mapped-type <InputIterator>>>;
template<class InputIterator, class Alloc>
flat_multimap(sorted_equivalent_t, InputIterator, InputIterator, Alloc)
-> flat_multimap<iter-key-type <InputIterator>, iter-mapped-type <InputIterator>,
less<iter-key-type <InputIterator>>,
vector<iter-key-type <InputIterator>>,
vector<iter-mapped-type <InputIterator>>>;
template<class Key, class T, class Compare = less<Key>>
flat_multimap(initializer_list<pair<Key, T>>, Compare = Compare())
-> flat_multimap<Key, T, Compare, vector<Key>, vector<T>>;
template<class Key, class T, class Compare, class Alloc>
flat_multimap(initializer_list<pair<Key, T>>, Compare, Alloc)
-> flat_multimap<Key, T, Compare, vector<Key>, vector<T>>;
template<class Key, class T, class Alloc>
flat_multimap(initializer_list<pair<Key, T>>, Alloc)
-> flat_multimap<Key, T, less<Key>, vector<Key>, vector<T>>;
template<class Key, class T, class Compare = less<Key>>
flat_multimap(sorted_equivalent_t, initializer_list<pair<Key, T>>,
               Compare = Compare())
-> flat_multimap<Key, T, Compare, vector<Key>, vector<T>>;
template<class Key, class T, class Compare, class Alloc>
flat_multimap(sorted_equivalent_t, initializer_list<pair<Key, T>>, Compare, Alloc)
-> flat_multimap<Key, T, Compare, vector<Key>, vector<T>>;
template<class Key, class T, class Alloc>
flat_multimap(sorted_equivalent_t, initializer_list<pair<Key, T>>, Alloc)
-> flat_multimap<Key, T, less<Key>, vector<Key>, vector<T>>;
#endif
}
#endif

namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{

//Move to utility/arrow_proxy.hpp
template <class Reference>
struct arrow_proxy
{
	Reference  r;
	Reference *operator->()
	{
		return &r;
	}
};

struct sorted_unique_t
{
	explicit sorted_unique_t() = default;
};
#if __cplusplus > 201402L
inline constexpr sorted_unique_t sorted_unique{};
#endif

struct sorted_equivalent_t
{
	explicit sorted_equivalent_t() = default;
};
#if __cplusplus > 201402L
inline constexpr sorted_equivalent_t sorted_equivalent{};
#endif

template <class Key, class T, class Compare = less<Key>, class KeyContainer = vector<Key>,
          class MappedContainer = vector<T>>
class flat_map;

template <class KeyConstIterator, class MappedIterator>
class flat_map_iterator {
public:
	using key_const_iterator_type = KeyConstIterator;
	using mapped_iterator_type    = MappedIterator;

	using key_type                 = typename key_const_iterator_type::value_type;
	using mapped_type              = typename mapped_iterator_type::value_type;
	using key_const_reference_type = typename key_const_iterator_type::reference;
	using mapped_reference_type    = typename mapped_iterator_type::reference;

	using iterator_category = random_access_iterator_tag;
	using value_type        = pair<const key_type, mapped_type>;
	using difference_type   = typename key_const_iterator_type::difference_type;
	using reference         = pair<key_const_reference_type, mapped_reference_type>;
	using pointer           = arrow_proxy<reference>;

	flat_map_iterator(key_const_iterator_type key_it, mapped_iterator_type value_it)
	        : key_it(key_it),
	          value_it(value_it)
	{
	}

	flat_map_iterator &operator++()
	{
		++key_it;
		++value_it;
		return *this;
	}
	flat_map_iterator operator++(int)
	{
		flat_map_iterator retval = *this;
		++(*this);
		return retval;
	}
	flat_map_iterator &operator--()
	{
		--key_it;
		--value_it;
		return *this;
	}
	flat_map_iterator operator--(int)
	{
		flat_map_iterator retval = *this;
		++(*this);
		return retval;
	}
	flat_map_iterator &operator+=(difference_type n)
	{
		key_it += n;
		value_it += n;
		return *this;
	}
	flat_map_iterator &operator-=(difference_type n)
	{
		key_it -= n;
		value_it -= n;
		return *this;
	}
	friend flat_map_iterator operator+(flat_map_iterator const &x, difference_type n)
	{
		return {x.key_it + n, x.value_it + n};
	}
	friend flat_map_iterator operator+(difference_type n, flat_map_iterator x)
	{
		return x + n;
	}
	friend flat_map_iterator operator-(flat_map_iterator const &x, difference_type n)
	{
		return {x.key_it - n, x.value_it - n};
	}
	friend difference_type operator-(flat_map_iterator const &x, flat_map_iterator const &y)
	{
		return x.key_it - y.key_it;
	}
	reference operator[](difference_type n) const
	{
		return *(*this + n);
	}
	reference operator*() const
	{
		return {*key_it, *value_it};
	}
	pointer operator->() const
	{
		return {reference{*key_it, *value_it}};
	}
	friend bool operator==(flat_map_iterator const &x, flat_map_iterator const &y)
	{
		return x.key_it == y.key_it;
	}
	friend bool operator!=(flat_map_iterator const &x, flat_map_iterator const &y)
	{
		return !(x == y);
	}
	friend bool operator<(flat_map_iterator const &x, flat_map_iterator const &y)
	{
		return x.key_it < y.key_it;
	}

private:
	template <class Key, class T, class Compare, class KeyContainer, class MappedContainer>
	friend class flat_map;
	key_const_iterator_type key_it;
	mapped_iterator_type    value_it;
};

template <class Key, class T, class Compare, class KeyContainer, class MappedContainer>
class flat_map {
public:
	// types:
	using key_container_type          = KeyContainer;
	using mapped_container_type       = MappedContainer;
	using key_iterator_type           = typename key_container_type::iterator;
	using key_const_iterator_type     = typename key_container_type::const_iterator;
	using mapped_iterator_type        = typename mapped_container_type::iterator;
	using mapped_const_iterator_type  = typename mapped_container_type::const_iterator;
	using key_type                    = typename key_container_type::value_type;
	using mapped_type                 = typename mapped_container_type::value_type;
	using key_reference_type          = typename key_container_type::reference;
	using key_const_reference_type    = typename key_container_type::const_reference;
	using mapped_reference_type       = typename mapped_container_type::reference;
	using mapped_const_reference_type = typename mapped_container_type::const_reference;

	using key_compare = Compare;

	using value_type      = pair<const key_type, mapped_type>;
	using reference       = pair<key_const_reference_type, mapped_reference_type>;
	using const_reference = pair<key_const_reference_type, mapped_const_reference_type>;
	using pointer         = arrow_proxy<reference>;
	using const_pointer   = arrow_proxy<const_reference>;
	using size_type       = size_t;
	using difference_type = ptrdiff_t;

	class value_compare {
		friend class flat_map;

	protected:
		key_compare comp;
		value_compare(key_compare c)
		        : comp(c)
		{
		}

	public:
		bool operator()(const_reference x, const_reference y) const
		{
			return comp(x.first, y.first);
		}
	};

	using iterator               = flat_map_iterator<key_const_iterator_type, mapped_iterator_type>;
	using const_iterator         = flat_map_iterator<key_const_iterator_type, mapped_const_iterator_type>;
	using reverse_iterator       = std::reverse_iterator<iterator>;
	using const_reverse_iterator = std::reverse_iterator<const_iterator>;

	struct containers
	{
		key_container_type    keys;
		mapped_container_type values;
	};

	// 26.6.8.2, construct/copy/destroy
	flat_map() {}
	explicit flat_map(const key_compare &comp)
	        : c{}, compare{comp}
	{
	}

	flat_map(key_container_type &&key_cont, mapped_container_type &&mapped_cont)
	        // : c{move(key_cont), move(mapped_cont)}, compare{}
	        : c{},
	          compare{}
	{
		assert(key_cont.size() == mapped_cont.size());
		//sort(begin(), end(), value_compare{compare});
		auto mapped_it = mapped_cont.begin();
		for (auto const &k : key_cont)
		{
			(void)emplace(k, *mapped_it);
			++mapped_it;
		}
	}
	template <class InputIterator>
	flat_map(InputIterator first, InputIterator last, const key_compare &comp = key_compare())
	{
		for (; first != last; ++first)
		{
			(void)emplace(first->first, first->second);
			// c.keys.insert(c.keys.end(), first->first);
			// c.values.insert(c.values.end(), first->second);
		}
		//sort(begin(), end(), compare);
	}

	template <class Container>
	explicit flat_map(const Container &cont)
	        : flat_map(cont.begin(), cont.end(), key_compare())
	{
	}

	/// sorted_unique_t constructors
	////////////////////
	flat_map(sorted_unique_t, key_container_type &&key_cont, mapped_container_type &&mapped_cont)
	        : c{move(key_cont), move(mapped_cont)}, compare{}
	{
	}
	template <class Container>
	explicit flat_map(sorted_unique_t s, const Container &cont)
	        : flat_map(s, cont.begin(), cont.end(), key_compare())
	{
	}

	template <class InputIterator>
	flat_map(sorted_unique_t, InputIterator first, InputIterator last, const key_compare &comp = key_compare())
	        : c{}, compare{comp}
	{
		for (; first != last; ++first)
		{
			c.keys.insert(c.keys.end(), first->first);
			c.values.insert(c.values.end(), first->second);
		}
	}

	// todo:
	/// constructor with allocators
	//    template <class InputIterator, class Alloc>
	//    flat_map(InputIterator first, InputIterator last, const key_compare& comp, const Alloc& a)
	//    {
	//        assert(0);
	//    }
	//
	//    template <class InputIterator, class Alloc>
	//    flat_map(sorted_unique_t, InputIterator first, InputIterator last, const key_compare& comp, const Alloc& a)
	//    {
	//        assert(0);
	//    }

	//    template <class Alloc>
	//    flat_map(key_container_type&& key_cont, mapped_container_type&& mapped_cont, const Alloc& a)
	//                    : flat_map(key_container_type(std::move(key_cont), a),
	//                               mapped_container_type(std::move(mapped_cont), a))
	//    {
	//    }
	//    template <class Container, class Alloc>
	//    flat_map(const Container& cont, const Alloc& a)
	//                    : flat_map(cont.begin(), cont.end(), key_compare(), a)
	//    {
	//    }
	//
	//
	//    template <class Alloc>
	//    flat_map(sorted_unique_t s, key_container_type&& key_cont, mapped_container_type&& mapped_cont, const Alloc& a)
	//                    : flat_map(s, key_container_type(std::move(key_cont), a),
	//                               mapped_container_type(std::move(mapped_cont), a))
	//    {
	//    }
	//
	//    template <class Container, class Alloc>
	//    flat_map(sorted_unique_t s, const Container& cont, const Alloc& a)
	//                    : flat_map(s, cont.begin(), cont.end(), key_compare(), a)
	//    {
	//    }
	//    template <class InputIterator, class Alloc>
	//    flat_map(sorted_unique_t s, InputIterator first, InputIterator last, const Alloc& a)
	//                    : flat_map(s, first, last, key_compare(), a)
	//    {
	//    }

	//    flat_map(initializer_list<pair<key_type, mapped_type>> && il, const key_compare& comp = key_compare())
	//                    : flat_map(il, comp)
	//    {
	//    }
	//    template <class Alloc>
	//    flat_map(initializer_list<pair<key_type, mapped_type>> && il, const key_compare& comp, const Alloc& a)
	//                    : flat_map(il, comp, a)
	//    {
	//    }
	//    template <class Alloc>
	//    flat_map(initializer_list<pair<key_type, mapped_type>> && il, const Alloc& a)
	//                    : flat_map(il, key_compare(), a)
	//    {
	//    }
	flat_map(sorted_unique_t s, initializer_list<pair<key_type, mapped_type>> &&il, const key_compare &comp = key_compare())
	        : flat_map(s, il.begin(), il.end(), comp)
	{
	}
	// todo:
#if 0
    template <class Alloc>
    flat_map(sorted_unique_t s, initializer_list<pair<key_type, mapped_type>> && il, const key_compare& comp,
             const Alloc& a)
                    : flat_map(s, il, comp, a)
    {
    }
    template <class Alloc>
    flat_map(sorted_unique_t s, initializer_list<pair<key_type, mapped_type>> && il, const Alloc& a)
                    : flat_map(s, il, key_compare(), a)
    {
    }
#endif
	flat_map &operator=(initializer_list<pair<key_type, mapped_type>> il);

	//// iterators
	iterator begin() noexcept
	{
		// return iterator{c.keys.cbegin(), c.values.begin()};
		return iterator{c.keys.begin(), c.values.begin()};
	}
	const_iterator begin() const noexcept
	{
		return const_iterator{c.keys.begin(), c.values.begin()};
	}
	iterator end() noexcept
	{
		// return iterator{c.keys.cend(), c.values.end()};
		return iterator{c.keys.end(), c.values.end()};
	}
	const_iterator end() const noexcept
	{
		return const_iterator{c.keys.end(), c.values.end()};
	}
	reverse_iterator rbegin() noexcept
	{
		return reverse_iterator{end()};
	}
	const_reverse_iterator rbegin() const noexcept
	{
		return const_reverse_iterator{end()};
	}

	reverse_iterator rend() noexcept
	{
		return reverse_iterator{begin()};
	}
	const_reverse_iterator rend() const noexcept
	{
		return reverse_iterator{begin()};
	}
	const_iterator cbegin() const noexcept
	{
		return const_iterator{c.keys.cbegin(), c.values.cbegin()};
	}

	const_iterator cend() const noexcept
	{
		return const_iterator{c.keys.cend(), c.values.cend()};
	}
	const_reverse_iterator crbegin() const noexcept
	{
		return const_reverse_iterator{cend()};
	}

	const_reverse_iterator crend() const noexcept
	{
		return const_reverse_iterator{cbegin()};
	}

	// 26.6.8.4, capacity
	//[[nodiscard]]
	bool      empty() const noexcept { return c.keys.empty(); }
	size_type size() const noexcept { return c.keys.size(); }
	size_type max_size() const noexcept { return std::min<size_type>(c.keys.max_size(), c.values.max_size()); }

	// 26.6.8.5, element access
	mapped_type &operator[](const key_type &x)
	{
		// note that we are unable to define iterator::operator->
		return (*try_emplace(x).first).second;
	}
	mapped_type &operator[](key_type &&x)
	{
		return (*try_emplace(move(x)).first).second;
	}
	mapped_type &at(const key_type &k)
	{
		auto it = lower_bound(k);
		if (points_to(it, k))
			return *it.value_it;
		throw out_of_range("flat_map bad key");
	}
	const mapped_type &at(const key_type &k) const
	{
		auto it = lower_bound(k);
		if (points_to(it, k))
			return *it.value_it;
		throw out_of_range("flat_map bad key");
	}

	// 26.6.8.6, modifiers
	template <class... Args>
	pair<iterator, bool> emplace(Args &&... args)
	{
		value_type t{std::forward<Args>(args)...};
		auto       k  = t.first;
		auto       it = lower_bound(k);
		if (points_to(it, k))
			return make_pair(it, false);

		auto nkey_it   = c.keys.emplace(it.key_it, std::move(t.first));
		auto nvalue_it = c.values.emplace(it.value_it, std::move(t.second));
		return std::make_pair(iterator{nkey_it, nvalue_it}, true);
	}
	template <class... Args>
	iterator emplace_hint(const_iterator position, Args &&... args)
	{
		// todo: don't ignore the position
		(void)position;
		value_type t{std::forward<Args>(args)...};
		auto       k  = t.first;
		auto       it = lower_bound(k);
		if (points_to(it, k))
			return it;

		auto nkey_it   = c.keys.emplace(it.key_it, std::move(t.first));
		auto nvalue_it = c.values.emplace(it.value_it, std::move(t.second));
		return iterator{nkey_it, nvalue_it};
	}

	pair<iterator, bool> insert(const value_type &x)
	{
		return emplace(x);
	}
	pair<iterator, bool> insert(value_type &&x)
	{
		return emplace(move(x));
	}

	template <class P>
	pair<iterator, bool> insert(P &&x)
	{
		return emplace(std::forward<P>(x));
	}

	iterator insert(const_iterator position, const value_type &x)
	{
		return emplace_hint(position, x);
	}

	iterator insert(const_iterator position, value_type &&x)
	{
		return emplace_hint(position, move(x));
	}

	template <class P>
	iterator insert(const_iterator position, P &&x)
	{
		return emplace_hint(position, std::forward<P>(x));
	}

	template <class InputIterator>
	void insert(InputIterator first, InputIterator last)
	{
		for (; first < last; ++first)
			insert(*first);
	}

	template <class InputIterator>
	void insert(sorted_unique_t, InputIterator first, InputIterator last)
	{
		// todo : optimize
		insert(first, last);
	}

	void insert(initializer_list<pair<key_type, mapped_type>> il)
	{
		insert(il.begin(), il.end());
	}

	void insert(sorted_unique_t, initializer_list<pair<key_type, mapped_type>> il)
	{
		// todo : optimize
		insert(il.begin(), il.end());
	}

	void replace(key_container_type &&key_cont, mapped_container_type &&mapped_cont)
	{
		assert(key_cont.size() == mapped_cont.size());
		c.keys   = std::move(key_cont);
		c.values = std::move(mapped_cont);
	}

	template <class... Args>
	pair<iterator, bool> try_emplace(const key_type &k, Args &&... args)
	{
		auto it = lower_bound(k);
		if (points_to(it, k))
			return std::make_pair(it, false);

		auto nkey_it   = c.keys.insert(it.key_it, k);
		auto nvalue_it = c.values.emplace(it.value_it, std::forward<Args>(args)...);
		return std::make_pair(iterator{nkey_it, nvalue_it}, true);
	}

	template <class... Args>
	pair<iterator, bool> try_emplace(key_type &&k, Args &&... args)
	{
		auto it = lower_bound(move(k));
		if (points_to(it, k))
			return std::make_pair(it, false);
		auto nkey_it   = c.keys.insert(it.key_it, k);
		auto nvalue_it = c.values.emplace(it.value_it, std::forward<Args>(args)...);
		return std::make_pair(iterator{nkey_it, nvalue_it}, true);
	}

	template <class... Args>
	iterator try_emplace(const_iterator hint, const key_type &k, Args &&... args)
	{
		// todo: don't ignore the hint
		(void)hint;
		auto it = lower_bound(k);
		if (points_to(it, k))
			return std::make_pair(it, false);

		auto nkey_it   = c.keys.insert(it.key_it, k);
		auto nvalue_it = c.values.emplace(it.value_it, std::forward<Args>(args)...);
		return std::make_pair(iterator{nkey_it, nvalue_it}, true);
	}

	template <class... Args>
	iterator try_emplace(const_iterator hint, key_type &&k, Args &&... args)
	{
		// todo: don't ignore the hint
		(void)hint;
		auto it = lower_bound(move(k));
		if (points_to(it, k))
			return std::make_pair(it, false);
		auto nkey_it   = c.keys.insert(it.key_it, k);
		auto nvalue_it = c.values.emplace(it.value_it, std::forward<Args>(args)...);
		return std::make_pair(iterator{nkey_it, nvalue_it}, true);
	}
	template <class M>
	pair<iterator, bool> insert_or_assign(const key_type &k, M &&obj)
	{
		auto it = lower_bound(k);
		if (points_to(it, k))
		{
			(*it).second = obj;
			return std::make_pair(it, false);
		}

		auto nkey_it   = c.keys.insert(it.key_it, k);
		auto nvalue_it = c.values.emplace(it.value_it, std::forward<M>(obj));
		return std::make_pair(iterator{nkey_it, nvalue_it}, true);
	}

	template <class M>
	pair<iterator, bool> insert_or_assign(key_type &&k, M &&obj)
	{
		auto it = lower_bound(k);
		if (points_to(it, k))
		{
			(*it).second = obj;
			return std::make_pair(it, false);
		}

		auto nkey_it   = c.keys.insert(it.key_it, move(k));
		auto nvalue_it = c.values.emplace(it.value_it, std::forward<M>(obj));
		return std::make_pair(iterator{nkey_it, nvalue_it}, true);
	}

	iterator erase(iterator position)
	{
		iterator res;
		res.key_it   = c.keys.erase(position.key_it);
		res.value_it = c.values.erase(position.value_it);
		return res;
	}

	iterator erase(const_iterator position)
	{
		iterator res;
		res.key_it   = c.keys.erase(position.key_it);
		res.value_it = c.values.erase(position.value_it);
		return res;
	}

	size_type erase(const key_type &k)
	{
		auto it = lower_bound(k);
		if (points_to(it, k))
		{
			erase(it);
			return 1;
		}
		return 0;
	}

	// todo:
	//    iterator erase(const_iterator first, const_iterator last);

	containers extract() &&
	{
		containers temp;
		temp.keys.swap(c.keys);
		temp.values.swap(c.values);
		return temp;
	}

	void swap(flat_map &fm) noexcept
	{
		using std::swap;
		swap(c.keys, fm.c.keys);
		swap(c.values, fm.c.values);
		swap(c.compare, fm.compare);
	}

	void clear() noexcept
	{
		c.keys.clear();
		c.values.clear();
	}

	// todo:
	//    template <class C2>
	//    void merge(flat_map<key_type, mapped_type, C2, key_container_type, mapped_container_type>& source)
	//    {
	//        flat_map res;
	//        std::merge(c.keys.begin(), c.keys.end(), source.c.keys.begin(), source.c.keys.end()
	//                   back_inserter(res));
	//        *this = res;
	//    }
	//    template <class C2>
	//    void merge(flat_map<key_type, mapped_type, C2, key_container_type, mapped_container_type> && source);

	//// observers
	key_compare   key_comp() const { return compare; }
	value_compare value_comp() const { return value_compare(compare); }

	const key_container_type &keys() const
	{
		return c.keys;
	}
	const mapped_container_type &values() const
	{
		return c.values;
	}

	// map operations
	iterator find(const key_type &x)
	{
		auto it = lower_bound(x);
		if (points_to(it, x))
			return it;
		return end();
	}
	const_iterator find(const key_type &x) const
	{
		auto it = lower_bound(x);
		if (points_to(it, x))
			return it;
		return end();
	}

// todo:
// Compare::is_transparent
#if 0
    template <class K>
    // requires Compare::is_transparent
    iterator find(const K& x)
    {
        auto it = lower_bound(x);
        if (points_to(it, x)) return it;
        return end();
    }

    template <class K>
    // requires Compare::is_transparent
    const_iterator find(const K& x) const
    {
        auto it = lower_bound(x);
        if (points_to(it, x)) return it;
        return end();
    }
#endif

	size_type count(const key_type &x) const
	{
		return contains(x) ? 1 : 0;
	}
	//    template <class K> size_type count(const K& x) const;

	bool contains(const key_type &x) const
	{
		//todo: optimize
#if 0
        return find(x) != end();
#else
		auto it = lower_bound(x);
		return points_to(it, x);
#endif
	}

	// todo:
	//    template <class K> bool contains(const K& x) const;

	iterator lower_bound(const key_type &x)
	{
		auto key_it   = std::lower_bound(c.keys.begin(), c.keys.end(), x, compare);
		auto value_it = c.values.begin() + distance(c.keys.begin(), key_it);
		return iterator{key_it, value_it};
	}

	const_iterator lower_bound(const key_type &x) const
	{
		auto key_it   = std::lower_bound(c.keys.begin(), c.keys.end(), x, compare);
		auto value_it = c.values.begin() + distance(c.keys.begin(), key_it);
		return const_iterator{key_it, value_it};
	}

	// todo:
	//    template <class K> iterator lower_bound(const K& x);
	//    template <class K> const_iterator lower_bound(const K& x) const;

	iterator upper_bound(const key_type &x)
	{
		auto key_it   = std::upper_bound(c.keys.begin(), c.keys.end(), x, compare);
		auto value_it = c.values.begin() + distance(c.keys.begin(), key_it);
		return iterator{key_it, value_it};
	}
	const_iterator upper_bound(const key_type &x) const
	{
		auto const key_it   = std::upper_bound(c.keys.begin(), c.keys.end(), x, compare);
		auto const value_it = c.values.begin() + distance(c.keys.begin(), key_it);
		return const_iterator{key_it, value_it};
	}

	// todo:
	//    template <class K> iterator upper_bound(const K& x);
	//    template <class K> const_iterator upper_bound(const K& x) const;

	pair<iterator, iterator> equal_range(const key_type &x)
	{
		auto       key_pit   = std::equal_range(c.keys.begin(), c.keys.end(), x, compare);
		auto const value_fit = c.values.begin() + distance(c.keys.begin(), key_pit.first);
		auto const value_sit = c.values.begin() + distance(c.keys.begin(), key_pit.second);
		return make_pair(iterator{key_pit.first, value_fit}, iterator{key_pit.second, value_sit});
	}

	pair<const_iterator, const_iterator> equal_range(const key_type &x) const
	{
		auto       key_pit   = std::equal_range(c.keys.begin(), c.keys.end(), x, compare);
		auto const value_fit = c.values.begin() + distance(c.keys.begin(), key_pit.first);
		auto const value_sit = c.values.begin() + distance(c.keys.begin(), key_pit.second);
		return make_pair(const_iterator{key_pit.first, value_fit}, const_iterator{key_pit.second, value_sit});
	}
	// todo:
	//    template <class K>
	//    pair<iterator, iterator> equal_range(const K& x);
	//    template <class K>
	//    pair<const_iterator, const_iterator> equal_range(const K& x) const;

private:
	bool points_to(iterator const &it, const key_type &x)
	{
		auto const key_it = it.key_it;
		return (key_it != c.keys.end() && compare(x, *key_it) == 0);
	}
	bool points_to(const_iterator const &it, const key_type &x) const
	{
		auto const key_it = it.key_it;
		return (key_it != c.keys.end() && compare(x, *key_it) == 0);
	}
	containers  c;
	key_compare compare;
};

template <class Key, class T, class Compare, class KeyContainer, class MappedContainer>
void swap(flat_map<Key, T, Compare, KeyContainer, MappedContainer> &x,
          flat_map<Key, T, Compare, KeyContainer, MappedContainer> &y) noexcept(noexcept(x.swap(y)))
{
	x.swap(y);
}

} // namespace fundamental_v3
} // namespace experimental
} // namespace std

#endif
