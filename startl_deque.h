//
// Created by simanstar on 16-10-10.
//

#ifndef STARTL_STARTL_DEQUE_H
#define STARTL_STARTL_DEQUE_H

#include "memory.h"
#include "startl_iterator.h"
#include "startl_uninitialized.h"
namespace startl {
template<class T, size_t BufSiz>
class __deque_iterator {
public:
  static size_t buffer_size()  { return __deque_buf_size(BufSiz, sizeof(T)); }           //原来是static，为什么要static???因为告诉编译器这不会修改对象的成员？

  typedef random_access_iterator_tag iterator_category;
  typedef T value_type;
  typedef ptrdiff_t difference_type;
  typedef value_type *pointer;
  typedef value_type &reference;
  typedef size_t size_type;

  typedef T** map_pointer;
  typedef __deque_iterator self;
  //保持与容器的联结
  T* cur;
  T* first;
  T* last;
  map_pointer node;     //指向管控中心

  static size_t __deque_buf_size(size_t n, size_t sz) {
    return n != 0 ? n : (sz < 512 ? size_t(512 / sz) : size_t(1));
  }

  void set_node(map_pointer new_node) {
    node = new_node;
    first = *new_node;
    last = first + difference_type(buffer_size());
  }

  reference operator*() const {return *cur;}
  pointer operator->() const {return &(operator*());}
  //x一定要在前面？
  difference_type operator-(const self& x) const {
    return difference_type (buffer_size()) * (node - x.node - 1) + (cur - first) + (x.last - x.cur);
  }

  self& operator++() {
    ++cur;
    if (cur == last) {
      set_node(node + 1);
      cur = first;
    }
    return *this;
  }

  self operator++(int) {
    self tmp = *this;
    ++*this;
    return tmp;
  }

  self& operator--() {
    if (cur == first) {
      set_node(node - 1);
      cur = last;
    }
    --cur;
    return *this;
  }

  self operator--(int) {
    self tmp = *this;
    --*this;
    return tmp;
  }

  self& operator+=(difference_type n) {
    difference_type offset = n + (cur - first);
    if (offset >= 0 && offset < difference_type(buffer_size())) //目标位置在同一缓冲区内
      cur += n;
    else {
      //标的位置不在同一缓冲区内
      difference_type node_offset = offset > 0 ? offset / difference_type(buffer_size())
                                              : -difference_type ((-offset - 1) / buffer_size()) - 1;
      //切换至正确的节点
      set_node(node + node_offset);
      //切换至正确的元素
      cur = first + (offset - node_offset * difference_type(buffer_size()));
    }
    return *this;
  }

  self operator+(difference_type n) const {
    self tmp = *this;
    return tmp += n;
  }

  self& operator-=(difference_type n) { return *this += -n; }
  self operator-(difference_type n) const {
    self tmp = *this;
    return tmp -= n;
  }
  reference operator[](difference_type n) const {return *(*this + n);}

  bool operator==(const self& x) const {return cur == x.cur;}
  bool operator!=(const self& x) const {return !(*this == x);}
  bool operator<(const self& x) const {
    return (node == x.node) ? (cur < x.cur) : (node < x.node);
  }

  bool operator>(const self& x) const {
    return x < *this;
  }

  bool operator<=(const self& x) const {
    return operator<(x) || operator==(x) ;
  }

  bool operator>=(const self& x) const {
    return x <= *this;
  }


};

template<class T, template<class E> class Alloc = Allocator, size_t BufSiz = 32>
class deque {
public:
  typedef T value_type;
  typedef value_type *pointer;
  typedef value_type &reference;
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;
  typedef __deque_iterator<T, BufSiz> iterator;
protected:
  typedef pointer *map_pointer;
  typedef Alloc <value_type > data_allocator;
  typedef Alloc <pointer > map_allocator;
protected:
  iterator start;
  iterator finish;
  map_pointer map;
  size_type map_size;
public:
  deque(int n, const value_type& value)
      : start(), finish(), map(0), map_size(0) {
    fill_initialize(n, value);
  }
  iterator begin() { return start; }
  iterator end() { return finish; }
  reference operator[](size_type n) {
    return start[difference_type(n)];
  }
  reference front() {return *start;}
  reference back() {
    iterator tmp = finish;
    --tmp;
    return *tmp;
  }

  void push_back(const value_type& t) {
    if (finish.cur != finish.last - 1) {
      construct(finish.cur, t);     //直接在备用空间上构造元素
      ++finish.cur;                 //调整最后缓冲区的使用状态
    }
    else      //最后缓冲区无元素备用空间
      push_back_aux(t);
  }

  void push_front(const value_type& t) {
    if (start.cur != start.first) {       //第一缓冲区尚有备用空间
      construct(start.cur - 1, t);
      --start.cur;
    }
    else      //已无备用空间
      push_front_aux(t);
  }

