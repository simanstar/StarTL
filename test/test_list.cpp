//
// Created by simanstar on 16-10-2.
//
#include "memory.h"
#include <iostream>
#include "startl_alloc.h"
#include "startl_uninitialized.h"
#include "startl_list.h"
using namespace startl;
using std::cout;
using std::endl;
int main() {

  list<int> iv;
  cout << "size = " << iv.size() << endl;

  iv.push_back(1);
  cout << "size = " << iv.size() << endl;

  iv.push_back(2);
  cout << "size = " << iv.size() << endl;

  iv.push_back(5);
  cout << "size = " << iv.size() << endl;

  iv.push_back(8);
  cout << "size = " << iv.size() << endl;

  list<int> iv2;
  iv2.push_back(3);
  iv2.push_back(6);
  iv2.push_back(7);
  iv2.push_back(9);
  //iv.merge(iv2);
  //iv.swap(iv2);
  //iv.sort();
  for (auto iter : iv2)
    cout << iter << " ";
  cout << endl;

  iv.push_back(5);
  cout << "size = " << iv.size() << endl;

  for (auto iter : iv)
    cout << iter << " ";
  cout << endl;

  iv.pop_back();
  iv.pop_back();
  cout << "size = " << iv.size() << endl;

  iv.pop_back();
  cout << "size = " << iv.size() << endl;

  list<int>::iterator ivite = find(iv.begin(), iv.end(), 1);
  if (ivite != iv.end() ) iv.erase(ivite);
  cout << "size = " << iv.size() << endl;

  for (auto iter : iv)
    cout << iter << " ";
  cout << endl;

  auto ite = find(iv.begin(), iv.end(), 2);
  if (ite != iv.end()) iv.insert(ite, 7);

  cout << "size = " << iv.size() << endl;

  for (auto iter : iv)
    cout << iter << " ";
  cout << endl;

  iv.clear();
  cout << "size = " << iv.size() << endl;
  return 0;
}
