//
// Created by simanstar on 16-9-13.
//
#include "memory.h"
#include <iostream>
#include "startl_alloc.h"
#include "startl_uninitialized.h"
#include "startl_vector.h"
using namespace startl;
using std::cout;
using std::endl;
int main() {
  size_t i;
  vector<int> iv(2,9);
  //cout << vector(2,9).size() << endl;
  cout << "size = " << iv.size() << endl;
  cout << "capacity = " << iv.capacity() << endl;

  iv.push_back(1);
  cout << "size = " << iv.size() << endl;
  cout << "capacity = " << iv.capacity() << endl;

  iv.push_back(2);
  cout << "size = " << iv.size() << endl;
  cout << "capacity = " << iv.capacity() << endl;

  iv.push_back(3);
  cout << "size = " << iv.size() << endl;
  cout << "capacity = " << iv.capacity() << endl;

  iv.push_back(4);
  cout << "size = " << iv.size() << endl;
  cout << "capacity = " << iv.capacity() << endl;

  for (auto iter : iv)
    cout << iter << " ";
  cout << endl;

  iv.push_back(5);
  cout << "size = " << iv.size() << endl;
  cout << "capacity = " << iv.capacity() << endl;

  for (i = 0; i < iv.size(); ++i)//是否支持c++ auto遍历
    cout << iv[i] << " ";
  cout << endl;

  iv.pop_back();
  iv.pop_back();
  cout << "size = " << iv.size() << endl;
  cout << "capacity = " << iv.capacity() << endl;

  iv.pop_back();
  cout << "size = " << iv.size() << endl;
  cout << "capacity = " << iv.capacity() << endl;

  vector<int>::iterator ivite = find(iv.begin(), iv.end(), 1);
  if (ivite) iv.erase(ivite);
  cout << "size = " << iv.size() << endl;
  cout << "capacity = " << iv.capacity() << endl;
  for(i = 0; i < iv.size(); ++i)
    cout << iv[i] << " ";
  cout << endl;

  auto ite = find(iv.begin(), iv.end(), 2);
  if (ite) iv.insert(ite, 3, 7);

  cout << "size = " << iv.size() << endl;
  cout << "capacity = " << iv.capacity() << endl;
  for(i = 0; i < iv.size(); ++i)
    cout << iv[i] << " ";
  cout << endl;

  iv.clear();
  cout << "size = " << iv.size() << endl;
  cout << "capacity = " << iv.capacity() << endl;

  return 0;
}
