//
//  TestPlaceString1.cpp
//
//  A test program for the place PlaceString module.
//
//  This program is to be used with Assignment 1: Part B for
//    CS115, 202310.
//
//  Do not modify this file.
//

#include "PlaceString.h"
#include "PlaceString.h" // repeated to test for #include errors

#include <cassert>
#include <iomanip>
#include <iostream>
#include <string>

#include "TestHelper.h"

using namespace std;

bool testStringToInteger(const string &str, int correct_value,
                         bool is_print_result_and_correct,
                         bool is_print_incorrect);
bool testPlaceStringToRow(const string &coord, int correct_row,
                          bool is_print_result_and_correct,
                          bool is_print_incorrect);
bool testPlaceStringToColumn(const string &coord, int correct_row,
                             bool is_print_result_and_correct,
                             bool is_print_incorrect);
bool testIsPlaceStringWellFormed(const string &coord, bool correct_result,
                                 bool is_print_result_and_correct,
                                 bool is_print_incorrect);

int calculateMark(bool correct_invalid_coord,
                  double correct_str_to_int_list_fraction,
                  double correct_str_to_int_range_valid_fraction,
                  double correct_str_to_int_range_invalid_fraction,
                  double correct_get_row_list_fraction,
                  double correct_get_row_grid_yes_fraction,
                  double correct_get_row_grid_no_fraction,
                  double correct_get_column_list_fraction,
                  double correct_get_column_grid_valid_fraction,
                  double correct_get_column_grid_invalid_fraction,
                  double correct_is_well_formed_list_fraction,
                  double correct_is_well_formed_grid_yes_fraction,
                  double correct_is_well_formed_grid_no_fraction,
                  bool is_finished);

const int COMPILE_AND_START_MARKS = 6;
const int DID_NOT_CRASH_RUNNING_MARKS = 6;
const int COMPILE_AND_NOT_CRASH_MARKS =
    COMPILE_AND_START_MARKS + DID_NOT_CRASH_RUNNING_MARKS;
const int COMBINED_TEST_MARKS = 23;
const int TOTAL_MARKS = COMPILE_AND_NOT_CRASH_MARKS + COMBINED_TEST_MARKS;

const int INVALID_COORDINATE_CORRECT = -1;

const unsigned int COORD_LIST_COUNT = 16;
const string COORD_LIST_STRING[COORD_LIST_COUNT] = {
    "A0", "C4", "K9",  "X123", "",  "E-1", "I6", "-1",
    "A*", "~~", "P9G", "O",    "H", " ",   "2B", "15",
};

const int COORD_LIST_ROW[COORD_LIST_COUNT] = {
    0,
    4,
    9,
    123,
    INVALID_COORDINATE_CORRECT,
    INVALID_COORDINATE_CORRECT,
    6,
    1,
    INVALID_COORDINATE_CORRECT,
    INVALID_COORDINATE_CORRECT,
    INVALID_COORDINATE_CORRECT,
    INVALID_COORDINATE_CORRECT,
    INVALID_COORDINATE_CORRECT,
    INVALID_COORDINATE_CORRECT,
    INVALID_COORDINATE_CORRECT,
    5,
};

const int COORD_LIST_COLUMN[COORD_LIST_COUNT] = {
    0,
    2,
    9,
    21,
    INVALID_COORDINATE_CORRECT,
    4,
    INVALID_COORDINATE_CORRECT,
    INVALID_COORDINATE_CORRECT,
    0,
    INVALID_COORDINATE_CORRECT,
    13,
    INVALID_COORDINATE_CORRECT,
    7,
    INVALID_COORDINATE_CORRECT,
    INVALID_COORDINATE_CORRECT,
    INVALID_COORDINATE_CORRECT,
};

