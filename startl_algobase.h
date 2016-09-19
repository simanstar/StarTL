//
// Created by simanstar on 16-9-19.
//

#ifndef STARTL_STARTL_ALGOBASE_H
#define STARTL_STARTL_ALGOBASE_H
template <class OutputIterator, class Size, class T>
OutputIterator fill_n(OutputIterator first, Size n, const T& value) {
  for (; n >0 ; --n, ++ first)
    *first = value;
  return first;
};
#endif //STARTL_STARTL_ALGOBASE_H
