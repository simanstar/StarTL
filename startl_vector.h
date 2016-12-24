//
// Created by simanstar on 16-9-19.
//

#ifndef STARTL_STARTL_VECTOR_H
#define STARTL_STARTL_VECTOR_H

#include "memory.h"
#include "startl_algobase.h"
#include "startl_iterator.h"
#include "startl_uninitialized.h"

namespace startl {
template <class T, template <class E> class Alloc = Allocator >
class vector {
public:
  typedef T value_type;
  typedef value_type * pointer;
  typedef T * iterator;
  typedef value_type & reference;
  typedef const value_type & const_reference;
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;
  typedef Alloc<T> value_allocator;
private:
  iterator start;         //表示目前使用空间的头
  iterator finish;       //表示目前使用空间的尾
  iterator end_of_storage;//表示目前可用的尾

  void insert_aux(iterator position, const T& x);
  void deallocate() {
    if (start)
      value_allocator::deallocate(start, end_of_storage - start);
  }
  void fill_initialize(size_type n, const T& value) {
    start = allocate_and_fill(n, value);
    finish = start + n;
    end_of_storage = finish;
  }

public:
  iterator begin() const {return start;}
  iterator end() const {return finish;}

  size_type size() const {
    return size_type (end() - begin());
  }

  size_type capacity() const {
    return size_type (end_of_storage - begin());
  }

  bool empty() const {return begin() == end();}

  reference operator[](size_type n ) {
    return *(begin() + n);
  }

  const_reference operator[](size_type n ) const {
    return *(begin() + n);
  }

  reference operator=(const vector& another) {
    clear();
    for (auto atr_itr = another.begin(); atr_itr != another.end(); ++atr_itr)
      push_back(*atr_itr);
  }

  template <class _T,  template <class E> class _Alloc>
  friend bool operator==(const vector&, const vector&);

  void insert(iterator position, size_type n, const T& x);
  vector() : start(nullptr), finish(nullptr), end_of_storage(nullptr) {}
  vector(size_type n, const T& value) {fill_initialize(n, value);}
  //以下为什么有多个版本，不会转化？
  vector(int n, const T& value) {fill_initialize(n, value);}
  vector(long n, const T& value) {fill_initialize(n, value);}
  explicit vector(size_type n) {fill_initialize(n, T());}

  ~vector() {
    destroy(start, finish);
    deallocate();
  }
  reference front() {return *begin();}
  reference back() {return *(end() - 1);}

  void push_back(const T& x) {
    if (finish != end_of_storage) {
      construct(finish, x);
      ++finish;
    } else {
      insert_aux(end(), x);
    }
  }

  void pop_back() {
    --finish;
    destroy(finish);
  }

  iterator erase(iterator position) {
    if (position + 1 != end())
      copy(position + 1, finish, position);
    --finish;
    destroy(finish);
    return position;
  }

  iterator erase(iterator pos_begin, iterator pos_finish) {
    if (pos_finish != end())
      copy(pos_finish, finish, pos_begin);
      iterator old_finish = finish;
      finish = finish - (pos_finish - pos_begin);
    destroy(finish, old_finish);
    return pos_begin;
  }

  void resize(size_type new_size, const T&x) {
    if (new_size < size())
      erase(begin() + new_size, end());
    else
      insert(end(), new_size - size(), x);
  }
  void resize(size_type new_size) {resize(new_size, T());}
  void clear() {erase(begin(), end());}
  void swap(vector& atr) {
    vector temp = atr;
    atr = *this;
    *this = temp;
  }
  void reserve(size_type n) {
    //以配置新的vector空间
    if (size_type(end_of_storage - start) >= n) //已有足够的空间
      return;
    iterator new_start = value_allocator::allocate(n);
    iterator new_finish = new_start;
    try {
      new_finish = uninitialized_copy(start, finish, new_start);;
    } catch (...) {
      destroy(new_start, new_finish);
      value_allocator::deallocate(new_start, n);
      throw;
    }
    destroy(start, finish);
    deallocate();
    start = new_start;
    finish = new_finish;
    end_of_storage = new_start + n;
  }

protected:
  iterator allocate_and_fill(size_type n, const T& x) {
    iterator result = value_allocator::allocate(n);
    uninitialized_fill_n(result, n, x);
    return result;
  }
};

template <class T, template <class T> class Alloc>
void vector<T, Alloc>::insert_aux(iterator position, const T &x) {
  if (finish != end_of_storage) {     //还有备用空间
    construct(finish, *(finish - 1));
    ++finish;
    T x_copy = x;
    copy_backward(position, finish - 2, finish - 1);
    *position = x_copy;
  }
  else {      //已无备用空间
    const size_type old_size = size();
    const size_type len = old_size != 0? 2 * old_size : 1;
    iterator new_start = value_allocator::allocate(len);
    iterator new_finish = new_start;
    try {
      //原vector的内容拷贝到新vector
      new_finish = uninitialized_copy(start, position, new_start);
      construct(new_finish, x);
      ++new_finish;
      new_finish = uninitialized_copy(position, finish, new_finish);
    } catch (...) {
      //rollback
      destroy(new_start, new_finish);
      value_allocator::deallocate(new_start, len);
      throw ;
    }
    destroy(begin(), end());
    deallocate();
    //调整迭代器
    start = new_start;
    finish = new_finish;
    end_of_storage = new_start + len;

  }
}

template <class T, template <class T> class Alloc>
void vector<T, Alloc>::insert(iterator position, const size_type n, const T& x) {
  if (n != 0) {
    if (size_type(end_of_storage - finish) >= n) {
      //备用空间大于等于　“新增元素个数”
      T x_copy = x;
      //计算插入点后面的现有元素个数
      const size_type elems_after = finish - position;
      iterator old_finish = finish;
      if (elems_after > n) {
        //插入点后面元素个数　> 新增元素个数
        uninitialized_copy(finish - n, finish, finish);
        finish += n;
        copy_backward(position, old_finish - n, old_finish);
        fill(position, position + n, x_copy);
      }
      else {
        //插入点后面元素个数　< 新增元素个数
        uninitialized_fill_n(finish, n - elems_after, x_copy);
        finish += n - elems_after;
        uninitialized_copy(position, old_finish, finish);
        finish += elems_after;
        fill(position, old_finish, x_copy);
      }
    } else {
      //备用空间小于新增元素个数
      const size_type old_size = size();
      const size_type len = old_size + max(old_size, n);
      //以配置新的vector空间
      iterator new_start = value_allocator::allocate(len);
      iterator new_finish = new_start;
      try {
        new_finish = uninitialized_copy(start, position, new_start);
        new_finish = uninitialized_fill_n(new_finish, n, x);
        new_finish = uninitialized_copy(position, finish, new_finish);
      } catch (...) {
        destroy(new_start, new_finish);
        value_allocator::deallocate(new_start, len);
        throw;
      }
      destroy(start, finish);
      deallocate();
      start = new_start;
      finish = new_finish;
      end_of_storage = new_start + len;
    }
  }
}

template <class T, template <class E>  class Alloc>
bool operator==(const vector<T, Alloc>& v1, const vector<T, Alloc>& v2) {
  bool is_equal = false;
  auto v1_first = v1.begin();
  auto v2_first = v2.begin();
  for(;v1_first != v1.end() && v2_first != v2.end() && *v1_first == *v2_first;++v1_first, ++v2_first);
  if(v1_first == v1.end() && v2_first == v2.end())
    is_equal = true;

  return is_equal;
}

}
#endif //STARTL_STARTL_VECTOR_H
