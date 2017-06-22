//
// Created by simanstar on 17-4-22.
//
#include <iostream>
#include <time.h>
#include <vector>
#include <MessageHeader.h>
#include "OrderPush.h"
#include "oms_api.h"
#include <bitset>
#include <algorithm>
#include <stack>

using namespace std;
using namespace oms;

int firstMissingPositive(int* nums, int numsSize) {
  int one_loc = 0;
  while(one_loc < numsSize && nums[one_loc] != 1)
    ++one_loc;
  if (one_loc == numsSize || numsSize == 1)
    return 1;
  for (int i = one_loc ; i < numsSize ; ++i) {
    if (nums[i] == 1)
      nums[i] = 0;
  }

  int min_one_loc = 0;
  int next_loc = nums[0] - 1 ;
  nums[0] = 1;
  while (min_one_loc < numsSize ) {
    if (next_loc < numsSize && next_loc >=0 && nums[next_loc] != 1) {
      int temp = nums[next_loc];
      nums[next_loc] = 1;
      next_loc = temp - 1;
    }
    else {
      ++min_one_loc;
      next_loc =nums[min_one_loc] - 1;
    }
  }
  for (int i = 0; i < numsSize ; ++i) {
    if (nums[i] != 1)
      return i + 1;
  }
  return numsSize + 1;
}
int main() {
  vector<int> vec = {3,1,2,4,5,1,1};
  cout << firstMissingPositive(&vec[0], vec.size());

}