//
// Created by simanstar on 16-9-19.
//

#ifndef STARTL_STARTL_ALGOBASE_H
#define STARTL_STARTL_ALGOBASE_H

#include "startl_iterator.h"
namespace startl {

template<class OutputIterator, class Size, class T>
OutputIterator fill_n(OutputIterator first, Size n, const T &value) {
  for (; n > 0; --n, ++first)
    *first = value;
  return first;
};

template<class T>
inline const T &max(const T &v1, const T &v2) {
  return v1 > v2 ? v1 : v2;
}

template <class Iterator, class value_type>
Iterator find(Iterator begin, const Iterator& end, const value_type& v ) {
  while (begin != end) {
    if (*begin == v) break;
    begin++;
  }
  return begin;
}

template <class Iterator>//在已经初始化后的空间中使用
Iterator copy(Iterator begin_old, Iterator finish,Iterator begin_new) {
  if(begin_new == begin_old) return finish;
  else if (begin_new < begin_old || begin_new >= finish) {
    while (begin_old != finish) {
      *begin_new = *begin_old;
      ++begin_old;
      ++begin_new;
    }
    return begin_new;
  }
  else {  //新地址在区间中间
    Iterator new_finish = finish + (begin_new - begin_old);
    while (finish != begin_old) {
      *(--new_finish) = *(--finish);
    }
    return new_finish;
  }
}

template <class Iterator>//在已经初始化后的空间中使用
Iterator copy_backward(Iterator begin, Iterator finish_old,Iterator finish_new) {
  if (finish_new == finish_old) return begin;
  else if (finish_new > finish_old || finish_new <= begin) {
    while (finish_old != begin) {
      *(--finish_new) = *(--finish_old);
    }
    return begin;
  }
  else { //新地址在区间中间
    Iterator begin_new = begin + (finish_old - finish_new);
    while (begin != finish_old) {
      *begin_new = *begin;
      begin++;
      begin_new++;
    }
    return begin;
  }

}

template <class Iterator, class T>
void fill(Iterator first,const Iterator& last,const T& x) {
  for (; first != last; ++first)
    *first = x;
}

namespace __startl {
template <class T>
void swap(T &a, T &b) {
  T temp = a;
  a = b;
  b = temp;
}
}



}
#endif //STARTL_STARTL_ALGOBASE_H