  void pop_back() {
    if (finish.cur != finish.first) {
      //最后缓冲区有一个或多个元素
      --finish.cur;
      destroy(finish.cur);
    }
    else
      //最后缓冲区没有任何元素
    pop_back_aux();                     //进行缓冲区的释放工作
  }
   //在deque头，cur指向第一个元素,在deque尾,cur指向最后一个元素＋１
  void pop_front() {
    if (start.cur != start.last - 1) {
      //第一缓冲区有两个或多个元素
      destroy(start.cur);
      ++start.cur;
    }
    else
      //缓冲区仅有一个元素
      pop_front_aux();
  }

  size_type size() const {return finish -start;}
  size_type max_size() const {return size_type (-1);}       //为什么是-1???
  bool empty() const {return finish == start;}
  void clear();
  iterator erase(iterator pos);
  iterator erase(iterator first, iterator last);
  iterator insert(iterator position, const value_type& x) {
    if (position.cur == start.cur) {                        //插入点是deque最前端,调用push_front
      push_front(x);
      return start;
    }
    else if (position.cur == finish.cur) {                  //插入点是deque最尾端，调用push_back
      push_back(x);
      iterator tmp = finish;
      --tmp;
      return tmp;
    }
    else
      return insert_aux(position, x);
  }

private:
  void fill_initialize(size_type n, const value_type& value);
  void create_map_and_nodes(size_type num_elements);
  void push_back_aux(const value_type& t);
  void push_front_aux(const value_type& t);
  void reserve_map_at_back(size_type nodes_to_add = 1) {
    if (nodes_to_add + 1 > map_size - (finish.node - map))
      //如果map尾端的节点备用空间不足
      reallocate_map(nodes_to_add, false);
  }

