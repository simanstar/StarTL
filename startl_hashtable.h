//
// Created by simanstar on 16-11-2.
//

#ifndef STARTL_STARTL_HASHTABLE_H
#define STARTL_STARTL_HASHTABLE_H

#include "startl_iterator.h"
#include "startl_vector.h"
#include "startl_pair.h"
#include "startl_algobase.h"


namespace startl {

static const int __startl_num_primes = 28;
static const unsigned long __startl_prime_list [__startl_num_primes] = {
    53, 97, 193, 389, 769,
    1543, 3079, 6151, 12289, 24593,
    49157, 98317, 196613, 393241, 786433,
    1572869, 31457339, 6291469, 12582917, 25165843,
    50331653, 100663319, 201326611, 402653189, 805306457,
    1610612741, 3221225473ul, 4294967291ul
};

template <class Value>
struct __hashtable_node {
  __hashtable_node *next;
  Value val;
};

template <class Key, class Value, class HashFcn, class ExtractKey, class EqualKey, template <class E> class Alloc>
class hashtable;

template <class Key, class Value, class HashFcn, class ExtractKey, class EqualKey, template <class E> class Alloc>
struct __hashtable_iterator {
  typedef hashtable <Key, Value, HashFcn, ExtractKey, EqualKey, Alloc> _hashtable;
  typedef __hashtable_iterator iterator;
  typedef __hashtable_node<Value> node;

  typedef forward_iterator_tag iterator_category;
  typedef Value value_type;
  typedef ptrdiff_t difference_type;
  typedef size_t size_type;
  typedef Value& reference;
  typedef Value* pointer;

  node *cur;
  _hashtable *ht;

  __hashtable_iterator(node *n, _hashtable *tab) : cur(n), ht(tab) { }

  __hashtable_iterator(__hashtable_iterator& atr) : cur(atr.cur), ht(atr.ht){ }

  template <class any_hashtable_iterator>   // for build error;识别不了typedef的类型
  __hashtable_iterator(const any_hashtable_iterator& atr) : cur(atr.cur), ht(atr.ht){ }

  __hashtable_iterator() { }

  reference operator*() const { return cur->val; }

  pointer operator->() const { return &(operator*()); }

  iterator &operator++();

  iterator operator++(int);

  bool operator==(const iterator &it) const { return cur == it.cur; }

  bool operator!=(const iterator &it) const { return cur != it.cur; }

};

template <class Key, class Value, class HashFcn, class ExtractKey, class EqualKey, template <class E> class Alloc>
struct __hashtable_const_iterator {
  typedef hashtable <Key, Value, HashFcn, ExtractKey, EqualKey, Alloc> _hashtable;
  typedef __hashtable_const_iterator iterator;
  typedef __hashtable_node<Value> node;

  typedef forward_iterator_tag iterator_category;
  typedef Value value_type;
  typedef ptrdiff_t difference_type;
  typedef size_t size_type;
  typedef const Value& reference;
  typedef const Value* pointer;

  const node *cur;
  const _hashtable *ht;

  __hashtable_const_iterator(const node *n, const _hashtable *tab) : cur(n), ht(tab) { }

  __hashtable_const_iterator(const __hashtable_const_iterator& atr) : cur(atr.cur), ht(atr.ht){ }

  template <class any_hashtable_const_iterator>   // for build error;
  __hashtable_const_iterator(const any_hashtable_const_iterator& atr) : cur(atr.cur), ht(atr.ht){ }

  __hashtable_const_iterator() { }

  reference operator*() const { return cur->val; }

  pointer operator->() const { return &(operator*()); }

  iterator &operator++();

  iterator operator++(int);

  bool operator==(const iterator &it) const { return cur == it.cur; }

