//
//  TestSearch1.cpp
//
//  A test program for the Search module.
//
//  This program is to be used with Assignment 1: Part E for
//    CS115, 202310.
//
//  Do not modify this file.
//

#include "Search.h"
#include "Search.h" // repeated to test for #include errors

#include <cassert>
#include <climits>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include "TestHelper.h"

using namespace std;

// this creates a type that can hold a search funciton
using SearchFunction = int(const string data[], int count, const string &value);
bool testSearch(SearchFunction search_function, const string &description,
                const string data[], unsigned int count,
                const string &search_for, unsigned int correct_index_first,
                unsigned int correct_index_last, bool must_be_first);

// this creates a type that can hold a find smallest/largest funciton
using FindExtremeFunction = int(const string data[], int count);
bool testFindExtreme(FindExtremeFunction find_extreme_function,
                     const string &description, const string data[],
                     unsigned int count, bool is_smallest,
                     unsigned int correct_index);

int calculateMark(bool correct_value_not_found,
                  unsigned int correct_linear_search,
                  unsigned int correct_unsorted_smallest,
                  unsigned int correct_unsorted_largest, bool is_finished);

const int COMPILE_AND_START_MARKS = 2;
const int DID_NOT_CRASH_RUNNING_MARKS = 3;
const int COMPILE_AND_NOT_CRASH_MARKS =
    COMPILE_AND_START_MARKS + DID_NOT_CRASH_RUNNING_MARKS;
const int COMBINED_TEST_MARKS = 10;
const int TOTAL_MARKS = COMPILE_AND_NOT_CRASH_MARKS + COMBINED_TEST_MARKS;

const int VALUE_NOT_FOUND_CORRECT = INT_MAX;

const string DESCRIPTION_EMPTY = "in an array with no data";
const string DESCRIPTION_MISSING = "in an array without it";
const string DESCRIPTION_ONLY = "as the only element";
const string DESCRIPTION_FIRST = "as the first element";
const string DESCRIPTION_MIDDLE = "in the middle of the array";
const string DESCRIPTION_LAST = "as the last element";
const string DESCRIPTION_DUP_ANY = "in an array with duplicates";
const string DESCRIPTION_DUP_FIRST = "as a duplicate at the start";
const string DESCRIPTION_DUP_MIDDLE = "as a duplicate in the middle";
const string DESCRIPTION_DUP_LAST = "as a duplicate at the end";

const unsigned int DATA_SEARCH_COUNT = 8;
const unsigned int DATA_FIND_EXTREME_COUNT = 6;

const string SINGLE_DATA_DESCRIPTION = "a very small array";
const unsigned int SINGLE_DATA_COUNT = 1;
const string SINGLE_DATA_UNSORTED[SINGLE_DATA_COUNT] = {
    "Hello World!",
};
const string *SINGLE_DATA_CORRECT =
    SINGLE_DATA_UNSORTED; // cheating: same array under new name
const string SINGLE_SEARCH_FOR = "Hello World!";
const unsigned int SINGLE_SEARCH_AT = 0;
const unsigned int SINGLE_SMALLEST_AT = 0;
const unsigned int SINGLE_LARGEST_AT = 0;

const string ORDERED_DATA_DESCRIPTION = "a pre-sorted array";
const unsigned int ORDERED_DATA_COUNT = 5;
const string ORDERED_DATA_UNSORTED[ORDERED_DATA_COUNT] = {
    "ant", "bat", "cat", "dog", "eel",
};
const string *ORDERED_DATA_CORRECT =
    ORDERED_DATA_UNSORTED; // cheating: same array under new name
const string ORDERED_SEARCH_FOR = "ant";
const unsigned int ORDERED_SEARCH_AT = 0;
const unsigned int ORDERED_SMALLEST_AT = 0;
const unsigned int ORDERED_LARGEST_AT = ORDERED_DATA_COUNT - 1;

