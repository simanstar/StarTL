//
// Created by simanstar on 16-10-29.
//

#ifndef STARTL_STARTL_MAP_H
#define STARTL_STARTL_MAP_H

#include "startl_rbtree.h"
#include "memory.h"
#include "startl_function.h"
#include "startl_pair.h"

namespace startl {
template <class Key, class T, class Compare = less<Key>, template <class E> class Alloc = Allocator>
class map {
public:
  typedef Key key_type;
  typedef T data_type;
  typedef T mapped_type;
  typedef pair<const Key, T> value_type;
  typedef Compare key_compare;

  class value_compare : public binary_function<value_type, value_type, bool> {
    friend class map;//???为什么要加入这个友元类
  protected:
    Compare comp;
    value_compare(Compare c) : comp(c) { }

  public:
    bool operator()(const value_type &x, const value_type &y) const {
      return comp(x.first, y.first);
    }
  };

private:
  typedef rb_tree<key_type, value_type, select1st<value_type>, key_compare, Alloc> rep_type;
  rep_type t;
public:
  typedef typename rep_type::pointer pointer;
  typedef typename rep_type::const_pointer const_pointer;
  typedef typename rep_type::reference reference;
  typedef typename rep_type::const_reference const_reference;
  typedef typename rep_type::iterator iterator;
  typedef typename rep_type::const_iterator const_iterator;
  typedef typename rep_type::reverse_iterator reverse_iterator;
  typedef typename rep_type::const_reverse_iterator const_reverse_iterator;
  typedef typename rep_type::size_type size_type;
  typedef typename rep_type::difference_type difference_type;

  map() : t(Compare()) {}
  explicit map(const Compare& comp) : t(comp) {}

  template <class InputIterator>
  map(InputIterator first, InputIterator last) : t(Compare()) {
    t.insert_unique(first, last);
  }

  template <class Inputiterator>
  map(Inputiterator first, Inputiterator last, const Compare& comp) : t(comp) {
    t.insert_unique(first, last);
  }

  map(const map& x) : t(x.t) {}
  map& operator=(const map& x) {                  //测试时试下这个复制函数
    t = x.t;
    return *this;
  }

  key_compare key_comp() const { return t.key_comp(); }
  value_compare value_comp() const { return value_compare(t.key_comp()); }
  iterator begin() { return t.begin(); }
  const_iterator begin() const { return t.begin(); }
  iterator end() { return t.end(); }
  //const_iterator end() { return t.end(); }  返回值不能重载，可以返回iterator 来构造const_iterator
  reverse_iterator rbegin() { return t.rbegin(); }
  //const_reverse_iterator rbegin() const {return t.rbegin(); }
  reverse_iterator rend() { return t.rend(); }
  //const_reverse_iterator rend() const { return t.rend(); }
  bool empty() const { return t.empty(); }
  size_type size() const {return t.size(); }
  size_type max_size() const { return t.max_size(); }
  //
  T& operator[](const key_type& k) {
    return (*((insert(value_type(k, T()))).first)).second;
  }
  void swap(map& x) { t.swap(x.t); }
  pair<iterator, bool> insert(const value_type& x) {
    return t.insert_unique(x);
  }

  iterator insert(iterator position, const value_type& x) {
    return t.insert_unique(position, x);
  }

  template <class InputIterator>
  void insert(InputIterator first, InputIterator last) {
    t.insert_unique(first, last);
  }

  void erase(iterator position) { t.erase(position); }
  size_type erase(const key_type& x) { return t.erase(x); }
  void erase(iterator first, iterator last) { t.erase(first, last); }
  void clear() { t.clear(); }
  iterator find(const key_type& x) { return t.find(x); }
  const_iterator find(const key_type& x) const { return t.find(x); }
  size_type count(const key_type& x) const { return t.count(x); }
  iterator lower_bound(const key_type& x) { return t.lower_bound(x); }
  const_iterator lower_bound(const key_type& x) const { return t.lower_bound(x); }
  iterator upper_bound(const key_type& x) { return t.upper_bound(x); }
  const_iterator upper_bound(const key_type& x) const {
    return t.upper_bound(x);
  }

  pair<iterator, iterator> equal_range(const key_type& x) {
    return t.equal_range(x);
  }

  pair<const_iterator, const_iterator> equal_range(const key_type& x) const {
    return t.equal_range(x);
  }
  //为啥用友元？？待看,以下类型参数列表没用？？直接看map的参数列表就确定了以下的函数？
//  template <class Key, class T, class Compare, template <class E> class Alloc >
   bool operator==(/*const map<_Key, _T, _Compare, _Alloc>& x, */const map& y) {
    return t == y.t;
  }

  /*
  template <class _Key, class _T, class _Compare, template <class _E> class _Alloc >
  friend bool operator< (const map&, const map&);
   */
};
/*
template <class Key, class T, class Compare , template <class E> class Alloc>
inline bool operator==(const map<Key, T, Compare, Alloc>& x, const map<Key, T, Compare, Alloc>& y) {
  return x.t == y.t;
}

template <class Key, class T, class Compare = less<Key>, template <class E> class Alloc = Allocator>
inline bool operator<(const map<Key, T, Compare, Alloc>& x, const map<Key, T, Compare, Alloc>& y) {
  return x.t < y.t;
}
*/
}

#endif //STARTL_STARTL_MAP_H