int main() {
  TestHelper::startup("TestPlaceString1", COMPILE_AND_START_MARKS, TOTAL_MARKS);

  //
  //  Test constants
  //

  cout << "Testing INVALID_COORDINATE constant" << endl;
  bool correct_invalid_coord = TestHelper::testConstant(
      INVALID_COORDINATE, INVALID_COORDINATE_CORRECT, "INVALID_COORDINATE");
  cout << endl;

  cout << endl;
  cout << endl;

  //
  //  Print results 1
  //

  TestHelper::printSummaryHeaderPartial(32, 2, COMPILE_AND_START_MARKS);
  TestHelper::printSummaryLine("INVALID_COORDINATE", correct_invalid_coord);
  cout << endl;

  unsigned int highest1 = calculateMark(true, 0.0, 0.0, 0.0, // stringToInteger
                                        0.0, 0.0, 0.0,       // coordinateGetRow
                                        0.0, 0.0, 0.0, // coordinateGetColumn
                                        0.0, 0.0, 0.0, // coordinateIsWellFormed
                                        false);        // finished
  unsigned int mark1 =
      calculateMark(correct_invalid_coord, 0.0, 0.0, 0.0, // stringToInteger
                    0.0, 0.0, 0.0,                        // coordinateGetRow
                    0.0, 0.0, 0.0,                        // coordinateGetColumn
                    0.0, 0.0, 0.0, // coordinateIsWellFormed
                    false);        // finished
  assert(mark1 <= TOTAL_MARKS);
  TestHelper::printMarkPartial(mark1, highest1, TOTAL_MARKS);

  cout << endl;
  cout << endl;

  //
  //  Test converting strings to integers
  //

  cout << "Testing stringToInteger:" << endl;
  cout << "------------------------" << endl;
  cout << endl;
  unsigned str_to_int_valid_correct = 0;
  unsigned str_to_int_invalid_correct = 0;

  cout << left << setw(12) << "String" << left << setw(12) << "Returns"
       << "Analysis" << endl;
  cout << left << setw(12) << "------" << left << setw(12) << "-------"
       << "--------" << endl;

  if (testStringToInteger("0", 0, true, true))
    str_to_int_valid_correct++;
  if (testStringToInteger("1", 1, true, true))
    str_to_int_valid_correct++;
  if (testStringToInteger("23", 23, true, true))
    str_to_int_valid_correct++;
  if (testStringToInteger("4567", 4567, true, true))
    str_to_int_valid_correct++;
  if (testStringToInteger("-8", INVALID_COORDINATE_CORRECT, true, true))
    str_to_int_invalid_correct++;
  if (testStringToInteger("", INVALID_COORDINATE_CORRECT, true, true))
    str_to_int_invalid_correct++;
  if (testStringToInteger(" 9", INVALID_COORDINATE_CORRECT, true, true))
    str_to_int_invalid_correct++;
  if (testStringToInteger("ten", INVALID_COORDINATE_CORRECT, true, true))
    str_to_int_invalid_correct++;
  cout << endl;

  cout << "Converting all numbers from -10 to 100:" << endl;
  unsigned str_to_int_range_valid_trials = 0;
  unsigned str_to_int_range_invalid_trials = 0;
  unsigned str_to_int_range_valid_correct = 0;
  unsigned str_to_int_range_invalid_correct = 0;
  bool str_to_int_all_correct = true;
  for (int i = -10; i < 0; i++) {
    str_to_int_range_invalid_trials++;
    bool is_correct =
        testStringToInteger(to_string(i), INVALID_COORDINATE_CORRECT, false,
                            str_to_int_all_correct);
    if (is_correct)
      str_to_int_range_invalid_correct++;
    else if (str_to_int_all_correct) {
      cout << "             For " << i << endl;
      str_to_int_all_correct = false;
    }
  }
  for (int i = 0; i <= 100; i++) {
    str_to_int_range_valid_trials++;
    bool is_correct =
        testStringToInteger(to_string(i), i, false, str_to_int_all_correct);
    if (is_correct)
      str_to_int_range_valid_correct++;
    else if (str_to_int_all_correct) {
      cout << "             For " << i << endl;
      str_to_int_all_correct = false;
    }
  }

  if (str_to_int_all_correct)
    cout << "* Correct" << endl;
  else {
    unsigned int trial_count =
        str_to_int_range_valid_trials + str_to_int_range_invalid_trials;
    unsigned int correct_count =
        str_to_int_range_valid_correct + str_to_int_range_invalid_correct;
    assert(correct_count <= trial_count);
    unsigned int incorrect_count = trial_count - correct_count;
    if (incorrect_count > 1)
      cout << "* " << incorrect_count << " more incorrect" << endl;
  }
  cout << endl;

  cout << endl;
  cout << endl;

  //
  //  Print results 2
  //

  double str_to_int_list_correct_fraction =
      (str_to_int_valid_correct + str_to_int_invalid_correct) / 8.0;
  double str_to_int_range_valid_correct_fraction =
      (double)(str_to_int_range_valid_correct) /
      (double)(str_to_int_range_valid_trials);
  double str_to_int_range_invalid_correct_fraction =
      (double)(str_to_int_range_invalid_correct) /
      (double)(str_to_int_range_invalid_trials);
  double str_to_int_range_total_correct_fraction =
      (double)(str_to_int_range_invalid_correct +
               str_to_int_range_valid_correct) /
      (double)(str_to_int_range_invalid_trials + str_to_int_range_valid_trials);

  TestHelper::printSummaryHeaderPartial(32, 2, COMPILE_AND_START_MARKS);
  TestHelper::printSummaryLine("INVALID_COORDINATE", correct_invalid_coord);
  TestHelper::printSummaryLine("stringToInteger (valid)",
                               str_to_int_valid_correct, 4);
  TestHelper::printSummaryLine("stringToInteger (invalid)",
                               str_to_int_invalid_correct, 4);
  TestHelper::printSummaryLine("stringToInteger (range)",
                               str_to_int_range_total_correct_fraction);

  unsigned int highest2 = calculateMark(true, 1.0, 1.0, 1.0, // stringToInteger
                                        0.0, 0.0, 0.0,       // coordinateGetRow
                                        0.0, 0.0, 0.0, // coordinateGetColumn
                                        0.0, 0.0, 0.0, // coordinateIsWellFormed
                                        false);        // finished
  unsigned int mark2 =
      calculateMark(correct_invalid_coord, str_to_int_list_correct_fraction,
                    str_to_int_range_valid_correct_fraction,
                    str_to_int_range_invalid_correct_fraction, 0.0, 0.0,
                    0.0,           // coordinateGetRow
                    0.0, 0.0, 0.0, // coordinateGetColumn
                    0.0, 0.0, 0.0, // coordinateIsWellFormed
                    false);        // finished
  assert(mark2 <= TOTAL_MARKS);
  TestHelper::printMarkPartial(mark2, highest2, TOTAL_MARKS);

  cout << endl;
  cout << endl;

  //
  //  Test parsing rows
  //

  cout << "Testing placeStringToRow:" << endl;
  cout << "-------------------------" << endl;
  cout << endl;
  cout << left;
  unsigned int get_row_valid_trials = 0;
  unsigned int get_row_invalid_trials = 0;
  unsigned int get_row_valid_correct = 0;
  unsigned int get_row_invalid_correct = 0;

  cout << left << setw(16) << "Place-string" << left << setw(12) << "Returns"
       << "Analysis" << endl;
  cout << left << setw(16) << "------------" << left << setw(12) << "-------"
       << "--------" << endl;

  for (unsigned int i = 0; i < COORD_LIST_COUNT; i++) {
    if (COORD_LIST_ROW[i] == INVALID_COORDINATE_CORRECT)
      get_row_invalid_trials++;
    else
      get_row_valid_trials++;

    string message = "\"" + COORD_LIST_STRING[i] + "\": ";
    cout << setw(16) << message;
    if (testPlaceStringToRow(COORD_LIST_STRING[i], COORD_LIST_ROW[i], true,
                             true)) {
      if (COORD_LIST_ROW[i] == INVALID_COORDINATE_CORRECT)
        get_row_invalid_correct++;
      else
        get_row_valid_correct++;
    }
  }
  cout << endl;

  cout << "Testing all coordinates from A-10 to Z25:" << endl;
  unsigned int get_row_grid_valid_trials = 0;
  unsigned int get_row_grid_invalid_trials = 0;
  unsigned int get_row_grid_valid_correct = 0;
  unsigned int get_row_grid_invalid_correct = 0;
  bool get_row_grid_all_correct = true;
  for (int r = -10; r <= 25; r++) {
    for (int c = 0; c <= 'Z' - 'A'; c++) {
      bool is_valid = r >= 0;
      if (is_valid)
        get_row_grid_valid_trials++;
      else
        get_row_grid_invalid_trials++;

      string coord = string(1, 'A' + c) + to_string(r);
      int correct_row = is_valid ? r : INVALID_COORDINATE_CORRECT;
      bool is_correct = testPlaceStringToRow(coord, correct_row, false,
                                             get_row_grid_all_correct);
      if (is_correct) {
        if (is_valid)
          get_row_grid_valid_correct++;
        else
          get_row_grid_invalid_correct++;
      } else if (get_row_grid_all_correct) {
        cout << "             For \"" << coord << "\"" << endl;
        get_row_grid_all_correct = false;
      }
    }
  }

  if (get_row_grid_all_correct)
    cout << "* Correct" << endl;
  else {
    unsigned int trial_count =
        get_row_grid_valid_trials + get_row_grid_invalid_trials;
    unsigned int correct_count =
        get_row_grid_valid_correct + get_row_grid_invalid_correct;
    assert(correct_count <= trial_count);
    unsigned int incorrect_count = trial_count - correct_count;
    if (incorrect_count > 1)
      cout << "* " << incorrect_count << " more incorrect" << endl;
  }
  cout << endl;

  cout << endl;
  cout << endl;

  //
  //  Test parsing columns
  //

  cout << "Testing placeStringToColumn:" << endl;
  cout << "----------------------------" << endl;
  cout << endl;
  unsigned int get_column_valid_trials = 0;
  unsigned int get_column_invalid_trials = 0;
  unsigned int get_column_valid_correct = 0;
  unsigned int get_column_invalid_correct = 0;

  cout << left << setw(16) << "Place-string" << left << setw(12) << "Returns"
       << "Analysis" << endl;
  cout << left << setw(16) << "------------" << left << setw(12) << "-------"
       << "--------" << endl;

  for (unsigned int i = 0; i < COORD_LIST_COUNT; i++) {
    if (COORD_LIST_COLUMN[i] == INVALID_COORDINATE_CORRECT)
      get_column_invalid_trials++;
    else
      get_column_valid_trials++;

    string message = "\"" + COORD_LIST_STRING[i] + "\"";
    cout << setw(16) << message;
    if (testPlaceStringToColumn(COORD_LIST_STRING[i], COORD_LIST_COLUMN[i],
                                true, true)) {
      if (COORD_LIST_COLUMN[i] == INVALID_COORDINATE_CORRECT)
        get_column_invalid_correct++;
      else
        get_column_valid_correct++;
    }
  }
  cout << endl;

  cout << "Testing all coordinates from A-10 to Z25:" << endl;
  unsigned int get_column_grid_valid_trials = 0;
  unsigned int get_column_grid_invalid_trials = 0;
  unsigned int get_column_grid_valid_correct = 0;
  unsigned int get_column_grid_invalid_correct = 0;
  bool get_column_grid_all_correct = true;
  for (int r = -10; r <= 25; r++) {
    for (int c = 0; c <= 'Z' - 'A'; c++) {
      bool is_valid = (c != 'I' - 'A') && (c != 'O' - 'A');
      if (is_valid)
        get_column_grid_valid_trials++;
      else
        get_column_grid_invalid_trials++;

      string coord = string(1, 'A' + c) + to_string(r);
      int correct_col = INVALID_COORDINATE_CORRECT;
      if (c < 'I' - 'A')
        correct_col = c;
      else if (c > 'I' - 'A' && c < 'O' - 'A')
        correct_col = c - 1;
      else if (c > 'O' - 'A')
        correct_col = c - 2;

      bool is_correct = testPlaceStringToColumn(coord, correct_col, false,
                                                get_column_grid_all_correct);
      if (is_correct) {
        if (is_valid)
          get_column_grid_valid_correct++;
        else
          get_column_grid_invalid_correct++;
      } else if (get_column_grid_all_correct) {
        cout << "             For \"" << coord << "\"" << endl;
        get_column_grid_all_correct = false;
      }
    }
  }

  if (get_column_grid_all_correct)
    cout << "* Correct" << endl;
  else {
    unsigned int trial_count =
        get_column_grid_valid_trials + get_column_grid_invalid_trials;
    unsigned int correct_count =
        get_column_grid_valid_correct + get_column_grid_invalid_correct;
    assert(correct_count <= trial_count);
    unsigned int incorrect_count = trial_count - correct_count;
    if (incorrect_count > 1)
      cout << "* " << incorrect_count << " more incorrect" << endl;
  }
  cout << endl;

  cout << endl;
  cout << endl;

  //
  //  Test checking for well-formed strings
  //

  cout << "Testing isPlaceStringWellFormed:" << endl;
  cout << "--------------------------------" << endl;
  cout << endl;
  unsigned int is_well_formed_yes_trials = 0;
  unsigned int is_well_formed_no_trials = 0;
  unsigned int is_well_formed_yes_correct = 0;
  unsigned int is_well_formed_no_correct = 0;

  cout << left << setw(16) << "Place-string" << left << setw(12) << "Returns"
       << "Analysis" << endl;
  cout << left << setw(16) << "------------" << left << setw(12) << "-------"
       << "--------" << endl;

  for (unsigned int i = 0; i < COORD_LIST_COUNT; i++) {
    bool true_result = COORD_LIST_ROW[i] != INVALID_COORDINATE_CORRECT &&
                       COORD_LIST_COLUMN[i] != INVALID_COORDINATE_CORRECT;
    if (true_result == true)
      is_well_formed_yes_trials++;
    else
      is_well_formed_no_trials++;

    string message = "\"" + COORD_LIST_STRING[i] + "\"";
    cout << setw(16) << message;
    if (testIsPlaceStringWellFormed(COORD_LIST_STRING[i], true_result, true,
                                    true)) {
      if (true_result == true)
        is_well_formed_yes_correct++;
      else
        is_well_formed_no_correct++;
    }
  }
  cout << endl;

  cout << "Testing all coordinates from A-10 to Z25:" << endl;
  unsigned int is_well_formed_grid_yes_trials = 0;
  unsigned int is_well_formed_grid_no_trials = 0;
  unsigned int is_well_formed_grid_yes_correct = 0;
  unsigned int is_well_formed_grid_no_correct = 0;
  bool is_well_formed_grid_all_correct = true;
  for (int r = -10; r <= 25; r++) {
    for (int c = 0; c <= 'Z' - 'A'; c++) {
      bool is_yes = (r >= 0) && (c != 'I' - 'A') && (c != 'O' - 'A');
      if (is_yes)
        is_well_formed_grid_yes_trials++;
      else
        is_well_formed_grid_no_trials++;

      string coord = string(1, 'A' + c) + to_string(r);
      bool is_correct = testIsPlaceStringWellFormed(
          coord, is_yes, false, is_well_formed_grid_all_correct);
      if (is_correct) {
        if (is_yes)
          is_well_formed_grid_yes_correct++;
        else
          is_well_formed_grid_no_correct++;
      } else if (is_well_formed_grid_all_correct) {
        cout << "             For \"" << coord << "\"" << endl;
        is_well_formed_grid_all_correct = false;
      }
    }
  }

  if (is_well_formed_grid_all_correct)
    cout << "* Correct" << endl;
  else {
    unsigned int trial_count =
        is_well_formed_grid_yes_trials + is_well_formed_grid_no_trials;
    unsigned int correct_count =
        is_well_formed_grid_yes_correct + is_well_formed_grid_no_correct;
    assert(correct_count <= trial_count);
    unsigned int incorrect_count = trial_count - correct_count;
    if (incorrect_count > 1)
      cout << "* " << incorrect_count << " more incorrect" << endl;
  }
  cout << endl;

  cout << endl;
  cout << endl;

  //
  //  Print results 3
  //

  double get_row_list_correct_fraction =
      (double)(get_row_valid_correct + get_row_invalid_correct) /
      (double)(get_row_valid_trials + get_row_invalid_trials);
  double get_row_grid_valid_correct_fraction =
      (double)(get_row_grid_valid_correct) /
      (double)(get_row_grid_valid_trials);
  double get_row_grid_invalid_correct_fraction =
      (double)(get_row_grid_invalid_correct) /
      (double)(get_row_grid_invalid_trials);
  double get_row_grid_total_correct_fraction =
      (double)(get_row_grid_valid_correct + get_row_grid_invalid_correct) /
      (double)(get_row_grid_valid_trials + get_row_grid_invalid_trials);

  double get_column_list_correct_fraction =
      (double)(get_column_valid_correct + get_column_invalid_correct) /
      (double)(get_column_valid_trials + get_column_invalid_trials);
  double get_column_grid_valid_correct_fraction =
      (double)(get_column_grid_valid_correct) /
      (double)(get_column_grid_valid_trials);
  double get_column_grid_invalid_correct_fraction =
      (double)(get_column_grid_invalid_correct) /
      (double)(get_column_grid_invalid_trials);
  double get_column_grid_total_correct_fraction =
      (double)(get_column_grid_valid_correct +
               get_column_grid_invalid_correct) /
      (double)(get_column_grid_valid_trials + get_column_grid_invalid_trials);

  double is_well_formed_list_correct_fraction =
      (double)(is_well_formed_yes_correct + is_well_formed_no_correct) /
      (double)(is_well_formed_yes_trials + is_well_formed_no_trials);
  double is_well_formed_grid_yes_correct_fraction =
      (double)(is_well_formed_grid_yes_correct) /
      (double)(is_well_formed_grid_yes_trials);
  double is_well_formed_grid_no_correct_fraction =
      (double)(is_well_formed_grid_no_correct) /
      (double)(is_well_formed_grid_no_trials);
  double is_well_formed_grid_total_correct_fraction =
      (double)(is_well_formed_grid_yes_correct +
               is_well_formed_grid_no_correct) /
      (double)(is_well_formed_grid_yes_trials + is_well_formed_grid_no_trials);

  TestHelper::printSummaryHeaderEndOfFile(32, 2, DID_NOT_CRASH_RUNNING_MARKS);
  TestHelper::printSummaryLine("INVALID_COORDINATE", correct_invalid_coord);
  TestHelper::printSummaryLine("stringToInteger (valid)",
                               str_to_int_valid_correct, 4);
  TestHelper::printSummaryLine("stringToInteger (invalid)",
                               str_to_int_invalid_correct, 4);
  TestHelper::printSummaryLine("stringToInteger (range)",
                               str_to_int_range_total_correct_fraction);
  TestHelper::printSummaryLine("placeStringToRow (valid)",
                               get_row_valid_correct, get_row_valid_trials);
  TestHelper::printSummaryLine("placeStringToRow (invalid)",
                               get_row_invalid_correct, get_row_invalid_trials);
  TestHelper::printSummaryLine("placeStringToRow (range)",
                               get_row_grid_total_correct_fraction);
  TestHelper::printSummaryLine("placeStringToColumn (valid)",
                               get_column_valid_correct,
                               get_column_valid_trials);
  TestHelper::printSummaryLine("placeStringToColumn (invalid)",
                               get_column_invalid_correct,
                               get_column_invalid_trials);
  TestHelper::printSummaryLine("placeStringToColumn (range)",
                               get_column_grid_total_correct_fraction);
  TestHelper::printSummaryLine("isPlaceStringWellFormed (yes)",
                               is_well_formed_yes_correct,
                               is_well_formed_yes_trials);
  TestHelper::printSummaryLine("isPlaceStringWellFormed (no)",
                               is_well_formed_no_correct,
                               is_well_formed_no_trials);
  TestHelper::printSummaryLine("isPlaceStringWellFormed (grid)",
                               is_well_formed_grid_total_correct_fraction);

  unsigned int mark3 = calculateMark(
      correct_invalid_coord, str_to_int_list_correct_fraction,
      str_to_int_range_valid_correct_fraction,
      str_to_int_range_invalid_correct_fraction, get_row_list_correct_fraction,
      get_row_grid_valid_correct_fraction,
      get_row_grid_invalid_correct_fraction, get_column_list_correct_fraction,
      get_column_grid_valid_correct_fraction,
      get_column_grid_invalid_correct_fraction,
      is_well_formed_list_correct_fraction,
      is_well_formed_grid_yes_correct_fraction,
      is_well_formed_grid_no_correct_fraction,
      true); // finished
  assert(mark3 <= TOTAL_MARKS);
  TestHelper::printMark(mark3, TOTAL_MARKS);

  TestHelper::waitForEnter();
  return 0;
}

