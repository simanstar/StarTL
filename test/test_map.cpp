//
// Created by simanstar on 16-10-30.
//

#include "startl_map.h"
#include "startl_pair.h"
#include <iostream>
#include <startl_algobase.h>
#include <string>

using namespace startl;
using std::cout;
using std::endl;
using std::string;

int main() {
  map<string, int> simap;
  simap[string("jjhou")] = 1;
  simap[string("jerry")] = 2;
  simap[string("jason")] = 3;
  simap[string("jimmy")] = 4;

  /*
  pair<string, int> value(string("david"), 5);
  simap.insert(value);

  map<string, int>::iterator simap_iter = simap.begin();
  for (; simap_iter != simap.end(); ++simap_iter)
    cout << simap_iter->first << ' '
         << simap_iter->second << endl;

  int number = simap[string("jjhou")];
  cout << number << endl;

  map<string, int>::iterator ite1;
  ite1 = simap.find(string("mchen"));
  if (ite1 == simap.end())
    cout << "mchen not found" << endl;

  ite1 = simap.find(string("jerry"));
  if (ite1 != simap.end())
    cout << "jerry found" << endl;

  ite1->second = 9;
  int number2 = simap[string("jerry")];
  cout << number2 << endl;
  */
  map<string, int> simap2;
  simap2[string("jjhou")] = 1;
  simap2[string("jerry")] = 2;
  simap2[string("jason")] = 3;
  simap2[string("jimmy")] = 4;

  cout << (simap == simap2);
  return 0;
}