  bool operator!=(const iterator &it) const { return cur != it.cur; }

};

template <class Key, class Value, class HashFcn, class ExtractKey, class EqualKey, template <class E> class Alloc>
__hashtable_iterator<Key, Value, HashFcn, ExtractKey, EqualKey, Alloc> &
__hashtable_iterator<Key, Value, HashFcn, ExtractKey, EqualKey, Alloc>::operator++() {
  const node *old = cur;
  cur = cur->next;
  if (!cur) {
    size_type bucket = ht->bkt_num(old->val);
    while (!cur && ++bucket < ht->buckets.size())
      cur = ht->buckets[bucket];
  }
  return *this;
}

template <class Key, class Value, class HashFcn, class ExtractKey, class EqualKey, template <class E> class Alloc>
__hashtable_iterator<Key, Value, HashFcn, ExtractKey, EqualKey, Alloc>
__hashtable_iterator<Key, Value, HashFcn, ExtractKey, EqualKey, Alloc>::operator++(int) {
  iterator tmp = *this;
  ++*this;
  return tmp;
}

template <class Key, class Value, class HashFcn, class ExtractKey, class EqualKey, template <class E> class Alloc>
__hashtable_const_iterator<Key, Value, HashFcn, ExtractKey, EqualKey, Alloc> &
__hashtable_const_iterator<Key, Value, HashFcn, ExtractKey, EqualKey, Alloc>::operator++() {
  const node *old = cur;
  cur = cur->next;
  if (!cur) {
    size_type bucket = ht->bkt_num(old->val);
    while (!cur && ++bucket < ht->buckets.size())
      cur = ht->buckets[bucket];
  }
  return *this;
}

template <class Key, class Value, class HashFcn, class ExtractKey, class EqualKey, template <class E> class Alloc>
__hashtable_const_iterator<Key, Value, HashFcn, ExtractKey, EqualKey, Alloc>
__hashtable_const_iterator<Key, Value, HashFcn, ExtractKey, EqualKey, Alloc>::operator++(int) {
  iterator tmp = *this;
  ++*this;
  return tmp;
}

template <class Key, class Value, class HashFcn, class ExtractKey, class EqualKey, template <class E> class Alloc = Allocator>
class hashtable {
public:
  typedef HashFcn hasher;
  typedef EqualKey key_equal;
  typedef size_t size_type;
  typedef Key key_type;
  typedef Value value_type;
  typedef ptrdiff_t difference_type;
  typedef value_type* pointer;
  typedef const value_type* const_pointer;
  typedef Value& reference;
  typedef const Value& const_reference;
  typedef __hashtable_iterator<Key, Value, HashFcn, ExtractKey, EqualKey, Alloc> iterator;
  typedef __hashtable_const_iterator<Key,  Value, HashFcn, ExtractKey, EqualKey, Alloc> const_iterator;
  friend  class __hashtable_iterator<Key, Value, HashFcn, ExtractKey, EqualKey, Alloc> ;
  friend  class __hashtable_const_iterator<Key,  Value, HashFcn, ExtractKey, EqualKey, Alloc>;
private:
  hasher hash;
  key_equal equals;
  ExtractKey get_key;

  typedef __hashtable_node<Value> node;
  typedef Alloc<node> node_allocator;

  vector<node *, Alloc> buckets;
  size_type num_elements;
public:
  hashtable(size_type n, const HashFcn& hf, const EqualKey& eql) : hash(hf), equals(eql), get_key(ExtractKey()), num_elements(0) {
    initialize_buckets(n);
  }

  size_type bucket_count() const { return buckets.size(); }
  size_type max_bucket_count() const {
    return __startl_prime_list[__startl_num_primes - 1];
  }

  node* new_node(const value_type& obj) {
    node* n = node_allocator::allocate();
    n->next = nullptr;
    try {
      construct(&n->val, obj);
      return n;
    } catch (...){
      node_allocator::deallocate(n);
    }
  }

  void delete_node(node* n) {
    destroy(&n->val);
    node_allocator::deallocate(n);
  }

  pair<iterator, bool> insert_unique(const value_type& obj) {
   resize(num_elements + 1);
   return insert_unique_noresize(obj);
  }

  template <class InputIterator>
  void insert_unique(InputIterator f, InputIterator l) {
    for (; f != l ; ++f)
      insert_unique(*f);
  }

  iterator insert_equal(const value_type& obj) {
    resize(num_elements + 1);
    return insert_equal_noresize(obj);
  }

  void clear();
  void copy_from(const hashtable& ht);

  iterator find(const key_type& key) {
    size_type n = bkt_num_key(key);
    node* first;
    for (first = buckets[n]; first; first = first->next) {
      if (equals(get_key(first->val), key))
        break;
    }
    return iterator(first, this);
  }

  const_iterator find(const key_type& key) const {
    size_type n = bkt_num_key(key);
    node* first;
    for (first = buckets[n]; first; first = first->next) {
      if (equals(get_key(first->val), key))
        break;
    }
    return const_iterator(first, this);
  }