const string REVERSE_DATA_DESCRIPTION = "an array in reverse order";
const unsigned int REVERSE_DATA_COUNT = 9;
const string REVERSE_DATA_UNSORTED[REVERSE_DATA_COUNT] = {
    "Ilsa", "Hal", "Greg", "Frank", "Evie", "Dan", "Chuck", "Bob", "Alice",
};
const string REVERSE_DATA_CORRECT[REVERSE_DATA_COUNT] = {
    "Alice", "Bob", "Chuck", "Dan", "Evie", "Frank", "Greg", "Hal", "Ilsa",
};
const string REVERSE_UNSORTED_SEARCH_FOR = "Alice";
const string REVERSE_CORRECT_SEARCH_FOR = "Ilsa";
const unsigned int REVERSE_SEARCH_AT = REVERSE_DATA_COUNT - 1;
const unsigned int REVERSE_SMALLEST_AT = REVERSE_DATA_COUNT - 1;
const unsigned int REVERSE_LARGEST_AT = 0;

const string RADIO_DATA_DESCRIPTION = "some ICAO radio codes";
const unsigned int RADIO_DATA_COUNT = 6;
const string RADIO_DATA_UNSORTED[RADIO_DATA_COUNT] = {
    "delta", "bravo", "foxtrot", "charlie", "alfa", "echo",
};
const string RADIO_DATA_CORRECT[RADIO_DATA_COUNT] = {
    "alfa", "bravo", "charlie", "delta", "echo", "foxtrot",
};
const string RADIO_UNSORTED_SEARCH_FOR = "charlie";
const string RADIO_CORRECT_SEARCH_FOR = "delta";
const unsigned int RADIO_SEARCH_AT = 3;
const unsigned int RADIO_SMALLEST_AT = 4;
const unsigned int RADIO_LARGEST_AT = 2;

const string QUOTATION_DATA_DESCRIPTION = "a quotation";
const unsigned int QUOTATION_DATA_COUNT = 7;
const string QUOTATION_DATA_UNSORTED[QUOTATION_DATA_COUNT] = {
    "I", "AM", "THE", "LIGHT", "OF", "THE", "WORLD",
};
const string QUOTATION_DATA_CORRECT[QUOTATION_DATA_COUNT] = {
    "AM", "I", "LIGHT", "OF", "THE", "THE", "WORLD",
};
const string QUOTATION_SEARCH_FOR = "DARKNESS";
const unsigned int QUOTATION_SEARCH_AT = VALUE_NOT_FOUND_CORRECT;
const unsigned int QUOTATION_SMALLEST_AT = 1;
const unsigned int QUOTATION_LARGEST_AT = 6;

const string DUPLICATES_DATA_DESCRIPTION = "an array with duplicates";
const unsigned int DUPLICATES_DATA_COUNT = 8;
const string DUPLICATES_DATA_UNSORTED[DUPLICATES_DATA_COUNT] = {
    "yes", "yes", "no", "yes", "maybe", "no", "no", "yes",
};
const string DUPLICATES_DATA_CORRECT[DUPLICATES_DATA_COUNT] = {
    "maybe", "no", "no", "no", "yes", "yes", "yes", "yes",
};
const string DUPLICATES_SEARCH_FOR = "yes";
const unsigned int DUPLICATES_UNSORTED_SEARCH_AT = 0;
const unsigned int DUPLICATES_SEARCH_AT_FIRST = 4;
const unsigned int DUPLICATES_SEARCH_AT_LAST = 7;
const unsigned int DUPLICATES_SMALLEST_AT = 4;
const unsigned int DUPLICATES_LARGEST_AT = 0;

