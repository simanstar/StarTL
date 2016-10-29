//
// Created by simanstar on 16-10-25.
//

#ifndef STARTL_STARTL_PAIR_H
#define STARTL_STARTL_PAIR_H

namespace startl {

template <class T1, class T2>
struct pair {
  typedef T1 first_type;
  typedef T2 second_type;

  first_type first_value;
  second_type second_value;

  pair() {}

  template <class U1, class U2>
  pair(const pair<U1, U2>& atr) : first_value(atr.first_value), second_value(atr.second_value) {}

  template <class U1, class U2>
  pair(const U1& a, const U2& b) : first_value(a), second_value(b) {}
  //返回值语法有问题吗？
  template <class U1, class U2>
  pair& operator=(const pair<U1, U2>& atr) {
    first_value = atr.first_value;
    second_value = atr.second_value;
    return *this;
  }

  template <class U1, class U2>
  bool operator==(const pair<U1, U2>& atr) {
    return first_value == atr.first_value && second_value == atr.second_value;
  }

};

template <class T1, class T2>
inline pair<T1, T2> make_pair(const T1& a, const T2& b) {
  return pair<T1, T2>(a, b);
}
}
#endif //STARTL_STARTL_PAIR_H
