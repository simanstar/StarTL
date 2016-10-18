//
// Created by simanstar on 16-10-18.
//

#include "startl_deque.h"
#include <iostream>
#include "startl_algobase.h"

using namespace startl;
using std::cout;
using std::endl;

int main() {
  deque<int> ideq(20, 9);
  cout << "size = " << ideq.size() << endl;

  for (int i = 0; i < ideq.size(); ++i)
    ideq[i] = i;

  for(int i = 0; i < ideq.size(); ++i)
    cout << ideq[i] << ' ';
  cout << endl;

  for (int i = 0; i < 3; ++i)
    ideq.push_back(i);

  for(int i = 0;i < ideq.size(); ++i)
    cout << ideq[i] << ' ';
  cout << endl;
  cout << "size = " << ideq.size() << endl;

  ideq.push_back(3);
  for(int i = 0;i < ideq.size(); ++i)
    cout << ideq[i] << ' ';
  cout << endl;
  cout << "size = " << ideq.size() << endl;

  ideq.push_front(99);
  for(int i = 0;i < ideq.size(); ++i)
    cout << ideq[i] << ' ';
  cout << endl;
  cout << "size = " << ideq.size() << endl;

  ideq.push_front(98);
  ideq.push_front(97);
  for(int i = 0;i < ideq.size(); ++i)
    cout << ideq[i] << ' ';
  cout << endl;
  cout << "size = " << ideq.size() << endl;

  deque<int>::iterator iter;
  iter = find(ideq.begin(), ideq.end(), 99);
  cout << *iter << endl;
  cout << *(iter.cur) << endl;

  ideq.erase(iter);
  for(int i = 0;i < ideq.size(); ++i)
    cout << ideq[i] << ' ';
  cout << endl;
  cout << "size = " << ideq.size() << endl;

  ideq.insert(iter, 666);
  for(int i = 0;i < ideq.size(); ++i)
    cout << ideq[i] << ' ';
  cout << endl;
  cout << "size = " << ideq.size() << endl;

  ideq.push_front(-1);
  ideq.push_front(-2);
  for(int i = 0;i < ideq.size(); ++i)
    cout << ideq[i] << ' ';
  cout << endl;
  cout << "size = " << ideq.size() << endl;

}