//
// Created by simanstar on 16-9-13.
//

#ifndef STARTL_STARTL_UNINITIALIZED_H
#define STARTL_STARTL_UNINITIALIZED_H

#include "startl_construct.h"
#include "startl_iterator.h"
#include "type_traits.h"
#include <string.h>
#include "startl_algobase.h"

namespace startl {
//////////uninitialized_fill_n
//函数重载效率高，在编译期确定执行哪个函数
template<class ForwardIterator, class Size, class T>
inline ForwardIterator __uninitialized_fill_n_aux(ForwardIterator first, Size n, const T &x, __true_type) {
  return fill_n(first, n, x);
}

template<class ForwardIterator, class Size, class T>
inline ForwardIterator __uninitialized_fill_n_aux(ForwardIterator first, Size n, const T &x, __false_type) {
  ForwardIterator cur = first;
  for (; n > 0; --n, ++cur)
    construct(&*cur, x);
  return cur;
}

template<class ForwardIterator, class Size, class T, class T1>
inline ForwardIterator __uninitialized_fill_n(ForwardIterator first, Size n, const T &x, T1 *) {
  typedef typename __type_traits<T1>::is_POD_type is_POD;
  return __uninitialized_fill_n_aux(first, n, x, is_POD());
}

template<class ForwardIterator, class Size, class T>
inline ForwardIterator uninitialized_fill_n(ForwardIterator first, Size n, const T &x) {
  return __uninitialized_fill_n(first, n, x, value_type(first));
}



///////////////uninitialized_copy
template<class InputIterator, class ForwardIterator>
inline ForwardIterator __uninitialized_copy_aux(InputIterator first, InputIterator last, ForwardIterator result, __false_type) {
  ForwardIterator cur = result;
  for (; first != last; ++first, ++cur)
    construct(&*cur, *first);
  return cur;
}

template<class InputIterator, class ForwardIterator>
inline ForwardIterator __uninitialized_copy_aux(InputIterator first, InputIterator last, ForwardIterator result, __true_type) {
  return copy(first, last, result);
}

template<class InputIterator, class ForwardIterator, class T>
inline ForwardIterator __uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result, T *) {
  typedef typename __type_traits<T>::is_POD_type is_POD;
  return __uninitialized_copy_aux(first, last, result, is_POD());
}

template<class InputIterator, class ForwardIterator>
inline ForwardIterator uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result) {
  return __uninitialized_copy(first, last, result, value_type(result));
}

//const char*特化版
inline char * uninitialized_copy(const char * first, const char * last, char * result) {
  memmove(result, first, last - first);
  return (result + (last - first));
}
//const wchar_t*特化版
inline wchar_t * uninitialized_copy(const wchar_t * first, const wchar_t * last, wchar_t * result) {
  memmove(result, first, sizeof(wchar_t) * (last - first));
  return (result + (last - first));
}

///////uninitialized fill
template <class ForwardIterator, class T>
inline void __uninitialized_fill_aux(ForwardIterator first, ForwardIterator last, const T& x, __true_type) {
  fill(first, last, x);
}

template <class ForwardIterator, class T>
inline void __uninitialized_fill_aux(ForwardIterator first, ForwardIterator last, const T& x, __false_type) {
  ForwardIterator cur = first;
  for (; cur != last; ++cur)
    construct(&*cur, x);
}

template <class ForwardIterator, class T, class T1>
inline void __uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& x, T1*) {
  typedef typename __type_traits<T1>::is_POD_type is_POD;
  __uninitialized_fill_aux(first, last, x, is_POD());
}

template <class ForwardIterator, class T>
inline void uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& x) {
  __uninitialized_fill(first, last, x, value_type(first));
}

}
#endif //STARTL_STARTL_UNINITIALIZED_H