bool testStringToInteger(const string &str, int correct_value,
                         bool is_print_result_and_correct,
                         bool is_print_incorrect) {
  assert(correct_value >= 0 || correct_value == INVALID_COORDINATE_CORRECT);

  if (is_print_result_and_correct) {
    string quoted = "\"" + str + "\"";
    cout << left << setw(12) << quoted;
  }

  int reported_value = stringToInteger(str);
  if (is_print_result_and_correct) {
    if (reported_value == INVALID_COORDINATE_CORRECT)
      cout << left << setw(12) << "INVALID";
    else
      cout << left << setw(12) << reported_value;
  }

  if (reported_value == correct_value) {
    if (is_print_result_and_correct)
      cout << "Correct" << endl;
    return true;
  }

  if (is_print_incorrect) {
    int first_digit = correct_value;
    while (first_digit >= 10)
      first_digit /= 10;

    int last_digit = correct_value % 10;

    if (reported_value == INVALID_COORDINATE_CORRECT &&
        correct_value != INVALID_COORDINATE_CORRECT)
      cout << "Incorrect: Should not be reported as invalid" << endl;
    else if (reported_value != INVALID_COORDINATE_CORRECT &&
             correct_value == INVALID_COORDINATE_CORRECT)
      cout << "Incorrect: Should reported as invalid" << endl;
    else if (reported_value < 0)
      cout << "Incorrect: Negative values should never be returned" << endl;
    else if (reported_value == first_digit)
      cout << "Incorrect: Only first digit was parsed" << endl;
    else if (reported_value == last_digit)
      cout << "Incorrect: Only last digit was parsed" << endl;
    else
      cout << "Incorrect: Should be " << correct_value << endl;
  }
  return false;
}

