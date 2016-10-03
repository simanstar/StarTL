//
// Created by simanstar on 16-9-13.
//

#ifndef STARTL_MEMORY_H
#define STARTL_MEMORY_H

#include <new>
#include <cstddef>
#include <climits>
#include <iostream>
#include "startl_alloc.h"
#include "startl_construct.h"
namespace startl {
//空间配置外层接口
/*template <class T, class Alloc>
class shell_alloc {
public:
  typedef T value_type;
  typedef T *pointer;
  typedef const T *const_pointer;
  typedef T &reference;
  typedef const T &const_reference;
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;

  static pointer address(reference x) {return &x;}

  static const_pointer address(const_reference x);//非const转const指针?????怎么转
  //调用内层实现
  static pointer allocate(size_type n, const void * = 0) {
    Alloc::allocate
  }

  static void deallocate(pointer p, size_type n) {
    auto need_bytes = n * sizeof(T);
    if (need_bytes > 128) return __first_level_alloc<0>::deallocate(p);
    else return __second_level_alloc<0, 0>::deallocate(p, need_bytes);
  }

  static size_type max_size();
};*/
//空间配置器内层具体实现
template<class T>
class Allocator {
public:
  typedef T value_type;
  typedef T *pointer;
  typedef const T *const_pointer;
  typedef T &reference;
  typedef const T &const_reference;
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;

  static pointer address(reference x) {return &x;}

  static const_pointer address(const_reference x);//非const转const指针?????怎么转
  //以下实现全部按n的大小分为一级和二级
  static pointer allocate(size_type n, const void * = 0) {
    auto need_bytes = n * sizeof(T);
    if (need_bytes > 128) return (pointer) __first_level_alloc<0>::allocate(need_bytes);
    else return (pointer) __second_level_alloc<0, 0>::allocate(need_bytes);
  }

  static void deallocate(pointer p, size_type n) {
    auto need_bytes = n * sizeof(T);
    if (need_bytes > 128) return __first_level_alloc<0>::deallocate(p);
    else return __second_level_alloc<0, 0>::deallocate(p, need_bytes);
  }

  static size_type max_size();
};
}

#endif //STARTL_MEMORY_H
