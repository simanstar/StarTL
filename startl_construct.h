//
// Created by simanstar on 16-9-15.
//

#ifndef STARTL_STL_CONSTRUCT_H
#define STARTL_STL_CONSTRUCT_H
#include <new>
#include "type_traits.h"
#include "startl_iterator.h"

namespace startl {
template<class T1, class T2>
inline void construct(T1 *p, const T2 &value) {
  new(p) T1(value);
}

template <class T>
inline void destroy(T* pointer) {
  pointer->~T();
}

template <class ForwardIterator>
inline void __destroy_aux(ForwardIterator first, ForwardIterator last, __false_type) {
  for (; first < last; ++first)
    destroy(&*first);
}

template <class ForwardIterator>
inline void __destroy_aux(ForwardIterator first, ForwardIterator last, __true_type) {
}

template <class ForwardIterator, class T>
inline void __destroy(ForwardIterator first, ForwardIterator last, T *) {
  typedef typename __type_traits<T>::has_trivial_destructor trivial_destructor;
  __destroy_aux(first, last, trivial_destructor());
}

template <class ForwardIterator>
inline void destroy(ForwardIterator first, ForwardIterator last) {
  __destroy(first, last, value_type(first));
}

//以下两个不懂
template <>
inline void destroy<char*>(char*, char*) {
}

template <>
inline void destroy<wchar_t*>(wchar_t*, wchar_t*) {
}
}
#endif //STARTL_STL_CONSTRUCT_H