  size_type count(const key_type& key) const {
    const size_type n = bkt_num_key(key);
    size_type result = 0;
    for (const node* cur = buckets[n]; cur; cur = cur->next)
      if (equals(get_key(cur->val), key))
        ++result;
    return result;
  }

  hasher hash_funct() const {
    return hash;
  }

  key_equal key_eq() const {
    return equals;
  }

  size_type size() const { return num_elements ; }
  size_type max_size() const {return size_type (-1);}
  bool empty() const {return num_elements == 0;}
  void swap(hashtable& ht) {
    __startl::swap(hash, ht.hash);
    __startl::swap(equals, ht.equals);
    __startl::swap(buckets, ht.buckets);
    __startl::swap(num_elements, ht.num_elements);

  }

  iterator begin() {
    for (size_type n = 0;  n < num_elements; ++n)
      if (buckets[n])
        return iterator(buckets[n],this);
    return end();
  }

  const_iterator begin() const{
    for (size_type n = 0;  n < buckets.size(); ++n)
      if (buckets[n])
        return const_iterator(buckets[n],this);
    return end();
  }
  //最后被使用节点的下个位置，所以使用0来初始化迭代器
  iterator end()  {                                       //
    return iterator(nullptr, this);
  }

  const_iterator end() const {                                       //
    return const_iterator(nullptr, this);
  }

  pair<iterator, bool> insert_unique_noresize(const value_type& obj);

  size_type erase(const key_type& key) {
    size_type n = bkt_num_key(key);
    node* first = buckets[n];
    node* pre_first = nullptr;
    size_type num = 0;
    for (; first; ) {
      if (equals(get_key(first->val), key)) {
        node* temp = first;
        if (pre_first == nullptr) buckets[n] = first->next; //first为第一个
        else pre_first->next = first->next;
        first = first->next;
        delete_node(temp);
        num++;
      } else {
        pre_first = first;
        first = first->next;
      }
    }
    return num;

  }
  void resize(size_type num_elements_hint);

  size_type elems_in_bucket(size_type n) const {
    size_type result = 0;
    for (const node* cur = buckets[n]; cur; cur = cur->next)
        ++result;
    return result;
  }

  template <class _Key, class _Value, class _HashFcn, class _ExtractKey, class _EqualKey, template <class E> class _Alloc>
  friend bool operator==(const hashtable&, const hashtable&);

  iterator insert_equal_noresize(const value_type& obj);
private:
  inline unsigned long __startl_next_prime(unsigned long n) const{
    const unsigned long * first = __startl_prime_list;
    const unsigned long * last = __startl_prime_list + __startl_num_primes;
    const unsigned long * pos = lower_bound(first, last, n);
    return pos == last ? *(last - 1) : *pos;
  }

  void initialize_buckets(size_type n) {
    const size_type n_buckets = next_size(n);
    buckets.reserve(n_buckets);
    buckets.insert(buckets.end(), n_buckets, (node*) 0);
    num_elements = 0;
  }

  size_type next_size(size_type n) const {
    return __startl_next_prime(n);
  }

  const unsigned long * lower_bound(const unsigned long * first, const unsigned long * last, unsigned long n) const{
    for (; first != last && *first < n; ++ first);
    return first;
  }

public:
  //bkt_num
  size_type bkt_num(const value_type& obj, size_t n) const {
    return bkt_num_key(get_key(obj), n);
  }

  size_type bkt_num(const value_type& obj) const {
    return bkt_num_key(get_key(obj));
  }

  size_type bkt_num_key(const key_type& key) const {
    return bkt_num_key(key, buckets.size());
  }