bool testPlaceStringToRow(const string &coord, int correct_row,
                          bool is_print_result_and_correct,
                          bool is_print_incorrect) {
  assert(correct_row >= 0 || correct_row == INVALID_COORDINATE_CORRECT);

  int reported_row = placeStringToRow(coord);
  if (is_print_result_and_correct) {
    if (reported_row == INVALID_COORDINATE_CORRECT)
      cout << left << setw(12) << "INVALID";
    else
      cout << left << setw(4) << reported_row << "        ";
  }

  if (reported_row == correct_row) {
    if (is_print_result_and_correct)
      cout << "Correct" << endl;
    return true;
  }

  if (is_print_incorrect) {
    bool is_all_digits = true;
    for (unsigned int i = 1; i < coord.size(); i++) {
      if (!isdigit(coord[i]))
        is_all_digits = false;
    }

    if (reported_row == INVALID_COORDINATE_CORRECT &&
        correct_row != INVALID_COORDINATE_CORRECT)
      cout << "Incorrect: Should not be reported as invalid" << endl;
    else if (coord.size() < 2)
      cout << "Incorrect: Too few characters to be valid" << endl;
    else if (!isdigit(coord[1]))
      cout << "Incorrect: 2nd character must be a digit to be valid" << endl;
    else if (!is_all_digits)
      cout << "Incorrect: All characters after first must be digits to be valid"
           << endl;
    else if (reported_row != INVALID_COORDINATE_CORRECT &&
             correct_row == INVALID_COORDINATE_CORRECT)
      cout << "Incorrect: Should be reported as invalid" << endl;
    else
      cout << "Incorrect: Should be " << correct_row << endl;
  }
  return false;
}

