//
// Created by simanstar on 16-10-24.
//

#ifndef STARTL_STARTL_FUNCTION_H
#define STARTL_STARTL_FUNCTION_H

namespace startl {

template <class Arg, class Result>
struct unary_function {
  typedef Arg argument_type;
  typedef Result result_type;
};

template <class Arg1, class Arg2, class Result>
struct binary_function {
  typedef Arg1 first_argument_type;
  typedef Arg2 second_argument_type;
  typedef Result result_type;
};

template <class T>
struct identity : public unary_function<T, T> {
  const T& operator()(const T& x) const { return x; }
};

template <class T>
struct less : public binary_function<T, T, bool> {
  bool operator()(const T& x, const T& y) const {return x < y;}
};

template <class T>
struct select1st : public unary_function<T, typename T::first_type> {
  const typename T::first_type& operator()(const T& x) const { return x.first; }
};

}
#endif //STARTL_STARTL_FUNCTION_H
