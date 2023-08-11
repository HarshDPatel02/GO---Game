#include "Search.h"
#include <cassert>
#include <iostream>
#include <string>

using namespace std;

int linearSearch(const SortableMove data_in[], int count_in,const SortableMove &value_in) {

  for (int i = 0; i < count_in; i++) {
    if (data_in[i] == value_in) {
      return i;
    }
  }
  return VALUE_NOT_FOUND;
}

int unsortedFindSmallest(const SortableMove data_in[], int count_in) {
  SortableMove a = data_in[0];
  int temp = 0;
  for (int i = 0; i < count_in; i++)

  {

    if (data_in[i] < a) {
      a = data_in[i];
      temp = i;
    }
  }
  return temp;
}

int unsortedFindLargest(const SortableMove data_in[], int count_in) {
  SortableMove b = data_in[0];
  int temp = 0;
  for (int i = 0; i < count_in; i++) {

    if (b < data_in[i]) {
      b = data_in[i];
      temp = i;
    }
  }
  return temp;
}

void mySort(SortableMove data_in[], int count_in) {
  int i, j, search_min;
 SortableMove temp;
  // selection sorting method is use over here :
  for (i = 0; i < count_in; i++) {
    search_min = i;
    for (j = i + 1; j < count_in; j++)
      if (data_in[j] < data_in[search_min])
        search_min = j;

    temp = data_in[search_min];
    data_in[search_min] = data_in[i];
    data_in[i] = temp;
  }
}

bool isSorted(const SortableMove data_in[], int count_in) {
  int i;
  if (count_in <= 1) {
    return true;
  }
  for (i = 1; i < count_in; ++i) {
    if (data_in[i] < data_in[i - 1]) {
      return false;
    }
  }

  return true;
}

int sortedFindSmallest(const SortableMove data_in[], int count_in){
  assert(isSorted(data_in, count_in));

  assert(count_in > 0); // precondition:
  return 0;
}

int sortedFindLargest(const SortableMove data_in[], int count_in) {
  assert(count_in > 0); // precondition
  assert(isSorted(data_in, count_in));
  return count_in - 1;
}

int binarySearch(const SortableMove data_in[], int count_in, const SortableMove &value_in) {
  assert(isSorted(data_in, count_in));
  int l = 0;
  int h = count_in - 1;

  while (l <= h) {
    int mid = (l + h) / 2;
    if (value_in == data_in[mid]) {
      return mid;
    } else if (value_in < data_in[mid]) {
      h = mid - 1;
    } else {
      l = mid + 1;
    }
  }
  return VALUE_NOT_FOUND;
}

int binarySearchFirst(const SortableMove data_in[], int count_in, const SortableMove &value_in) {
 assert(isSorted(data_in, count_in));

	// using semi-open interval [low, high)
	int low  = 0;
	int high = count_in;

	while(low < high)  // loop also return out if value found
	{
		int mid = (high + low) / 2;
		if(data_in[mid] == value_in &&
		   (mid < 1 || data_in[mid - 1] <  value_in))
		{
			return mid;  // return out of loop
		}
		else if(data_in[mid] < value_in)
			low = mid + 1;
		else // data[mid] > value
			high = mid;
	}

	// value not in array
	return VALUE_NOT_FOUND;
  }