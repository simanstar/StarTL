//
// Created by simanstar on 16-12-9.
//

#ifndef STARTL_STARTL_HASHSET_H
#define STARTL_STARTL_HASHSET_H

#include "startl_hashfun.h"
#include "startl_hashtable.h"
#include "memory.h"
#include "startl_function.h"

namespace startl {

template <class Value, class HashFcn = hash<Value>, class EqualKey = equal_to<Value>, template <class E> class Alloc = Allocator>
class hash_set {
private:
  typedef hashtable<Value, Value, HashFcn, identity<Value>, EqualKey, Alloc> ht;
  ht rep;
public:
  typedef typename ht::key_type key_type;
  typedef typename ht::value_type value_type;
  typedef typename ht::hasher hasher;
  typedef typename ht::key_equal key_equal;

  typedef typename ht::size_type size_type;
  typedef typename ht::difference_type difference_type;
  typedef typename ht::const_pointer pointer;
  typedef typename ht::const_pointer const_pointer;
  typedef typename ht::const_reference reference;
  typedef typename ht::const_reference const_reference;

  typedef typename ht::const_iterator iterator;
  typedef typename ht::const_iterator const_iterator;

  hasher hash_funct() const {
    return rep.hash_funct();
  }

  key_equal key_eq() const {
    return rep.key_eq();
  }

public:
  hash_set() : rep(100, hasher(), key_equal()) {}
  explicit hash_set(size_type n) : rep(n, hasher(), key_equal()) {}
  hash_set(size_type n, const hasher& hf) : rep(n, hf, key_equal()) {}
  hash_set(size_type n, const hasher& hf, const key_equal& eql) : rep(n, hf, eql) {}
  //不允许键值重复
  template <class InputIterator>
  hash_set(InputIterator f, InputIterator l) : rep(100, hasher(), key_equal()) {
    rep.insert_unique(f, l);
  }

  template <class InputIterator>
  hash_set(InputIterator f, InputIterator l, size_type n) : rep(n, hasher(), key_equal()) {
    rep.insert_unique(f, l);
  }

  template <class InputIterator>
  hash_set(InputIterator f, InputIterator l, size_type n, const hasher& hf, const key_equal& eql) : rep(n, hf, eql) {
    rep.insert_unique(f, l);
  }

public:
  size_type size() const {return rep.size();}
  size_type max_size() const {return rep.max_size();}
  bool empty() const {return rep.empty();}
  void swap(hash_set& hs) {rep.swap(hs.rep);}
  template <class _Value, class _HashFcn , class _EqualKey ,template <class E> class _Alloc >
  friend bool operator==(const hash_set&, const hash_set&);

  iterator begin() const {return rep.begin();}
  iterator end() const {return rep.end();}

  pair<iterator, bool> insert(const value_type& obj) {
    return rep.insert_unique(obj);
  }

  template <class InputIterator>
  void insert(InputIterator f, InputIterator l) {rep.insert_unique(f, l);}
  pair<iterator, bool> insert_noresize(const value_type& obj) {
    return rep.insert_unique_noresize(obj);
  }

  iterator find(const key_type& key) const {return rep.find(key);}
  size_type count(const key_type& key) const {return rep.count(key);}
  //pair<iterator, iterator> equal_range(const key_type& key) const {return rep.equal_range(key);}
  size_type erase(const key_type& key) {return rep.erase(key);}
  void clear() {rep.clear();}

public:
  void resize(size_type hint) {rep.resize(hint);}
  size_type bucket_count() const {return rep.bucket_count();}
  size_type max_bucket_count() const {return rep.max_bucket_count();}
  size_type elems_in_bucket(size_type n) const {return rep.elems_in_bucket(n);}

};
template <class Value, class HashFcn , class EqualKey , template <class E> class Alloc>
inline bool operator==(const hash_set<Value, HashFcn, EqualKey, Alloc>& hs1, const hash_set<Value, HashFcn, EqualKey, Alloc>& hs2) {
  return hs2.rep == hs1.rep;
};
}
#endif //STARTL_STARTL_HASHSET_H
