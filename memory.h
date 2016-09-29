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
//空间配置器接口,提供给容器调用,不用继承用类型参数？
/*template <class T, class Alloc>
class shell_alloc {
  typedef T value_type;
  typedef T* pointer;
  typedef const T* const_pointer;
  typedef T& reference;
  typedef const T& const_reference;
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;

  shell_alloc(const shell_alloc&);
  template <class U>
  shell_alloc(const shell_alloc<U>&);
  pointer address(reference x) const;
  const_pointer address(const_reference x) const;
  pointer allocate(size_type n, const void* = 0);
  void deallocate(pointer p, size_type n);
  size_type max_size() const;
  void construct(pointer p, const const_reference x);
  void destroy(pointer p);
};*/
//空间配置器具体实现
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

  Allocator(){}

  Allocator(const Allocator &);

  template<class U>
  Allocator(const Allocator<U> &);

  pointer address(reference x) const;

  const_pointer address(const_reference x) const;

  pointer allocate(size_type n, const void * = 0);

  void deallocate(pointer p, size_type n);

  size_type max_size() const;

 // void construct(pointer p, const const_reference x);

 // void destroy(pointer p);
};

template<class T>
inline typename Allocator<T>::pointer Allocator<T>::address(typename Allocator<T>::reference x) const {
  return &x;
}

//非const转const指针?????怎么转
template<class T>
inline typename Allocator<T>::const_pointer Allocator<T>::address(typename Allocator<T>::const_reference x) const {
  return &x;
}
//以下实现全部按n的大小分为一级和二级
template<class T>
typename Allocator<T>::pointer Allocator<T>::allocate(typename Allocator<T>::size_type n, const void *) {
  auto need_bytes = n * sizeof(T);
  if (need_bytes > 128) return (pointer) __first_level_alloc<0>::allocate(need_bytes);
  else return (pointer) __second_level_alloc<0, 0>::allocate(need_bytes);
}

template<class T>
void Allocator<T>::deallocate(typename Allocator<T>::pointer p, typename Allocator<T>::size_type n) {
  auto need_bytes = n * sizeof(T);
  if (need_bytes > 128) return __first_level_alloc<0>::deallocate(p);
  else return __second_level_alloc<0, 0>::deallocate(p, need_bytes);
}
}

#endif //STARTL_MEMORY_H