const string LONG_DATA_DESCRIPTION = "a big array";
const unsigned int LONG_DATA_COUNT = 45;
const string LONG_DATA_UNSORTED[LONG_DATA_COUNT] = {
    "1", "1", "2", "1", "2", "3", "1", "2", "3", "4", "1", "2", "3", "4", "5",
    "1", "2", "3", "4", "5", "6", "1", "2", "3", "4", "5", "6", "7", "1", "2",
    "3", "4", "5", "6", "7", "8", "1", "2", "3", "4", "5", "6", "7", "8", "9",
};
const string LONG_DATA_CORRECT[LONG_DATA_COUNT] = {
    "1", "1", "1", "1", "1", "1", "1", "1", "1", "2", "2", "2", "2", "2", "2",
    "2", "2", "3", "3", "3", "3", "3", "3", "3", "4", "4", "4", "4", "4", "4",
    "5", "5", "5", "5", "5", "6", "6", "6", "6", "7", "7", "7", "8", "8", "9",
};
const string LONG_SEARCH_FOR = "3";
const unsigned int LONG_SEARCH_AT_FIRST = 17;
const unsigned int LONG_SEARCH_AT_LAST = 23;
const unsigned int LONG_SMALLEST_AT = 0;
const unsigned int LONG_LARGEST_AT = LONG_DATA_COUNT - 1;

const string NONE_DATA_DESCRIPTION = "an array with no data";
const unsigned int NONE_DATA_COUNT = 0;
const string NONE_DATA_UNSORTED[NONE_DATA_COUNT +
                                1] = // C++ doesn't allow 0-sized arrays
    {};
const string *NONE_DATA_CORRECT =
    NONE_DATA_UNSORTED; // cheating: same array under new name
const string NONE_SEARCH_FOR = "Where am I?";
const unsigned int NONE_SEARCH_AT = VALUE_NOT_FOUND_CORRECT;

