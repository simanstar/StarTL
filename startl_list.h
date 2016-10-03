//
// Created by simanstar on 16-9-29.
//

#ifndef STARTL_STARTL_LIST_H
#define STARTL_STARTL_LIST_H

#include "startl_iterator.h"
#include "startl_alloc.h"
#include "startl_algobase.h"

namespace startl {

template<class T>
struct __list_node {
  __list_node<T>* prev;
  __list_node<T>* next;
  T data;
};

template<class T>
class __list_iterator{
public:
  typedef bidirectional_iterator_tag iterator_category;
  typedef T value_type;
  typedef ptrdiff_t difference_type;
  typedef value_type* pointer;
  typedef value_type& reference;
  typedef size_t size_type;

  typedef __list_node<T>* link_type;
  typedef __list_iterator<T> self_type;
  link_type node;     //指向list节点

  __list_iterator(link_type x) : node(x) {}
  __list_iterator() {}
  __list_iterator(const self_type& x) : node(x.node) {}

  bool operator==(const self_type& x) const {return node == x.node;}
  bool operator!=(const self_type& x) const {return node != x.node;}
  reference operator*() const {return (*node).data;}
  pointer operator->() const {return &(operator*());}
  self_type& operator++() {
    node = (link_type)((*node).next);
    return *this;
  }
  self_type operator++(int) {
    self_type tmp = *this;
    ++*this;
    return tmp;
  }
  self_type& operator--() {
    node = (link_type)((*node).prev);
    return *this;
  }
  self_type operator--(int) {
    self_type tmp = *this;
    --*this;
    return tmp;
  }
};

template <class T, template <class E> class Alloc = Allocator >
class list {
protected:
  typedef __list_node<T> list_node;
  typedef Alloc<list_node> alloc_type;

public:
  typedef list_node* link_type;
  typedef __list_iterator<T> iterator;

  typedef T value_type;
  typedef value_type * pointer;
  typedef value_type & reference;
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;

protected:
  link_type node;   //指向尾端一个空白节点

public:
  list () { empty_initialize(); }
  iterator begin() const { return (link_type)((*node).next); }
  iterator end() const { return node; }
  bool empty() const { return node->next == node; }

  size_type size() const {
    return distance(begin(), end());  //ptrdiff_t转size_type
  }

  reference front() { return *begin(); }
  reference back() { return *(--end()); }
  link_type get_node() { return (link_type)alloc_type::allocate(1); }
  void put_node(link_type p) { alloc_type::deallocate(p, 1); }


  link_type create_node(const T& x) {
    link_type p = get_node();
    construct(&(p->data), x);
    return p;
  }

  void destroy_node(link_type p) {
    destroy(&(p->data));
    put_node(p);
  }

  iterator insert(iterator position, const T& x) {
    link_type tmp = create_node(x);//产生一个节点
    //调整双向指针，　使tmp插入进去
    tmp->next = position.node;
    tmp->prev = position.node->prev;
    (link_type(position.node->prev))->next = tmp;
    position.node->prev = tmp;
    return tmp;
  }

  void push_front(const T& x) { insert(begin(), x); }
  void push_back(const T& x) { insert(end(), x); }
  void clear();
  void remove(const T& value);
  void unique();
  void reverse();
  void sort();
  void merge(list& x);


  iterator erase(iterator position) {
    link_type next_node = link_type(position.node->next);
    link_type prev_node = link_type(position.node->prev);
    prev_node->next = next_node;
    next_node->prev = prev_node;
    destroy_node(position.node);
    return next_node;
  }

  void pop_front() { erase(begin()); }
  void pop_back() { erase(--end()); }
  //将x接合于position前，x必须不同于*this
  void splice(iterator position, list& x) {  //list不用模板参数？
    if (!x.empty())
      transfer(position, x.begin(), x.end());
  }
  //position和i可指向同一个list
  void splice(iterator position, list&, iterator i) {
    iterator j = i;
    ++j;
    if (position == i || position == j) return;
    transfer(position, i, j);
  }
  void splice(iterator position, list&, iterator first, iterator last) {
    if (first != last)
      transfer(position, first, last);
  }

protected:
  void empty_initialize() {
    node = get_node();
    node->next = node;
    node->prev = node;
  }

  void transfer(iterator position, iterator first, iterator last) {
    if (position != last) {
      (*(link_type((*last.node).prev))).next = position.node;
      (*(link_type((*first.node).prev))).next = last.node;
      (*(link_type((*position.node).prev))).next = first.node;
      link_type tmp = link_type((*position.node).prev);
      (*position.node).prev = (*last.node).prev;
      (*last.node).prev = (*first.node).prev;
      (*first.node).prev = tmp;
    }
  }

  void swap(list& other_list) {
    iterator old_begin = begin();
    transfer(begin(), other_list.begin(), other_list.end());
    transfer(other_list.end(), old_begin, end());
  }
};

template <class T, template <class E> class Alloc>
void list<T, Alloc>::clear() {
  link_type cur = (link_type) node->next;
  while (cur != node) {
    link_type tmp = cur;
    cur = (link_type) cur->next;
    destroy_node(tmp);
  }
  //恢复node原始状态
  node->next = node;
  node->prev = node;
}

template <class T, template <class E> class Alloc>
void list<T, Alloc>::remove(const T& value) {
  iterator first = begin();
  iterator last = end();
  while (first != last) {
    iterator next = first;
    ++next;
    if (*first == value) erase(first);
    first = next;
  }
}

template <class T, template <class E> class Alloc>
void list<T, Alloc>::unique() {
  iterator first = begin();
  iterator last = end();
  if (first == last) return;
  iterator next = first;
  while (++next != last) {
    if (*first == *next) erase(next);
    else first = next;
  next = first;
  }
}
//使用前提是两个list都已经过递增排序
template <class T, template <class E> class Alloc>
void list<T, Alloc>::merge(list<T, Alloc> &x) {
  iterator first1 = begin();
  iterator last1 = end();
  iterator first2 = x.begin();
  iterator last2 = x.end();
  while (first1 != last1 && first2 != last2) {
    if (*first2 < *first1) {
      iterator next = first2;
      transfer(first1, first2, ++next);
      first2 = next;
    } else ++first1;
    if (first2 != last2)
      transfer(last1, first2, last2);
  }
}

template <class T, template <class E> class Alloc>
void list<T, Alloc>::reverse() {
  if (node->next == node || node->next.next == node) //与siee() == 0 || size() ==1 比效率更高
    return;
  iterator first = begin();
  ++first;
  while (first != end()) {
    transfer(begin(), first++, first);
  }
}
//sort()还没理解
template <class T, template <class E> class Alloc>
void list<T, Alloc>::sort() {
  if (node->next == node || link_type(node->next)->next == node)
    return;
  list<T, Alloc> carry;
  list<T, Alloc> counter[64];
  int fill = 0;
  while (!empty()) {
    carry.splice(carry.begin(), *this, begin());
    int i = 0;
    while (i < fill && !counter[i].empty()) {
      counter[i].merge(carry);
      carry.swap(counter[i++]);
    }
    carry.swap(counter[i]);
    if (i == fill) ++fill;
  }
  for (int i = 1; i < fill; ++i)
    counter[i].merge(counter[i - 1]);
  swap(counter[fill - 1]);
}

}
#endif //STARTL_STARTL_LIST_H