bool testPlaceStringToColumn(const string &coord, int correct_column,
                             bool is_print_result_and_correct,
                             bool is_print_incorrect) {
  assert(correct_column >= 0 || correct_column == INVALID_COORDINATE_CORRECT);

  int reported_column = placeStringToColumn(coord);
  if (is_print_result_and_correct) {
    if (reported_column == INVALID_COORDINATE_CORRECT)
      cout << left << setw(12) << "INVALID";
    else
      cout << left << setw(4) << reported_column << "        ";
  }

  if (reported_column == correct_column) {
    if (is_print_result_and_correct)
      cout << "Correct" << endl;
    return true;
  }

  if (is_print_incorrect) {
    bool is_all_digits = true;
    for (unsigned int i = 1; i < coord.size(); i++) {
      if (!isdigit(coord[i]))
        is_all_digits = false;
    }

    if (reported_column == INVALID_COORDINATE_CORRECT &&
        correct_column != INVALID_COORDINATE_CORRECT)
      cout << "Incorrect: Should not be reported as invalid" << endl;
    else if (coord.size() < 1)
      cout << "Incorrect: Too few characters to be valid" << endl;
    else if (!isupper(coord[0]))
      cout << "Incorrect: 1st character must be an uppercase letter to be valid"
           << endl;
    else if (coord[0] == 'I')
      cout << "Incorrect: Column 'I' should be reported as invalid" << endl;
    else if (coord[0] == 'O')
      cout << "Incorrect: Column 'O' should be reported as invalid" << endl;
    else if (reported_column != INVALID_COORDINATE_CORRECT &&
             correct_column == INVALID_COORDINATE_CORRECT)
      cout << "Incorrect: Should be reported as invalid" << endl;
    else
      cout << "Incorrect: Should be " << correct_column << endl;
  }
  return false;
}

