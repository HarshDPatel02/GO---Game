//
//  TestSearch2.cpp
//
//  A test program for the Search module, as expanded in
//    Assignment 2.
//
//  This program is to be used with Assignment 2: Part A for
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

struct SortResult {
  unsigned int correct;
  unsigned int max;
};

struct FrequencyForElement {
  string value;
  unsigned int frequency;
};

SortResult testSort(const string &description, const string unsorted[],
                    const string correct[], unsigned int count);
void copyArray(const string original[], string copy[], unsigned int count);
void printArrays(const string unsorted[], const string sorted[],
                 const string correct[], unsigned int count);
bool isArraySorted(const string data[], unsigned int count);
int getArraySortedCount(const string data[], unsigned int count);
unsigned int countArrayMismatches(const string data1[], const string data2[],
                                  unsigned int count);
unsigned int findArrayFirstMismatch(const string data1[], const string data2[],
                                    unsigned int count);
bool checkSortFrequency(const string sorted[], const string correct[],
                        unsigned int count);
unsigned int getCountInArray(const string data[], unsigned int count,
                             const string check_for);

bool testIsSorted(const string &description, const string data[],
                  unsigned int count, bool correct);
unsigned int getArrayUnsortedIndex(const string data[], unsigned int count);

int calculateMark(
    bool correct_value_not_found, unsigned int correct_linear_search,
    unsigned int correct_unsorted_smallest,
    unsigned int correct_unsorted_largest, const SortResult &sort_result_single,
    const SortResult &sort_result_ordered,
    const SortResult &sort_result_reverse, const SortResult &sort_result_radio,
    const SortResult &sort_result_quotation,
    const SortResult &sort_result_duplicates,
    const SortResult &sort_result_long, const SortResult &sort_result_none,
    unsigned int correct_is_sorted, unsigned int correct_sorted_smallest,
    unsigned int correct_sorted_largest, double correct_binary_search_fraction,
    double correct_binary_first_fraction, bool is_finished);
int calculateMarkBinary(double correct_binary_search_fraction,
                        double correct_binary_first_fraction);

const int ASSIGNMENT_1_FUNCTION_MARKS = 4;
const int COMPILE_AND_START_MARKS = 5 - ASSIGNMENT_1_FUNCTION_MARKS;
const int DID_NOT_CRASH_RUNNING_MARKS = 5;
const int COMPILE_AND_NOT_CRASH_MARKS =
    COMPILE_AND_START_MARKS + DID_NOT_CRASH_RUNNING_MARKS;
const int COMBINED_TEST_MARKS = 20 + ASSIGNMENT_1_FUNCTION_MARKS;
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
const unsigned int DATA_SEARCH_FIRST_COUNT = 10;
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
const bool SINGLE_IS_SORTED = true;

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
const bool ORDERED_IS_SORTED = true;

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
const bool REVERSE_IS_SORTED = false;

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
const bool RADIO_IS_SORTED = false;

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
const bool QUOTATION_IS_SORTED = false;

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
const bool DUPLICATES_IS_SORTED = false;

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
const bool LONG_IS_SORTED = false;
const string LONG_SEARCH_FOR_2 = "1";
const unsigned int LONG_SEARCH_AT_FIRST_2 = 0;
const unsigned int LONG_SEARCH_AT_LAST_2 = 8;
const string LONG_SEARCH_FOR_3 = "7";
const unsigned int LONG_SEARCH_AT_FIRST_3 = 39;
const unsigned int LONG_SEARCH_AT_LAST_3 = 41;

const string NONE_DATA_DESCRIPTION = "an array with no data";
const unsigned int NONE_DATA_COUNT = 0;
const string NONE_DATA_UNSORTED[NONE_DATA_COUNT +
                                1] = // C++ doesn't allow 0-sized arrays
    {};
const string *NONE_DATA_CORRECT =
    NONE_DATA_UNSORTED; // cheating: same array under new name
const string NONE_SEARCH_FOR = "Where am I?";
const unsigned int NONE_SEARCH_AT = VALUE_NOT_FOUND_CORRECT;
const bool NONE_IS_SORTED = true;