  void reserve_map_at_front(size_type nodes_to_add = 1) {
    if (nodes_to_add > start.node - map)
      //如果map前端的节点备用空间不足
      reallocate_map(nodes_to_add, true);
  }
  void reallocate_map(size_type nodes_to_add, bool add_at_front);
  void pop_back_aux();
  void pop_front_aux();
  iterator insert_aux(iterator pos, const value_type& x);
  size_type buffer_size() const { return BufSiz ; }
  size_type initial_map_size() const { return map_size;}
  pointer allocate_node() { return data_allocator::allocate(BufSiz);}
  void deallocate_node(map_pointer pos) { return data_allocator::deallocate(*pos, BufSiz); }

};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<class T, template<class E> class Alloc , size_t BufSiz >
void deque<T, Alloc, BufSiz>::fill_initialize(size_type n, const value_type& value) {
  create_map_and_nodes(n);      //把deque的结构都产生并安排好
  map_pointer cur;
  try {
    //为每个节点的缓冲区设定初值
    for (cur = start.node; cur < finish.node; ++cur)
      uninitialized_fill(*cur, *cur + buffer_size(), value);
    //最后节点尾端有备用空间，不必设初值
    uninitialized_fill(finish.first, finish.cur, value);
  }
  catch (...) {

  }
}

template<class T, template<class E> class Alloc , size_t BufSiz >
void deque<T, Alloc, BufSiz>::create_map_and_nodes(size_type num_elements) {
  size_type num_nodes = num_elements / buffer_size() + 1;
  map_size = max(initial_map_size(), num_nodes + 2);
  map = map_allocator::allocate(map_size);
  //令nstart和nfinish指向map所拥有之全部节点的最中央区段
  //保持在最中央，可使头尾两端的扩充能量一样大。
  map_pointer nstart = map + (map_size - num_nodes) / 2 ;
  map_pointer nfinish = nstart + num_nodes - 1;
  map_pointer cur;
  try {
    for (cur = nstart; cur <= nfinish; ++cur)
      *cur = allocate_node();
  }
  catch (...) {
    //commit or rollback
  }
  //为deque内的两个迭代器start和end设定正确内容
  start.set_node(nstart);
  finish.set_node(nfinish);
  start.cur = start.first;
  finish.cur = finish.first + num_elements % buffer_size();
}

template<class T, template<class E> class Alloc , size_t BufSiz >
void deque<T, Alloc, BufSiz>::push_back_aux(const value_type &t) {
  value_type t_copy = t;
  reserve_map_at_back();          //若符合某种条件则必须重换一个map
  *(finish.node + 1) = allocate_node();
  try {
    construct(finish.cur, t_copy);
    finish.set_node(finish.node + 1);
    finish.cur = finish.first;
  }
  catch (...) {
    //commit or rollback
  }
}

template<class T, template<class E> class Alloc , size_t BufSiz >
void deque<T, Alloc, BufSiz>::push_front_aux(const value_type &t) {
  value_type t_copy = t;
  reserve_map_at_front();
  *(start.node - 1) = allocate_node();
  try {
    start.set_node(start.node - 1);
    start.cur = start.last - 1;
    construct(start.cur, t_copy);
  }
  catch (...) {
    //commit or rollback
    start.set_node(start.node + 1);
    start.cur = start.first;
    deallocate_node(start.node - 1);
    throw;
  }
}

template<class T, template<class E> class Alloc , size_t BufSiz >
void deque<T, Alloc, BufSiz>::reallocate_map(size_type nodes_to_add, bool add_at_front) {
  size_type old_num_nodes = finish.node - start.node + 1;
  size_type new_num_nodes = old_num_nodes + nodes_to_add;
  map_pointer new_nstart;
  if (map_size > 2 * new_num_nodes) {
    new_nstart = map + (map_size - new_num_nodes) / 2
                  + (add_at_front ? nodes_to_add : 0);                 //为false为什么不减
    if (new_nstart < start.node)
      copy(start.node, finish.node + 1, new_nstart + old_num_nodes);
    else
      copy_backward(start.node, finish.node + 1, new_nstart + old_num_nodes);
  }
  else {
    size_type new_map_size = map_size + max(map_size, nodes_to_add) + 2;
    //配置一块空间，准备给新map使用
    map_pointer new_map = map_allocator::allocate(new_map_size);
    new_nstart = new_map + (new_map_size - new_num_nodes) / 2         //为false为什么不减
                  + (add_at_front ? nodes_to_add : 0);
    //把原map内容拷贝过来
    copy(start.node, finish.node + 1, new_nstart);
    map_allocator::deallocate(map, map_size);
    //设定新map的起始地址与大小
    map = new_map;
    map_size = new_map_size;
  }
  //重新设定迭代器start和finish
  start.set_node(new_nstart);
  finish.set_node(new_nstart + old_num_nodes - 1);
}

template<class T, template<class E> class Alloc , size_t BufSiz >
void deque<T, Alloc, BufSiz>::pop_back_aux() {
  deallocate_node(finish.node - 1);         //释放最后一个缓冲区
  finish.set_node(finish.node - 1);
  finish.cur = finish.last - 1;
  destroy(finish.cur);
}

template<class T, template<class E> class Alloc , size_t BufSiz >
void deque<T, Alloc, BufSiz>::pop_front_aux() {
  destroy(start.cur);
  deallocate_node(start.node);
  start.set_node(start.node + 1);
  start.cur = start.first;
}

template<class T, template<class E> class Alloc , size_t BufSiz >
void deque<T, Alloc, BufSiz>::clear() {
  //以下针对头尾以外的每个缓冲区
  for (map_pointer node = start.node + 1; node < finish.node; ++node) {
    //将缓冲区所有元素析构
    destroy(*node, *node + buffer_size());
    //释放缓冲区内存
    data_allocator::deallocate(*node, buffer_size());
  }
  if (start.node != finish.node) {                //至少有头尾两个缓冲区
    destroy(start.cur, start.last);
    destroy(finish.first, finish.cur);
    data_allocator::deallocate(finish.first, buffer_size());
  }
  else                                            //只有一个缓冲区
    destroy(start.cur, finish.cur);
  finish = start;   //调整状态

}

template<class T, template<class E> class Alloc , size_t BufSiz >
typename deque<T, Alloc, BufSiz>::iterator deque<T, Alloc, BufSiz>::erase(iterator pos) {
  iterator next = pos;
  ++next;
  difference_type index = pos - start;
  if (index < (size() >> 1)) {                    //如果清除点前的元素比较少
    copy_backward(start, pos, next);
    pop_front();
  }
  else {                                          //清除点后的元素比较少
    copy(next, finish, pos);
    pop_back();
  }
  return start + index;                           //??????为什么返回这个？
}

template<class T, template<class E> class Alloc , size_t BufSiz >
typename deque<T, Alloc, BufSiz>::iterator deque<T, Alloc, BufSiz>::erase(iterator first, iterator last) {
  if (first == start && last == finish) {                   //清除整个deque，直接调用clear()
    clear();
    return finish;
  }
  else {
    difference_type n = last - first;
    difference_type elems_before = first - start;
    if (elems_before < (size() - n) / 2) {          //前方的元素比较少
      copy_backward(start, first, last);
      iterator new_start = start + n;
      destroy(start, new_start);
      //将冗余的缓冲区释放
      for (map_pointer cur = start.node; cur < new_start.node; ++ cur)
        data_allocator::deallocate(*cur, buffer_size());
      start = new_start;
    } else {                                        //清除区间后方元素比较少
      copy(last, finish, first);
      iterator new_finish = finish - n;
      destroy(new_finish, finish);
      //将冗余缓冲区释放
      for (map_pointer cur = new_finish.node + 1; cur <= finish.node; ++cur)
        data_allocator::deallocate(*cur, buffer_size());
      finish = new_finish;
    }
    return start + elems_before;
  }
}

template<class T, template<class E> class Alloc , size_t BufSiz >
typename deque<T, Alloc, BufSiz>::iterator deque<T, Alloc, BufSiz>::insert_aux(iterator pos, const value_type &x) {
  difference_type index = pos - start;
  value_type x_copy = x;
  if (index < (size() >> 1)) {            //插入点前的元素个数比较少
    push_front(front());
    iterator front1 = start;
    ++front1;
    iterator front2 = front1;
    ++front2;
    pos = start + index;
    iterator pos1 = pos;
    ++pos1;
    copy(front2, pos1, front1);
  }
  else {
    push_back(back());
    iterator back1= finish;
    --back1;
    iterator back2 = back1;
    --back2;
    pos = start + index;
    copy_backward(pos, back2, back1);
  }
  *pos = x_copy;
  return pos;
}
}

#endif //STARTL_STARTL_DEQUE_H