bool testIsPlaceStringWellFormed(const string &coord, bool correct_result,
                                 bool is_print_result_and_correct,
                                 bool is_print_incorrect) {
  bool reported_result = isPlaceStringWellFormed(coord);
  if (is_print_result_and_correct)
    cout << left << setw(12) << reported_result;

  if (reported_result == correct_result) {
    if (is_print_result_and_correct)
      cout << "Correct" << endl;
    return true;
  }

  if (is_print_incorrect) {
    bool is_all_digits = true;
    for (unsigned int i = 1; i < coord.size(); i++) {
      if (!isdigit(coord[i]))
        is_all_digits = false;
    }

    if (reported_result == false && correct_result == true)
      cout << "Incorrect: Should be reported as well-formed" << endl;
    else if (coord.size() < 2)
      cout << "Incorrect: Too few characters to be well-formed" << endl;
    else if (placeStringToRow(coord) == INVALID_COORDINATE_CORRECT)
      cout << "Incorrect: If invalid row, then not well-formed" << endl;
    else if (placeStringToColumn(coord) == INVALID_COORDINATE_CORRECT)
      cout << "Incorrect: If invalid column, then not well-formed" << endl;
    else
      cout << "Incorrect: Should be reported as ill-formed" << endl;
  }
  return false;
}