  size_type bkt_num_key(const key_type& key, size_t n) const {
    return hash(key) % n;
  }

};

template <class Key, class Value, class HashFcn, class ExtractKey, class EqualKey, template <class E> class Alloc>
void hashtable<Key, Value, HashFcn, ExtractKey, EqualKey, Alloc>::resize(typename hashtable<Key, Value, HashFcn, ExtractKey, EqualKey, Alloc>::size_type num_elements_hint) {
     const size_type old_n = buckets.size();
     if (num_elements_hint > old_n) {
        size_type n = next_size(num_elements_hint);
       if (n > old_n) {
         vector<node*, Alloc> tmp(n, ( node * ) 0);
         try {
           for (size_type bucket = 0; bucket < old_n; ++ bucket) {
             node* first = buckets[bucket];
             while (first) {
               size_type new_bucket = bkt_num(first->val, n);
               buckets[bucket] = first->next;
               first->next = tmp[new_bucket];
               tmp[new_bucket] = first;
               first = buckets[bucket];
             }
           }
           buckets.swap(tmp);
         }catch (...) {

         }

       }
     }
}

template <class Key, class Value, class HashFcn, class ExtractKey, class EqualKey, template <class E> class Alloc>
pair<typename hashtable<Key, Value, HashFcn, ExtractKey, EqualKey, Alloc>::iterator, bool> hashtable<Key, Value, HashFcn, ExtractKey, EqualKey, Alloc>::insert_unique_noresize(const typename hashtable<Key, Value, HashFcn, ExtractKey, EqualKey, Alloc>::value_type& obj) {
  const size_type n = bkt_num(obj);
  node * first = buckets[n];
  for (node* cur = first; cur; cur = cur->next)
    if (equals(get_key(cur->val), get_key(obj)))
      return pair<iterator, bool>(iterator(cur, this), false);
  node* tmp = new_node(obj);
  tmp->next = first;
  buckets[n] = tmp;
  ++num_elements;
  return pair<iterator, bool>(iterator(tmp, this), true);
}

template <class Key, class Value, class HashFcn, class ExtractKey, class EqualKey, template <class E> class Alloc>
typename hashtable<Key, Value, HashFcn, ExtractKey, EqualKey, Alloc>::iterator hashtable<Key, Value, HashFcn, ExtractKey, EqualKey, Alloc>::insert_equal_noresize(const value_type& obj) {
  const size_type n = bkt_num(obj);
  node* first = buckets[n];
  for (node* cur = first; cur; cur = cur->next)
    if (equals(get_key(cur->val), get_key(obj))) {
      node* tmp = new_node(obj);
      tmp->next = cur->next;
      cur->next = tmp;
      ++num_elements;
      return iterator(tmp, this);
    }
  //至此，没有发现重复的键值
  node* tmp = new_node(obj);
  tmp->next = first;
  buckets[n] = tmp;
  ++num_elements;
  return iterator(tmp, this);
}

template <class Key, class Value, class HashFcn, class ExtractKey, class EqualKey, template <class E> class Alloc>
void hashtable<Key, Value, HashFcn, ExtractKey, EqualKey, Alloc>::clear() {
  for (size_type i = 0; i < buckets.size(); ++i) {
    node * cur = buckets[i];
    while (cur != 0) {
      node* next = cur->next;
      delete_node(cur);
      cur = next;
    }
    buckets[i] = 0;
  }
  num_elements = 0;
}

template <class Key, class Value, class HashFcn, class ExtractKey, class EqualKey, template <class E> class Alloc>
void hashtable<Key, Value, HashFcn, ExtractKey, EqualKey, Alloc>::copy_from(const hashtable &ht) {
  buckets.clear();
  //如果己方空间大于对方，就不动，如果已方空间小于对方，就会增大
  buckets.reserve(ht.buckets.size());
  buckets.insert(buckets.end(), ht.buckets.size(), (node*) 0);
  try {
    for (size_type i = 0; i < ht.buckets.size(); ++i) {
      if (const node* cur = ht.buckets[i]) {
        node* copy = new_node(cur->val);
        buckets[i] = copy;
        //针对同一个bucket list,复制每一个节点
        for (node* next = cur->next; next; cur = next, next = cur->next) {
          copy->next = new_node(next->val);
          copy = copy->next;
        }
      }
    }
    num_elements = ht.num_elements;
  }
  catch (...) {
    clear();
  }

}

template <class Key, class Value, class HashFcn, class ExtractKey, class EqualKey, template <class E> class Alloc>
inline bool operator==(const hashtable<Key, Value, HashFcn, ExtractKey, EqualKey, Alloc>& hs1, const hashtable<Key, Value, HashFcn, ExtractKey, EqualKey, Alloc>& hs2) {
  return hs1.buckets == hs2.buckets && hs1.num_elements == hs2.num_elements ;
};
}
#endif //STARTL_STARTL_HASHTABLE_H