int main() {
  TestHelper::startup("TestSearch1", COMPILE_AND_START_MARKS, TOTAL_MARKS);

  //
  //  Test constants
  //

  cout << "Testing VALUE_NOT_FOUND constant" << endl;
  bool correct_value_not_found = TestHelper::testConstant(
      VALUE_NOT_FOUND, VALUE_NOT_FOUND_CORRECT, "VALUE_NOT_FOUND");
  cout << endl;

  cout << endl;
  cout << endl;

  //
  //  Print results 1
  //

  TestHelper::printSummaryHeaderPartial(24, 1, COMPILE_AND_START_MARKS);
  TestHelper::printSummaryLine("VALUE_NOT_FOUND", correct_value_not_found);

  unsigned int highest1 = calculateMark(true, 0, 0, 0, // unsorted searches
                                        false);        // finished
  unsigned int mark1 =
      calculateMark(correct_value_not_found, 0, 0, 0, // unsorted searches
                    false);                           // finished
  assert(mark1 <= TOTAL_MARKS);
  TestHelper::printMarkPartial(mark1, highest1, TOTAL_MARKS);

  cout << endl;
  cout << endl;

  //
  //  Test linear search
  //

  cout << "Testing linearSearch:" << endl;
  cout << "---------------------" << endl;
  cout << endl;

  bool linear_search_single = testSearch(
      linearSearch, DESCRIPTION_ONLY, SINGLE_DATA_UNSORTED, SINGLE_DATA_COUNT,
      SINGLE_SEARCH_FOR, SINGLE_SEARCH_AT, SINGLE_SEARCH_AT, false);
  bool linear_search_ordered =
      testSearch(linearSearch, DESCRIPTION_FIRST, ORDERED_DATA_UNSORTED,
                 ORDERED_DATA_COUNT, ORDERED_SEARCH_FOR, ORDERED_SEARCH_AT,
                 ORDERED_SEARCH_AT, false);
  bool linear_search_reverse = testSearch(
      linearSearch, DESCRIPTION_LAST, REVERSE_DATA_UNSORTED, REVERSE_DATA_COUNT,
      REVERSE_UNSORTED_SEARCH_FOR, REVERSE_SEARCH_AT, REVERSE_SEARCH_AT, false);
  bool linear_search_radio = testSearch(
      linearSearch, DESCRIPTION_MIDDLE, RADIO_DATA_UNSORTED, RADIO_DATA_COUNT,
      RADIO_UNSORTED_SEARCH_FOR, RADIO_SEARCH_AT, RADIO_SEARCH_AT, false);
  bool linear_search_quotation =
      testSearch(linearSearch, DESCRIPTION_MISSING, QUOTATION_DATA_UNSORTED,
                 QUOTATION_DATA_COUNT, QUOTATION_SEARCH_FOR,
                 QUOTATION_SEARCH_AT, QUOTATION_SEARCH_AT, false);
  bool linear_search_duplicates = testSearch(
      linearSearch, DESCRIPTION_DUP_ANY, DUPLICATES_DATA_UNSORTED,
      DUPLICATES_DATA_COUNT, DUPLICATES_SEARCH_FOR,
      DUPLICATES_UNSORTED_SEARCH_AT, DUPLICATES_UNSORTED_SEARCH_AT, false);
  bool linear_search_long = testSearch(
      linearSearch, DESCRIPTION_DUP_ANY, LONG_DATA_UNSORTED, LONG_DATA_COUNT,
      LONG_SEARCH_FOR, LONG_SEARCH_AT_FIRST, LONG_SEARCH_AT_LAST, false);
  bool linear_search_none = testSearch(
      linearSearch, DESCRIPTION_EMPTY, NONE_DATA_UNSORTED, NONE_DATA_COUNT,
      NONE_SEARCH_FOR, NONE_SEARCH_AT, NONE_SEARCH_AT, false);
  cout << endl;
  cout << endl;

  //
  //  Test finding smallest and largest in unsorted data
  //

  cout << "Testing unsortedFindSmallest:" << endl;
  cout << "-----------------------------" << endl;
  cout << endl;

  bool unsorted_smallest_single = testFindExtreme(
      unsortedFindSmallest, SINGLE_DATA_DESCRIPTION, SINGLE_DATA_UNSORTED,
      SINGLE_DATA_COUNT, true, SINGLE_SMALLEST_AT);
  bool unsorted_smallest_ordered = testFindExtreme(
      unsortedFindSmallest, ORDERED_DATA_DESCRIPTION, ORDERED_DATA_UNSORTED,
      ORDERED_DATA_COUNT, true, ORDERED_SMALLEST_AT);
  bool unsorted_smallest_reverse = testFindExtreme(
      unsortedFindSmallest, REVERSE_DATA_DESCRIPTION, REVERSE_DATA_UNSORTED,
      REVERSE_DATA_COUNT, true, REVERSE_SMALLEST_AT);
  bool unsorted_smallest_radio = testFindExtreme(
      unsortedFindSmallest, RADIO_DATA_DESCRIPTION, RADIO_DATA_UNSORTED,
      RADIO_DATA_COUNT, true, RADIO_SMALLEST_AT);
  bool unsorted_smallest_duplicates =
      testFindExtreme(unsortedFindSmallest, DUPLICATES_DATA_DESCRIPTION,
                      DUPLICATES_DATA_UNSORTED, DUPLICATES_DATA_COUNT, true,
                      DUPLICATES_SMALLEST_AT);
  bool unsorted_smallest_long = testFindExtreme(
      unsortedFindSmallest, LONG_DATA_DESCRIPTION, LONG_DATA_UNSORTED,
      LONG_DATA_COUNT, true, LONG_SMALLEST_AT);
  cout << endl;
  cout << endl;

  cout << "Testing unsortedFindLargest:" << endl;
  cout << "----------------------------" << endl;
  cout << endl;

  bool unsorted_largest_single = testFindExtreme(
      unsortedFindLargest, SINGLE_DATA_DESCRIPTION, SINGLE_DATA_UNSORTED,
      SINGLE_DATA_COUNT, false, SINGLE_LARGEST_AT);
  bool unsorted_largest_ordered = testFindExtreme(
      unsortedFindLargest, ORDERED_DATA_DESCRIPTION, ORDERED_DATA_UNSORTED,
      ORDERED_DATA_COUNT, false, ORDERED_LARGEST_AT);
  bool unsorted_largest_reverse = testFindExtreme(
      unsortedFindLargest, REVERSE_DATA_DESCRIPTION, REVERSE_DATA_UNSORTED,
      REVERSE_DATA_COUNT, false, REVERSE_LARGEST_AT);
  bool unsorted_largest_radio = testFindExtreme(
      unsortedFindLargest, RADIO_DATA_DESCRIPTION, RADIO_DATA_UNSORTED,
      RADIO_DATA_COUNT, false, RADIO_LARGEST_AT);
  bool unsorted_largest_duplicates =
      testFindExtreme(unsortedFindLargest, DUPLICATES_DATA_DESCRIPTION,
                      DUPLICATES_DATA_UNSORTED, DUPLICATES_DATA_COUNT, false,
                      DUPLICATES_LARGEST_AT);
  bool unsorted_largest_long = testFindExtreme(
      unsortedFindLargest, LONG_DATA_DESCRIPTION, LONG_DATA_UNSORTED,
      LONG_DATA_COUNT, false, LONG_LARGEST_AT);
  cout << endl;
  cout << endl;

  //
  //  Print results 2
  //

  unsigned int correct_linear_search =
      (linear_search_single ? 1 : 0) + (linear_search_ordered ? 1 : 0) +
      (linear_search_reverse ? 1 : 0) + (linear_search_radio ? 1 : 0) +
      (linear_search_quotation ? 1 : 0) + (linear_search_duplicates ? 1 : 0) +
      (linear_search_long ? 1 : 0) + (linear_search_none ? 1 : 0);

  unsigned int correct_unsorted_smallest =
      (unsorted_smallest_single ? 1 : 0) + (unsorted_smallest_ordered ? 1 : 0) +
      (unsorted_smallest_reverse ? 1 : 0) + (unsorted_smallest_radio ? 1 : 0) +
      (unsorted_smallest_duplicates ? 1 : 0) + (unsorted_smallest_long ? 1 : 0);

  unsigned int correct_unsorted_largest =
      (unsorted_largest_single ? 1 : 0) + (unsorted_largest_ordered ? 1 : 0) +
      (unsorted_largest_reverse ? 1 : 0) + (unsorted_largest_radio ? 1 : 0) +
      (unsorted_largest_duplicates ? 1 : 0) + (unsorted_largest_long ? 1 : 0);

  TestHelper::printSummaryHeaderPartial(24, 1, COMPILE_AND_START_MARKS);
  TestHelper::printSummaryLine("VALUE_NOT_FOUND", correct_value_not_found);
  TestHelper::printSummaryLine("linearSearch", correct_linear_search,
                               DATA_SEARCH_COUNT);
  TestHelper::printSummaryLine("unsortedFindSmallest",
                               correct_unsorted_smallest,
                               DATA_FIND_EXTREME_COUNT);
  TestHelper::printSummaryLine("unsortedFindLargest", correct_unsorted_largest,
                               DATA_FIND_EXTREME_COUNT);

  unsigned int mark2 =
      calculateMark(correct_value_not_found, correct_linear_search,
                    correct_unsorted_smallest, correct_unsorted_largest,
                    true); // finished
  assert(mark2 <= TOTAL_MARKS);
  TestHelper::printMark(mark2, TOTAL_MARKS);

  TestHelper::waitForEnter();
  return 0;
}