int main() {
  TestHelper::startup("TestSearch2", COMPILE_AND_START_MARKS, TOTAL_MARKS);

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

  const SortResult RESULT_0 = {
      0,
      1,
  };
  const SortResult RESULT_1 = {
      1,
      1,
  };

  TestHelper::printSummaryHeaderPartial(25, 2, COMPILE_AND_START_MARKS);
  TestHelper::printSummaryLine("VALUE_NOT_FOUND", correct_value_not_found);

  unsigned int highest1 = calculateMark(
      true, 0, 0, 0,                                   // unsorted searches
      RESULT_0, RESULT_0, RESULT_0, RESULT_0,          // sort results
      RESULT_0, RESULT_0, RESULT_0, RESULT_0, 0, 0, 0, // sorted find extremes
      0.0, 0.0,                                        // binary searches
      false);                                          // finished
  unsigned int mark1 = calculateMark(
      correct_value_not_found, 0, 0, 0,                // unsorted searches
      RESULT_0, RESULT_0, RESULT_0, RESULT_0,          // sort results
      RESULT_0, RESULT_0, RESULT_0, RESULT_0, 0, 0, 0, // sorted find extremes
      0.0, 0.0,                                        // binary searches
      false);                                          // finished
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

  TestHelper::printSummaryHeaderPartial(25, 2, COMPILE_AND_START_MARKS);
  TestHelper::printSummaryLine("VALUE_NOT_FOUND", correct_value_not_found);
  TestHelper::printSummaryLine("linearSearch", correct_linear_search,
                               DATA_SEARCH_COUNT);
  TestHelper::printSummaryLine("unsortedFindSmallest",
                               correct_unsorted_smallest,
                               DATA_FIND_EXTREME_COUNT);
  TestHelper::printSummaryLine("unsortedFindLargest", correct_unsorted_largest,
                               DATA_FIND_EXTREME_COUNT);

  unsigned int highest2 = calculateMark(
      true, DATA_SEARCH_COUNT, DATA_FIND_EXTREME_COUNT,
      DATA_FIND_EXTREME_COUNT,                         // unsorted searches
      RESULT_0, RESULT_0, RESULT_0, RESULT_0,          // sort results
      RESULT_0, RESULT_0, RESULT_0, RESULT_0, 0, 0, 0, // sorted find extremes
      0.0, 0.0,                                        // binary searches
      false);                                          // finished
  unsigned int mark2 = calculateMark(
      correct_value_not_found, correct_linear_search, correct_unsorted_smallest,
      correct_unsorted_largest, RESULT_0, RESULT_0, RESULT_0,
      RESULT_0,                                        // sort results
      RESULT_0, RESULT_0, RESULT_0, RESULT_0, 0, 0, 0, // sorted find extremes
      0.0, 0.0,                                        // binary searches
      false);                                          // finished
  assert(mark2 <= TOTAL_MARKS);
  TestHelper::printMarkPartial(mark2, highest2, TOTAL_MARKS);

  cout << endl;
  cout << endl;

  //
  //  Test sorting
  //

  cout << "Testing sort:" << endl;
  cout << "-------------" << endl;
  cout << endl;

  SortResult result_single =
      testSort(SINGLE_DATA_DESCRIPTION, SINGLE_DATA_UNSORTED,
               SINGLE_DATA_CORRECT, SINGLE_DATA_COUNT);
  SortResult result_ordered =
      testSort(ORDERED_DATA_DESCRIPTION, ORDERED_DATA_UNSORTED,
               ORDERED_DATA_CORRECT, ORDERED_DATA_COUNT);
  SortResult result_reverse =
      testSort(REVERSE_DATA_DESCRIPTION, REVERSE_DATA_UNSORTED,
               REVERSE_DATA_CORRECT, REVERSE_DATA_COUNT);
  SortResult result_radio =
      testSort(RADIO_DATA_DESCRIPTION, RADIO_DATA_UNSORTED, RADIO_DATA_CORRECT,
               RADIO_DATA_COUNT);
  SortResult result_quotation =
      testSort(QUOTATION_DATA_DESCRIPTION, QUOTATION_DATA_UNSORTED,
               QUOTATION_DATA_CORRECT, QUOTATION_DATA_COUNT);
  SortResult result_duplicates =
      testSort(DUPLICATES_DATA_DESCRIPTION, DUPLICATES_DATA_UNSORTED,
               DUPLICATES_DATA_CORRECT, DUPLICATES_DATA_COUNT);
  SortResult result_long = testSort(LONG_DATA_DESCRIPTION, LONG_DATA_UNSORTED,
                                    LONG_DATA_CORRECT, LONG_DATA_COUNT);
  SortResult result_none = testSort(NONE_DATA_DESCRIPTION, NONE_DATA_UNSORTED,
                                    NONE_DATA_CORRECT, NONE_DATA_COUNT);
  cout << endl;
  cout << endl;

  //
  //  Print results 3
  //

  TestHelper::printSummaryHeaderPartial(25, 2, COMPILE_AND_START_MARKS);
  TestHelper::printSummaryLine("VALUE_NOT_FOUND", correct_value_not_found);
  TestHelper::printSummaryLine("linearSearch", correct_linear_search,
                               DATA_SEARCH_COUNT);
  TestHelper::printSummaryLine("unsortedFindSmallest",
                               correct_unsorted_smallest,
                               DATA_FIND_EXTREME_COUNT);
  TestHelper::printSummaryLine("unsortedFindLargest", correct_unsorted_largest,
                               DATA_FIND_EXTREME_COUNT);
  TestHelper::printSummaryLine("sort data (1 element)", result_single.correct,
                               result_single.max);
  TestHelper::printSummaryLine("sort data (sorted)", result_ordered.correct,
                               result_ordered.max);
  TestHelper::printSummaryLine("sort data (reverse)", result_reverse.correct,
                               result_reverse.max);
  TestHelper::printSummaryLine("sort data (radio codes)", result_radio.correct,
                               result_radio.max);
  TestHelper::printSummaryLine("sort data (quotation)",
                               result_quotation.correct, result_quotation.max);
  TestHelper::printSummaryLine("sort data (duplicates)",
                               result_duplicates.correct,
                               result_duplicates.max);
  TestHelper::printSummaryLine("sort data (long array)", result_long.correct,
                               result_long.max);
  TestHelper::printSummaryLine("sort data (no data)", result_none.correct,
                               result_none.max);

  unsigned int highest3 = calculateMark(
      true, DATA_SEARCH_COUNT, DATA_FIND_EXTREME_COUNT,
      DATA_FIND_EXTREME_COUNT,                         // unsorted searches
      RESULT_1, RESULT_1, RESULT_1, RESULT_1,          // sort results
      RESULT_1, RESULT_1, RESULT_1, RESULT_1, 0, 0, 0, // sorted find extremes
      0.0, 0.0,                                        // binary searches
      false);                                          // finished
  unsigned int mark3 = calculateMark(
      correct_value_not_found, correct_linear_search, correct_unsorted_smallest,
      correct_unsorted_largest, result_single, result_ordered, result_reverse,
      result_radio, result_quotation, result_duplicates, result_long,
      result_none, 0, 0, 0, // sorted find extremes
      0.0, 0.0,             // binary searches
      false);               // finished
  assert(mark3 <= TOTAL_MARKS);
  TestHelper::printMarkPartial(mark3, highest3, TOTAL_MARKS);

  cout << endl;
  cout << endl;

  //
  //  Test checking whether arrays are sorted
  //

  cout << "Testing isSorted:" << endl;
  cout << "-----------------" << endl;
  cout << endl;

  bool is_sorted_single =
      testIsSorted(SINGLE_DATA_DESCRIPTION, SINGLE_DATA_UNSORTED,
                   SINGLE_DATA_COUNT, SINGLE_IS_SORTED);
  bool is_sorted_ordered =
      testIsSorted(ORDERED_DATA_DESCRIPTION, ORDERED_DATA_UNSORTED,
                   ORDERED_DATA_COUNT, ORDERED_IS_SORTED);
  bool is_sorted_reverse =
      testIsSorted(REVERSE_DATA_DESCRIPTION, REVERSE_DATA_UNSORTED,
                   REVERSE_DATA_COUNT, REVERSE_IS_SORTED);
  bool is_sorted_radio =
      testIsSorted(RADIO_DATA_DESCRIPTION, RADIO_DATA_UNSORTED,
                   RADIO_DATA_COUNT, RADIO_IS_SORTED);
  bool is_sorted_quotation =
      testIsSorted(QUOTATION_DATA_DESCRIPTION, QUOTATION_DATA_UNSORTED,
                   QUOTATION_DATA_COUNT, QUOTATION_IS_SORTED);
  bool is_sorted_duplicates =
      testIsSorted(DUPLICATES_DATA_DESCRIPTION, DUPLICATES_DATA_UNSORTED,
                   DUPLICATES_DATA_COUNT, DUPLICATES_IS_SORTED);
  bool is_sorted_long = testIsSorted(LONG_DATA_DESCRIPTION, LONG_DATA_UNSORTED,
                                     LONG_DATA_COUNT, LONG_IS_SORTED);
  bool is_sorted_none = testIsSorted(NONE_DATA_DESCRIPTION, NONE_DATA_UNSORTED,
                                     NONE_DATA_COUNT, NONE_IS_SORTED);
  cout << endl;
  cout << endl;

  //
  //  Test finding smallest and largest in unsorted data
  //

  cout << "Testing sortedFindSmallest:" << endl;
  cout << "---------------------------" << endl;
  cout << endl;

  bool sorted_smallest_single =
      testFindExtreme(sortedFindSmallest, SINGLE_DATA_DESCRIPTION,
                      SINGLE_DATA_CORRECT, SINGLE_DATA_COUNT, true, 0);
  bool sorted_smallest_ordered =
      testFindExtreme(sortedFindSmallest, ORDERED_DATA_DESCRIPTION,
                      ORDERED_DATA_CORRECT, ORDERED_DATA_COUNT, true, 0);
  bool sorted_smallest_reverse =
      testFindExtreme(sortedFindSmallest, REVERSE_DATA_DESCRIPTION,
                      REVERSE_DATA_CORRECT, REVERSE_DATA_COUNT, true, 0);
  bool sorted_smallest_radio =
      testFindExtreme(sortedFindSmallest, RADIO_DATA_DESCRIPTION,
                      RADIO_DATA_CORRECT, RADIO_DATA_COUNT, true, 0);
  bool sorted_smallest_duplicates =
      testFindExtreme(sortedFindSmallest, DUPLICATES_DATA_DESCRIPTION,
                      DUPLICATES_DATA_CORRECT, DUPLICATES_DATA_COUNT, true, 0);
  bool sorted_smallest_long =
      testFindExtreme(sortedFindSmallest, LONG_DATA_DESCRIPTION,
                      LONG_DATA_CORRECT, LONG_DATA_COUNT, true, 0);
  cout << endl;
  cout << endl;

  cout << "Testing sortedFindLargest:" << endl;
  cout << "--------------------------" << endl;
  cout << endl;

  bool sorted_largest_single = testFindExtreme(
      sortedFindLargest, SINGLE_DATA_DESCRIPTION, SINGLE_DATA_CORRECT,
      SINGLE_DATA_COUNT, false, SINGLE_DATA_COUNT - 1);
  bool sorted_largest_ordered = testFindExtreme(
      sortedFindLargest, ORDERED_DATA_DESCRIPTION, ORDERED_DATA_CORRECT,
      ORDERED_DATA_COUNT, false, ORDERED_DATA_COUNT - 1);
  bool sorted_largest_reverse = testFindExtreme(
      sortedFindLargest, REVERSE_DATA_DESCRIPTION, REVERSE_DATA_CORRECT,
      REVERSE_DATA_COUNT, false, REVERSE_DATA_COUNT - 1);
  bool sorted_largest_radio = testFindExtreme(
      sortedFindLargest, RADIO_DATA_DESCRIPTION, RADIO_DATA_CORRECT,
      RADIO_DATA_COUNT, false, RADIO_DATA_COUNT - 1);
  bool sorted_largest_duplicates = testFindExtreme(
      sortedFindLargest, DUPLICATES_DATA_DESCRIPTION, DUPLICATES_DATA_CORRECT,
      DUPLICATES_DATA_COUNT, false, DUPLICATES_DATA_COUNT - 1);
  bool sorted_largest_long = testFindExtreme(
      sortedFindLargest, LONG_DATA_DESCRIPTION, LONG_DATA_CORRECT,
      LONG_DATA_COUNT, false, LONG_DATA_COUNT - 1);
  cout << endl;
  cout << endl;

  //
  //  Print results 4
  //

  unsigned int correct_is_sorted =
      (is_sorted_single ? 1 : 0) + (is_sorted_ordered ? 1 : 0) +
      (is_sorted_reverse ? 1 : 0) + (is_sorted_radio ? 1 : 0) +
      (is_sorted_quotation ? 1 : 0) + (is_sorted_duplicates ? 1 : 0) +
      (is_sorted_long ? 1 : 0) + (is_sorted_none ? 1 : 0);

  unsigned int correct_sorted_smallest =
      (sorted_smallest_single ? 1 : 0) + (sorted_smallest_ordered ? 1 : 0) +
      (sorted_smallest_reverse ? 1 : 0) + (sorted_smallest_radio ? 1 : 0) +
      (sorted_smallest_duplicates ? 1 : 0) + (sorted_smallest_long ? 1 : 0);

  unsigned int correct_sorted_largest =
      (sorted_largest_single ? 1 : 0) + (sorted_largest_ordered ? 1 : 0) +
      (sorted_largest_reverse ? 1 : 0) + (sorted_largest_radio ? 1 : 0) +
      (sorted_largest_duplicates ? 1 : 0) + (sorted_largest_long ? 1 : 0);

  TestHelper::printSummaryHeaderPartial(25, 2, COMPILE_AND_START_MARKS);
  TestHelper::printSummaryLine("VALUE_NOT_FOUND", correct_value_not_found);
  TestHelper::printSummaryLine("linearSearch", correct_linear_search,
                               DATA_SEARCH_COUNT);
  TestHelper::printSummaryLine("unsortedFindSmallest",
                               correct_unsorted_smallest,
                               DATA_FIND_EXTREME_COUNT);
  TestHelper::printSummaryLine("unsortedFindLargest", correct_unsorted_largest,
                               DATA_FIND_EXTREME_COUNT);
  TestHelper::printSummaryLine("sort data (1 element)", result_single.correct,
                               result_single.max);
  TestHelper::printSummaryLine("sort data (sorted)", result_ordered.correct,
                               result_ordered.max);
  TestHelper::printSummaryLine("sort data (reverse)", result_reverse.correct,
                               result_reverse.max);
  TestHelper::printSummaryLine("sort data (radio codes)", result_radio.correct,
                               result_radio.max);
  TestHelper::printSummaryLine("sort data (quotation)",
                               result_quotation.correct, result_quotation.max);
  TestHelper::printSummaryLine("sort data (duplicates)",
                               result_duplicates.correct,
                               result_duplicates.max);
  TestHelper::printSummaryLine("sort data (long array)", result_long.correct,
                               result_long.max);
  TestHelper::printSummaryLine("sort data (no data)", result_none.correct,
                               result_none.max);
  TestHelper::printSummaryLine("isSorted", correct_is_sorted,
                               DATA_SEARCH_COUNT);
  TestHelper::printSummaryLine("sortedFindSmallest", correct_sorted_smallest,
                               DATA_FIND_EXTREME_COUNT);
  TestHelper::printSummaryLine("sortedFindLargest", correct_sorted_largest,
                               DATA_FIND_EXTREME_COUNT);

  unsigned int highest4 = calculateMark(
      true, DATA_SEARCH_COUNT, DATA_FIND_EXTREME_COUNT,
      DATA_FIND_EXTREME_COUNT,                // unsorted searches
      RESULT_1, RESULT_1, RESULT_1, RESULT_1, // sort results
      RESULT_1, RESULT_1, RESULT_1, RESULT_1, DATA_SEARCH_COUNT,
      DATA_FIND_EXTREME_COUNT, DATA_FIND_EXTREME_COUNT, // sorted find extremes
      0.0, 0.0,                                         // binary searches
      false);                                           // finished
  unsigned int mark4 = calculateMark(
      correct_value_not_found, correct_linear_search, correct_unsorted_smallest,
      correct_unsorted_largest, result_single, result_ordered, result_reverse,
      result_radio, result_quotation, result_duplicates, result_long,
      result_none, correct_is_sorted, correct_sorted_smallest,
      correct_sorted_largest, 0.0, 0.0, // sorted searches
      false);                           // finished
  assert(mark4 <= TOTAL_MARKS);
  TestHelper::printMarkPartial(mark4, highest4, TOTAL_MARKS);

  cout << endl;
  cout << endl;

  //
  //  Test binary search
  //

  cout << "Testing binarySearch:" << endl;
  cout << "---------------------" << endl;
  cout << endl;

  bool binary_search_single = testSearch(
      binarySearch, DESCRIPTION_ONLY, SINGLE_DATA_CORRECT, SINGLE_DATA_COUNT,
      SINGLE_SEARCH_FOR, SINGLE_SEARCH_AT, SINGLE_SEARCH_AT, false);
  bool binary_search_ordered = testSearch(
      binarySearch, DESCRIPTION_FIRST, ORDERED_DATA_CORRECT, ORDERED_DATA_COUNT,
      ORDERED_SEARCH_FOR, ORDERED_SEARCH_AT, ORDERED_SEARCH_AT, false);
  bool binary_search_reverse = testSearch(
      binarySearch, DESCRIPTION_LAST, REVERSE_DATA_CORRECT, REVERSE_DATA_COUNT,
      REVERSE_CORRECT_SEARCH_FOR, REVERSE_SEARCH_AT, REVERSE_SEARCH_AT, false);
  bool binary_search_radio = testSearch(
      binarySearch, DESCRIPTION_MIDDLE, RADIO_DATA_CORRECT, RADIO_DATA_COUNT,
      RADIO_CORRECT_SEARCH_FOR, RADIO_SEARCH_AT, RADIO_SEARCH_AT, false);
  bool binary_search_quotation =
      testSearch(binarySearch, DESCRIPTION_MISSING, QUOTATION_DATA_CORRECT,
                 QUOTATION_DATA_COUNT, QUOTATION_SEARCH_FOR,
                 QUOTATION_SEARCH_AT, QUOTATION_SEARCH_AT, false);
  bool binary_search_duplicates =
      testSearch(binarySearch, DESCRIPTION_DUP_LAST, DUPLICATES_DATA_CORRECT,
                 DUPLICATES_DATA_COUNT, DUPLICATES_SEARCH_FOR,
                 DUPLICATES_SEARCH_AT_FIRST, DUPLICATES_SEARCH_AT_LAST, false);
  bool binary_search_long = testSearch(
      binarySearch, DESCRIPTION_DUP_MIDDLE, LONG_DATA_CORRECT, LONG_DATA_COUNT,
      LONG_SEARCH_FOR, LONG_SEARCH_AT_FIRST, LONG_SEARCH_AT_LAST, false);
  bool binary_search_none = testSearch(
      binarySearch, DESCRIPTION_EMPTY, NONE_DATA_CORRECT, NONE_DATA_COUNT,
      NONE_SEARCH_FOR, NONE_SEARCH_AT, NONE_SEARCH_AT, false);
  cout << endl;
  cout << endl;

  cout << "Testing binarySearchFirst:" << endl;
  cout << "--------------------------" << endl;
  cout << endl;

  bool binary_first_single =
      testSearch(binarySearchFirst, DESCRIPTION_ONLY, SINGLE_DATA_CORRECT,
                 SINGLE_DATA_COUNT, SINGLE_SEARCH_FOR, SINGLE_SEARCH_AT,
                 SINGLE_SEARCH_AT, true);
  bool binary_first_ordered =
      testSearch(binarySearchFirst, DESCRIPTION_FIRST, ORDERED_DATA_CORRECT,
                 ORDERED_DATA_COUNT, ORDERED_SEARCH_FOR, ORDERED_SEARCH_AT,
                 ORDERED_SEARCH_AT, true);
  bool binary_first_reverse =
      testSearch(binarySearchFirst, DESCRIPTION_LAST, REVERSE_DATA_CORRECT,
                 REVERSE_DATA_COUNT, REVERSE_CORRECT_SEARCH_FOR,
                 REVERSE_SEARCH_AT, REVERSE_SEARCH_AT, true);
  bool binary_first_radio =
      testSearch(binarySearchFirst, DESCRIPTION_MIDDLE, RADIO_DATA_CORRECT,
                 RADIO_DATA_COUNT, RADIO_CORRECT_SEARCH_FOR, RADIO_SEARCH_AT,
                 RADIO_SEARCH_AT, true);
  bool binary_first_quotation =
      testSearch(binarySearchFirst, DESCRIPTION_MISSING, QUOTATION_DATA_CORRECT,
                 QUOTATION_DATA_COUNT, QUOTATION_SEARCH_FOR,
                 QUOTATION_SEARCH_AT, QUOTATION_SEARCH_AT, true);
  bool binary_first_duplicates = testSearch(
      binarySearchFirst, DESCRIPTION_DUP_LAST, DUPLICATES_DATA_CORRECT,
      DUPLICATES_DATA_COUNT, DUPLICATES_SEARCH_FOR, DUPLICATES_SEARCH_AT_FIRST,
      DUPLICATES_SEARCH_AT_LAST, true);
  bool binary_first_long =
      testSearch(binarySearchFirst, DESCRIPTION_DUP_MIDDLE, LONG_DATA_CORRECT,
                 LONG_DATA_COUNT, LONG_SEARCH_FOR, LONG_SEARCH_AT_FIRST,
                 LONG_SEARCH_AT_LAST, true);
  bool binary_first_long_2 =
      testSearch(binarySearchFirst, DESCRIPTION_DUP_FIRST, LONG_DATA_CORRECT,
                 LONG_DATA_COUNT, LONG_SEARCH_FOR_2, LONG_SEARCH_AT_FIRST_2,
                 LONG_SEARCH_AT_LAST_2, true);
  bool binary_first_long_3 =
      testSearch(binarySearchFirst, DESCRIPTION_DUP_MIDDLE, LONG_DATA_CORRECT,
                 LONG_DATA_COUNT, LONG_SEARCH_FOR_3, LONG_SEARCH_AT_FIRST_3,
                 LONG_SEARCH_AT_LAST_3, true);
  bool binary_first_none = testSearch(
      binarySearchFirst, DESCRIPTION_EMPTY, NONE_DATA_CORRECT, NONE_DATA_COUNT,
      NONE_SEARCH_FOR, NONE_SEARCH_AT, NONE_SEARCH_AT, true);

  cout << endl;
  cout << endl;

  //
  //  Print results 5
  //

  unsigned int correct_binary_search =
      (binary_search_single ? 1 : 0) + (binary_search_ordered ? 1 : 0) +
      (binary_search_reverse ? 1 : 0) + (binary_search_radio ? 1 : 0) +
      (binary_search_quotation ? 1 : 0) + (binary_search_duplicates ? 1 : 0) +
      (binary_search_long ? 1 : 0) + (binary_search_none ? 1 : 0);
  double correct_binary_search_fraction = correct_binary_search / 8.0;

  unsigned int correct_binary_first =
      (binary_first_single ? 1 : 0) + (binary_first_ordered ? 1 : 0) +
      (binary_first_reverse ? 1 : 0) + (binary_first_radio ? 1 : 0) +
      (binary_first_quotation ? 1 : 0) + (binary_first_duplicates ? 1 : 0) +
      (binary_first_long ? 1 : 0) + (binary_first_long_2 ? 1 : 0) +
      (binary_first_long_3 ? 1 : 0) + (binary_first_none ? 1 : 0);
  double correct_binary_first_fraction = correct_binary_first / 10.0;

  TestHelper::printSummaryHeaderFinal(25, 2, DID_NOT_CRASH_RUNNING_MARKS);
  TestHelper::printSummaryLine("VALUE_NOT_FOUND", correct_value_not_found);
  TestHelper::printSummaryLine("linearSearch", correct_linear_search,
                               DATA_SEARCH_COUNT);
  TestHelper::printSummaryLine("unsortedFindSmallest",
                               correct_unsorted_smallest,
                               DATA_FIND_EXTREME_COUNT);
  TestHelper::printSummaryLine("unsortedFindLargest", correct_unsorted_largest,
                               DATA_FIND_EXTREME_COUNT);
  TestHelper::printSummaryLine("sort data (1 element)", result_single.correct,
                               result_single.max);
  TestHelper::printSummaryLine("sort data (sorted)", result_ordered.correct,
                               result_ordered.max);
  TestHelper::printSummaryLine("sort data (reverse)", result_reverse.correct,
                               result_reverse.max);
  TestHelper::printSummaryLine("sort data (radio codes)", result_radio.correct,
                               result_radio.max);
  TestHelper::printSummaryLine("sort data (quotation)",
                               result_quotation.correct, result_quotation.max);
  TestHelper::printSummaryLine("sort data (duplicates)",
                               result_duplicates.correct,
                               result_duplicates.max);
  TestHelper::printSummaryLine("sort data (long array)", result_long.correct,
                               result_long.max);
  TestHelper::printSummaryLine("sort data (no data)", result_none.correct,
                               result_none.max);
  TestHelper::printSummaryLine("isSorted", correct_is_sorted,
                               DATA_SEARCH_COUNT);
  TestHelper::printSummaryLine("sortedFindSmallest", correct_sorted_smallest,
                               DATA_FIND_EXTREME_COUNT);
  TestHelper::printSummaryLine("sortedFindLargest", correct_sorted_largest,
                               DATA_FIND_EXTREME_COUNT);
  TestHelper::printSummaryLine("binarySearch", correct_binary_search,
                               DATA_SEARCH_COUNT);
  TestHelper::printSummaryLine("binarySearchFirst", correct_binary_first,
                               DATA_SEARCH_FIRST_COUNT);

  unsigned int mark5 = calculateMark(
      correct_value_not_found, correct_linear_search, correct_unsorted_smallest,
      correct_unsorted_largest, result_single, result_ordered, result_reverse,
      result_radio, result_quotation, result_duplicates, result_long,
      result_none, correct_is_sorted, correct_sorted_smallest,
      correct_sorted_largest, correct_binary_search_fraction,
      correct_binary_first_fraction,
      true); // finished
  assert(mark5 <= TOTAL_MARKS);
  TestHelper::printMark(mark5, TOTAL_MARKS);

  int binary_mark = calculateMarkBinary(correct_binary_search_fraction,
                                        correct_binary_first_fraction);
  if (binary_mark > 0) {
    cout << "Note: " << binary_mark << " marks were for binary searches"
         << endl;
    cout << endl;
  }

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

SortResult testSort(const string &description, const string unsorted[],
                    const string correct[], unsigned int count) {
  cout << "Sorting " << description << " (" << count;
  if (count == 1)
    cout << " element):" << endl;
  else
    cout << " elements):" << endl;

  string *sorted = new string[count];
  copyArray(unsorted, sorted, count);
  mySort(sorted, count);
  printArrays(unsorted, sorted, correct, count);

  int count_unsorted = getArraySortedCount(unsorted, count);
  int count_sorted = getArraySortedCount(sorted, count);
  int count_correct = getArraySortedCount(correct, count);

  SortResult result;
  result.correct = 0;
  if (count_sorted >= count_unsorted)
    result.correct = 1 + count_sorted - count_unsorted;
  assert(count_correct >= count_unsorted);
  result.max = 1 + count_correct - count_unsorted;

  if (checkSortFrequency(sorted, correct, count) == false)
    result.correct = 0; // message is printed by checkSortFrequency
  else if (result.correct == result.max)
    cout << "* Correct" << endl;
  else {
    unsigned int first_mismatch =
        findArrayFirstMismatch(sorted, correct, count);
    cout << "* Incorrect: Element " << first_mismatch << " should be \""
         << correct[first_mismatch] << "\"" << endl;

    unsigned int additional = countArrayMismatches(sorted, correct, count) - 1;
    if (additional == 1)
      cout << "* " << additional << " more element incorrect" << endl;
    else if (additional > 1)
      cout << "* " << additional << " more elements incorrect" << endl;
  }
  cout << endl;

  return result;
}

void copyArray(const string original[], string copy[], unsigned int count) {
  for (unsigned int i = 0; i < count; i++) {
    copy[i] = original[i];
  }
}

void printArrays(const string unsorted[], const string sorted[],
                 const string correct[], unsigned int count) {
  TestHelper::printCaptureBlockLine();

  int max_width = 8; // width of "Original"
  for (unsigned int i = 0; i < count; i++) {
    if ((int)(unsorted[i].size()) > max_width)
      max_width = unsorted[i].size();
  }
  int display_width = max_width + 4;

  cout << "        ";
  cout << left << setw(display_width) << "Original";
  cout << left << setw(display_width) << "Sorted";
  cout << left << setw(display_width) << "Correct";
  cout << endl;

  cout << "        ";
  cout << left << setw(display_width) << "--------";
  cout << left << setw(display_width) << "------";
  cout << left << setw(display_width) << "-------";
  cout << endl;

  for (unsigned int i = 0; i < count; i++) {
    cout << right << setw(4) << i << ":   ";
    cout << left << setw(display_width) << unsorted[i];
    cout << left << setw(display_width) << sorted[i];
    cout << left << setw(display_width) << correct[i];
    if (sorted[i] != correct[i])
      cout << "<-";
    cout << endl;
  }

  TestHelper::printCaptureBlockLine();
}

bool isArraySorted(const string data[], unsigned int count) {
  for (unsigned int i = 1; i < count; i++) {
    if (data[i - 1] > data[i])
      return false;
  }
  return true;
}

int getArraySortedCount(const string data[], unsigned int count) {
  int result = 0;
  for (unsigned int i = 1; i < count; i++) {
    if (data[i - 1] <= data[i])
      result++;
  }
  return result;
}

unsigned int findArrayFirstMismatch(const string data1[], const string data2[],
                                    unsigned int count) {
  for (unsigned int i = 0; i < count; i++) {
    if (data1[i] != data2[i])
      return i;
  }
  cout << "Testing error: No mismatch found in getArrayFirstMismatch" << endl;
  return count; // no mismatches found
}

unsigned int countArrayMismatches(const string data1[], const string data2[],
                                  unsigned int count) {
  unsigned int result = 0;
  for (unsigned int i = 0; i < count; i++) {
    if (data1[i] != data2[i])
      result++;
  }
  return result;
}

bool checkSortFrequency(const string sorted[], const string correct[],
                        unsigned int count) {
  // construct frequency list

  vector<FrequencyForElement> frequencies;
  for (unsigned int i = 0; i < count; i++) {
    // skip duplicates
    if (getCountInArray(correct, i, correct[i]))
      continue;

    // otherwise, add to list
    FrequencyForElement entry;
    entry.value = correct[i];
    entry.frequency = getCountInArray(correct, count, correct[i]);
    frequencies.push_back(entry);
  }

  // check sorted array
  unsigned int changed_count = 0;
  for (unsigned int i = 0; i < count; i++) {
    // skip duplicates
    if (getCountInArray(sorted, i, sorted[i]))
      continue;

    // otherwise, check frequency

    // find the frequency for this word
    unsigned int sorted_frequency = getCountInArray(sorted, count, sorted[i]);
    for (unsigned int j = 0; j < frequencies.size(); j++) {
      if (sorted[i] == frequencies[j].value) {
        // this is the one we want to compare to

        int delta = sorted_frequency - frequencies[j].frequency;
        if (delta == 0)
          break; // end loop immediately
        else if (delta > 0) {
          changed_count += delta;

          if (delta > 1)
            cout << "* Incorrect: " << delta << " array elements changed to \""
                 << sorted[i] << "\"" << endl;
          else
            cout << "* Incorrect: 1 array element changed to \"" << sorted[i]
                 << "\"" << endl;
        }
      }
    }
  }

  return changed_count == 0;
}

unsigned int getCountInArray(const string data[], unsigned int count,
                             const string check_for) {
  unsigned int result = 0;
  for (unsigned int i = 0; i < count; i++) {
    if (data[i] == check_for)
      result++;
  }
  return result;
}

bool testIsSorted(const string &description, const string data[],
                  unsigned int count, bool correct) {
  cout << "Testing if " << description << " is sorted:  ";

  bool returned = isSorted(data, count);
  cout << boolalpha << returned << endl;

  if (returned == correct) {
    cout << "* Correct" << endl;
    cout << endl;
    return true;
  } else if (returned == true) {
    assert(count >= 1);

    unsigned int index = getArrayUnsortedIndex(data, count);
    cout << "* Incorrect: Element " << index << " / " << count
         << " is out of order" << endl;
    cout << endl;
    return false;
  } else {
    cout << "* Incorrect: Data is sorted" << endl;
    cout << endl;
    return false;
  }
}

unsigned int getArrayUnsortedIndex(const string data[], unsigned int count) {
  assert(count >= 1);

  for (unsigned int i = 1; i < count; i++) {
    if (data[i] <= data[i - 1])
      return i;
  }
  cout << "Testing error: All elements are ordered in getArrayUnsortedIndex"
       << endl;
  return count; // no mismatches found
}

int calculateMark(
    bool correct_value_not_found, unsigned int correct_linear_search,
    unsigned int correct_unsorted_smallest,
    unsigned int correct_unsorted_largest, const SortResult &sort_result_single,
    const SortResult &sort_result_ordered,
    const SortResult &sort_result_reverse, const SortResult &sort_result_radio,
    const SortResult &sort_result_quotation,
    const SortResult &sort_result_duplicates,
    const SortResult &sort_result_long, const SortResult &sort_result_none,
    unsigned int correct_is_sorted, unsigned int correct_sorted_smallest,
    unsigned int correct_sorted_largest, double correct_binary_search_fraction,
    double correct_binary_first_fraction, bool is_finished) {
  unsigned int mark = 0;
  if (is_finished)
    mark = COMPILE_AND_NOT_CRASH_MARKS;
  else
    mark = COMPILE_AND_START_MARKS;

  // functions from Assignment 1
  assert(ASSIGNMENT_1_FUNCTION_MARKS == 4);
  if (correct_value_not_found)
    mark += 1;
  if (correct_linear_search == DATA_SEARCH_COUNT)
    mark += 1;
  if (correct_unsorted_smallest == DATA_FIND_EXTREME_COUNT)
    mark += 1;
  if (correct_unsorted_largest == DATA_FIND_EXTREME_COUNT)
    mark += 1;

  if (sort_result_single.correct == sort_result_single.max)
    mark += 1;
  if (sort_result_ordered.correct == sort_result_ordered.max)
    mark += 1;
  if (sort_result_reverse.correct == sort_result_reverse.max)
    mark += 1;
  if (sort_result_radio.correct == sort_result_radio.max)
    mark += 1;
  if (sort_result_quotation.correct == sort_result_quotation.max)
    mark += 1;
  if (sort_result_duplicates.correct == sort_result_duplicates.max)
    mark += 1;
  if (sort_result_long.correct == sort_result_long.max)
    mark += 1;
  if (sort_result_none.correct == sort_result_none.max)
    mark += 1;

  mark += correct_is_sorted / 4;
  if (correct_sorted_smallest == DATA_FIND_EXTREME_COUNT)
    mark += 1;
  if (correct_sorted_largest == DATA_FIND_EXTREME_COUNT)
    mark += 1;
  mark += calculateMarkBinary(correct_binary_search_fraction,
                              correct_binary_first_fraction);
  // mark += correct_binary_search / 2;
  // mark += 4 * correct_binary_first  / DATA_SEARCH_FIRST_COUNT;

  assert(mark <= TOTAL_MARKS);
  return mark;
}

int calculateMarkBinary(double correct_binary_search_fraction,
                        double correct_binary_first_fraction) {
  int mark = 0;
  mark += (int)(4.0 * correct_binary_search_fraction);
  mark += (int)(4.0 * correct_binary_first_fraction);
  return mark;
}