int calculateMark(bool correct_invalid_coord,
                  double correct_str_to_int_list_fraction,
                  double correct_str_to_int_range_valid_fraction,
                  double correct_str_to_int_range_invalid_fraction,
                  double correct_get_row_list_fraction,
                  double correct_get_row_grid_yes_fraction,
                  double correct_get_row_grid_no_fraction,
                  double correct_get_column_list_fraction,
                  double correct_get_column_grid_valid_fraction,
                  double correct_get_column_grid_invalid_fraction,
                  double correct_is_well_formed_list_fraction,
                  double correct_is_well_formed_grid_yes_fraction,
                  double correct_is_well_formed_grid_no_fraction,
                  bool is_finished) {
  unsigned int mark = 0;
  if (is_finished)
    mark = COMPILE_AND_NOT_CRASH_MARKS;
  else
    mark = COMPILE_AND_START_MARKS;

  if (correct_invalid_coord)
    mark += 2;
  mark += (int)(8 *
                (correct_str_to_int_list_fraction +
                 correct_str_to_int_range_valid_fraction *
                     correct_str_to_int_range_invalid_fraction) *
                0.5);
  mark += (int)(3 *
                (correct_get_row_list_fraction +
                 correct_get_row_grid_yes_fraction *
                     correct_get_row_grid_no_fraction) *
                0.5);
  mark += (int)(6 *
                (correct_get_column_list_fraction +
                 correct_get_column_grid_valid_fraction *
                     correct_get_column_grid_invalid_fraction) *
                0.5);
  mark += (int)(4 *
                (correct_is_well_formed_list_fraction +
                 correct_is_well_formed_grid_yes_fraction *
                     correct_is_well_formed_grid_no_fraction) *
                0.5);

  assert(mark <= TOTAL_MARKS);
  return mark;
}