bool testSearch(SearchFunction search_function, const string &description,
                const string data[], unsigned int count,
                const string &search_for, unsigned int correct_index_first,
                unsigned int correct_index_last, bool must_be_first) {
  cout << "Searching for \"" << search_for << "\" " << description << ":"
       << endl;

  int returned_index = search_function(data, count, search_for);
  if (returned_index == VALUE_NOT_FOUND_CORRECT)
    cout << "\tValue not found" << endl;
  else {
    cout << "\tFound at index " << returned_index << " / " << count;
    if (returned_index < 0)
      cout << " -- NEGATIVE_INDEX!" << endl;
    else if (returned_index >= (int)(count))
      cout << " -- OUTSIDE ARRAY!" << endl;
    else if (data[returned_index] != search_for)
      cout << " (\"" << data[returned_index] << "\")" << endl;
    else
      cout << endl;
  }

  if (returned_index == VALUE_NOT_FOUND_CORRECT &&
      correct_index_first == VALUE_NOT_FOUND_CORRECT) {
    cout << "* Correct" << endl;
    cout << endl;
    return true;
  } else if (data[returned_index] == search_for) {
    if (!must_be_first || returned_index == correct_index_first) {
      cout << "* Correct" << endl;
      cout << endl;
      return true;
    } else
      cout << "* Incorrect: First copy of \"" << search_for << "\" is at index "
           << correct_index_first << endl;
  } else if (returned_index != VALUE_NOT_FOUND_CORRECT &&
             correct_index_first == VALUE_NOT_FOUND_CORRECT) {
    cout << "* Incorrect: \"" << search_for << "\" is not in the array "
         << endl;
  } else if (returned_index == VALUE_NOT_FOUND_CORRECT &&
             correct_index_first != VALUE_NOT_FOUND_CORRECT) {
    if (correct_index_first == correct_index_last)
      cout << "* Incorrect: \"" << search_for << "\" is in array at index "
           << correct_index_first << endl;
    else {
      cout << "* Incorrect: \"" << search_for << "\" is in array at indexes "
           << correct_index_first << " - " << correct_index_last << endl;
    }
  } else if (correct_index_first == correct_index_last)
    cout << "* Incorrect: \"" << search_for << "\" is at index "
         << correct_index_first << endl;
  else {
    if (returned_index < (int)(correct_index_first))
      cout << "* Incorrect: \"" << search_for << "\" starts at index "
           << correct_index_first << endl;
    else if (returned_index > (int)(correct_index_last))
      cout << "* Incorrect: \"" << search_for << "\" ends at index "
           << correct_index_last << endl;
  }

  cout << endl;
  return false;
}

