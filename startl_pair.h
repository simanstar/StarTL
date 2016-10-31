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

  first_type first;
  second_type second;

  pair() {}

  template <class U1, class U2>
  pair(const pair<U1, U2>& atr) : first(atr.first), second(atr.second) {}

  template <class U1, class U2>
  pair(const U1& a, const U2& b) : first(a), second(b) {}

  template <class U1, class U2>
  pair& operator=(const pair<U1, U2>& atr) {
    first = atr.first;
    second = atr.second;
    return *this;
  }

  template <class U1, class U2>
  bool operator==(const pair<U1, U2>& atr) {
    return first == atr.first && second == atr.second;
  }

};

template <class T1, class T2>
inline pair<T1, T2> make_pair(const T1& a, const T2& b) {
  return pair<T1, T2>(a, b);
}
}
#endif //STARTL_STARTL_PAIR_H
