//
// Created by simanstar on 16-10-23.
//

#ifndef STARTL_STARTL_SET_H
#define STARTL_STARTL_SET_H

#include "startl_rbtree.h"
#include "memory.h"
#include "startl_function.h"

namespace startl {

template <class Key, class Compare = less <Key>, template <class E> class Alloc = Allocator>
class set {
public:
  typedef Key key_type;
  typedef Key value_type;
  typedef Compare key_compare;
  typedef Compare value_compare;
private:
  typedef rb_tree<key_type, value_type, identity<value_type>, key_compare, Alloc> rep_type;
  typedef set<Key, Compare, Alloc> self;
  rep_type t;
public:
  typedef typename rep_type::const_pointer pointer;
  typedef typename rep_type::const_pointer const_pointer;
  typedef typename rep_type::const_reference reference;
  typedef typename rep_type::const_reference const_reference;
  typedef typename rep_type::const_iterator iterator;
  typedef typename rep_type::const_iterator const_iterator;
  typedef typename rep_type::const_reverse_iterator reverse_iterator;
  typedef typename rep_type::const_reverse_iterator const_reverse_iterator;
  typedef typename rep_type::size_type size_type;
  typedef typename rep_type::difference_type difference_type;

  set() : t(Compare()) {}
  explicit set(const Compare& comp) : t(comp) {}

  template <class InputIterator>
  set(InputIterator first, InputIterator last)
      :t(Compare()) { t.insert_unique(first, last); }

  template <class InputIterator>
  set(InputIterator first, InputIterator last, const Compare& comp)
      :t(comp) { t.insert_unique(first, last); }

  set(const self& x) : t(x.t) {}
  self& operator=(const self& x) {
    t = x.t;
    return *this;
  }

  key_compare key_comp() const { return t.key_comp(); }
  value_compare value_comp() const {return t.key_comp();}
  iterator begin() const {return t.begin();}
  iterator end() const {return t.end();}
  reverse_iterator rbegin() const {return t.rbegin();}
  reverse_iterator rend() const {return rend();}
  bool empty() const {return t.empty();}
  size_type size() const {return t.size();}
  size_type max_size() const { return t.max_size();}
  void swap(self& x) {t.swap(x.t);}
  //insert/erase
  typedef pair<iterator, bool> pair_t;

  pair_t insert(const value_type& x) {
    return t.insert_unique(x);                //与原来代码不一样
  }

  iterator insert(iterator position, const value_type& x) {
    typedef typename rep_type::iterator rep_iterator;
    return t.insert_unique((rep_iterator&)position, x);
  }

  template <class InputIterator>
  void insert(InputIterator first, InputIterator last) {
    t.insert_unique(first, last);
  }

  void erase(iterator position) {
    typedef typename rep_type::iterator rep_iterator;
    t.erase((rep_iterator&)position);
  }

  size_type erase(const key_type& x) {
    return t.erase(x);
  }

  void erase(iterator first, iterator last) {
    typedef typename rep_type::iterator rep_iterator;
    t.erase((rep_iterator&)first, (rep_iterator&)last);
  }
  void clear() {t.clear();}
  iterator find(const key_type& x) const {return t.find(x);}
  size_type count(const key_type& x) const {return t.count(x);}
  iterator lower_bound(const key_type& x) const {
    return t.lower_bound(x);
  }

  iterator upper_bound(const key_type& x) const {
    return t.upper_bound(x);
  }

  pair<iterator, iterator> equal_range(const key_type& x) const {
    return t.equal_range(x);
  }
  //以下需经过一系列语法测试,要改成以下才行，不懂
  template <class _Key, class _Compare, template <class _E> class _Alloc>
  friend bool operator == (const set&, const set&);
  template <class _Key, class _Compare, template <class _E> class _Alloc>
  friend bool operator < (const set&, const set&);
  template <class _Key, class _Compare, template <class _E> class _Alloc>
  friend bool operator != (const set&, const set&);
};

template <class Key, class Compare, template <class E> class Alloc>
inline bool operator==(const set<Key, Compare, Alloc>& x, const set<Key, Compare, Alloc>& y) {
  return x.t == y.t;
}

template <class Key, class Compare, template <class E> class Alloc>
inline bool operator!=(const set<Key, Compare, Alloc>& x, const set<Key, Compare, Alloc>& y) {
  return x.t != y.t;
}

template <class Key, class Compare, template <class E> class Alloc>
inline bool operator<(const set<Key, Compare, Alloc>& x, const set<Key, Compare, Alloc>& y) {
  return x.t < y.t;
}

}
#endif //STARTL_STARTL_SET_H