bool testFindExtreme(FindExtremeFunction find_extreme_function,
                     const string &description, const string data[],
                     unsigned int count, bool is_smallest,
                     unsigned int correct_index) {
  string extreme_root = is_smallest ? "small" : "larg";
  cout << "Searching for " << extreme_root << "est element " << description
       << ":" << endl;

  int returned_index = find_extreme_function(data, count);

  bool is_correct = true;
  if (is_smallest == true && data[correct_index] < data[returned_index])
    is_correct = false;
  if (is_smallest == false && data[returned_index] < data[correct_index])
    is_correct = false;

  if (returned_index < 0) {
    cout << "\tFound invalid data at index " << returned_index << " / " << count
         << " -- NEGATIVE_INDEX!" << endl;
  } else if (returned_index >= (int)(count)) {
    cout << "\tFound invalid data at index " << returned_index << " / " << count
         << " -- OUTSIDE ARRAY!" << endl;
  } else {
    cout << "\tFound \"" << data[returned_index] << "\" at index "
         << returned_index << " / " << count << endl;
  }

  if (is_correct) {
    cout << "* Correct" << endl;
    cout << endl;
    return true;
  } else {
    cout << "* Incorrect: \"" << data[correct_index] << "\" at index "
         << correct_index << " is " << extreme_root << "er" << endl;
    cout << endl;
    return false;
  }
}

int calculateMark(bool correct_value_not_found,
                  unsigned int correct_linear_search,
                  unsigned int correct_unsorted_smallest,
                  unsigned int correct_unsorted_largest, bool is_finished) {
  unsigned int mark = 0;
  if (is_finished)
    mark = COMPILE_AND_NOT_CRASH_MARKS;
  else
    mark = COMPILE_AND_START_MARKS;

  if (correct_value_not_found)
    mark += 1;

  mark += correct_linear_search / 2;
  mark += correct_unsorted_smallest / 3;
  mark += correct_unsorted_largest / 3;
  if (correct_unsorted_smallest == DATA_FIND_EXTREME_COUNT ||
      correct_unsorted_largest == DATA_FIND_EXTREME_COUNT) {
    mark += 1;
  }

  assert(mark <= TOTAL_MARKS);
  return mark;
}