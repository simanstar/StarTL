//
// Created by simanstar on 16-12-19.
//

#include <iostream>
#include <cstring>
#include "startl_hashset.h"
#include "startl_function.h"
#include "startl_hashfun.h"
using std::cout;
using std::endl;
using namespace startl;

void lookup(const hash_set<const char*, hash<const char*>, equal_to<const char*> >& set, const char* word) {
  typename hash_set<const char*, hash<const char*>, equal_to<const char*> >::const_iterator it = set.find(word);
  cout << " " << word << " : " << (it != set.end() ? "present" : "not present") << endl;
}

void show(const hash_set<const char*, hash<const char*>, equal_to<const char*> >& set) {
  typename hash_set<const char*, hash<const char*>, equal_to<const char*> >::const_iterator it1 = set.begin();
  typename hash_set<const char*, hash<const char*>, equal_to<const char*> >::const_iterator it2 = set.end();
  for (; it1 != it2; ++ it1)
    cout << *it1 << ' ';
}

int main() {
  hash_set<const char*, hash<const char*>, equal_to<const char*> > set;
  set.insert("kiwi");
  set.insert("plum");
  set.insert("apple");
  set.insert("mango");
  set.insert("apricot");
  set.insert("banana");
  set.insert("ba");

  lookup(set, "mango");
  lookup(set, "apple");
  lookup(set, "durian");

  show(set);




}