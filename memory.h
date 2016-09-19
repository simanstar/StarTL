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

template <class T>
class Allocator {
public:
  typedef T value_type;
  typedef T* pointer;
  typedef const T* const_pointer;
  typedef T& reference;
  typedef const T& const_reference;
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;

  Allocator(const Allocator&);
  template <class U>
  Allocator(const Allocator<U>&);
  pointer address(reference x) const;
  const_pointer address(const_reference x) const;
  pointer allocate(size_type n, const void* = 0);
  void deallocate(pointer p, size_type n);
  size_type max_size() const;
  void construct(pointer p, const const_reference x);
  void destroy(pointer p);
};
}

#endif //STARTL_MEMORY_H
