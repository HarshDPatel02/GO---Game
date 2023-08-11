//
//  TestBoardValue2.cpp
//
//  A test program for the BoardValue module.
//
//  This program is to be used with Assignment 2: Part C for
//    CS115, 202310.
//
//  Do not modify this file.
//

#include "BoardValue.h"
#include "BoardValue.h" // repeated to test for #include errors

#include <cassert>
#include <iostream>
#include <string>

#include "TestHelper.h"

using namespace std;

int calculateMark(bool correct_board_value_empty,
                  bool correct_board_value_black,
                  bool correct_board_value_white, double correct_valid_fraction,
                  double correct_invalid_fraction, bool is_finished);

const int COMPILE_AND_START_MARKS = 1;
const int DID_NOT_CRASH_RUNNING_MARKS = 1;
const int COMPILE_AND_NOT_CRASH_MARKS =
    COMPILE_AND_START_MARKS + DID_NOT_CRASH_RUNNING_MARKS;
const int COMBINED_TEST_MARKS = 3;
const int TOTAL_MARKS = COMPILE_AND_NOT_CRASH_MARKS + COMBINED_TEST_MARKS;

const char BOARD_VALUE_EMPTY_CORRECT = '.';
const char BOARD_VALUE_BLACK_CORRECT = 'O';
const char BOARD_VALUE_WHITE_CORRECT = '@';

const char CHECK_CHAR_FIRST = (char)(32);
const char CHECK_CHAR_LAST = (char)(126);
const int CHECK_CHAR_TOTAL_COUNT = CHECK_CHAR_LAST - CHECK_CHAR_FIRST;
const int CHECK_CHAR_VALID_COUNT = 3;
const int CHECK_CHAR_INVALID_COUNT =
    CHECK_CHAR_TOTAL_COUNT - CHECK_CHAR_VALID_COUNT + 1;

int main() {
  TestHelper::startup("TestBoardValue2", COMPILE_AND_START_MARKS, TOTAL_MARKS);

  //
  //  Test constants
  //

  cout << "Testing BOARD_VALUE_EMPTY constant" << endl;
  bool correct_board_value_empty = TestHelper::testConstant(
      BOARD_VALUE_EMPTY, BOARD_VALUE_EMPTY_CORRECT, "BOARD_VALUE_EMPTY");
  cout << endl;

  cout << "Testing BOARD_VALUE_BLACK constant" << endl;
  bool correct_board_value_black = TestHelper::testConstant(
      BOARD_VALUE_BLACK, BOARD_VALUE_BLACK_CORRECT, "BOARD_VALUE_BLACK");
  cout << endl;

  cout << "Testing BOARD_VALUE_WHITE constant" << endl;
  bool correct_board_value_white = TestHelper::testConstant(
      BOARD_VALUE_WHITE, BOARD_VALUE_WHITE_CORRECT, "BOARD_VALUE_WHITE");
  cout << endl;

  cout << endl;
  cout << endl;

  //
  //  Print results 1
  //

  TestHelper::printSummaryHeaderPartial(26, 2, COMPILE_AND_START_MARKS);
  TestHelper::printSummaryLine("BOARD_VALUE_EMPTY", correct_board_value_empty);
  TestHelper::printSummaryLine("BOARD_VALUE_BLACK", correct_board_value_black);
  TestHelper::printSummaryLine("BOARD_VALUE_WHITE", correct_board_value_white);

  unsigned int highest1 = calculateMark(true, true, true, // constants
                                        0.0, 0.0,         // isBoardValueValid
                                        false);           // finished
  unsigned int mark1 =
      calculateMark(correct_board_value_empty, correct_board_value_black,
                    correct_board_value_white, 0.0, 0.0, // isBoardValueValid
                    false);                              // finished
  assert(mark1 <= TOTAL_MARKS);
  TestHelper::printMarkPartial(mark1, highest1, TOTAL_MARKS);

  cout << endl;
  cout << endl;

  //
  //  Test function
  //

  cout << "Testing isBoardValueValid:" << endl;
  cout << "--------------------------" << endl;
  cout << endl;

  unsigned int correct_valid = 0;
  unsigned int correct_invalid = 0;
  bool is_print_wrong = true;

  cout << "Testing for all chars from '" << CHECK_CHAR_FIRST << "' ("
       << (int)(CHECK_CHAR_FIRST) << ") to '" << CHECK_CHAR_LAST << "' ("
       << (int)(CHECK_CHAR_LAST) << ")" << endl;
  for (int c = CHECK_CHAR_FIRST; c <= CHECK_CHAR_LAST; c++) {
    bool correct =
        (c == BOARD_VALUE_EMPTY_CORRECT)
            ? true
            : ((c == BOARD_VALUE_BLACK_CORRECT)
                   ? true
                   : ((c == BOARD_VALUE_WHITE_CORRECT) ? true : false));
    bool result = isBoardValueValid(c);

    if (result == correct) {
      if (result == true)
        correct_valid++;
      else
        correct_invalid++;
    } else {
      if (result == true) {
        if (is_print_wrong)
          cout << "* Incorrect: char '" << (char)(c) << "' should be invalid"
               << endl;
        is_print_wrong = false;
      } else {
        // not too many of these, so just print all
        cout << "* Incorrect: char '" << (char)(c) << "' should be valid"
             << endl;
      }
    }
  }

  if (correct_valid == CHECK_CHAR_VALID_COUNT &&
      correct_invalid == CHECK_CHAR_INVALID_COUNT) {
    cout << "* Correct" << endl;
  } else {
    int wrong_count = CHECK_CHAR_INVALID_COUNT - correct_invalid;
    if (wrong_count > 1)
      cout << "* " << (wrong_count - 1) << " more should be invalid" << endl;
  }

  cout << endl;
  cout << endl;

  //
  //  Print results 2
  //

  double correct_valid_fraction =
      (double)(correct_valid) / CHECK_CHAR_VALID_COUNT;
  double correct_invalid_fraction =
      (double)(correct_invalid) / CHECK_CHAR_INVALID_COUNT;

  TestHelper::printSummaryHeaderFinal(26, 2, DID_NOT_CRASH_RUNNING_MARKS);
  TestHelper::printSummaryLine("BOARD_VALUE_EMPTY", correct_board_value_empty);
  TestHelper::printSummaryLine("BOARD_VALUE_BLACK", correct_board_value_black);
  TestHelper::printSummaryLine("BOARD_VALUE_WHITE", correct_board_value_white);
  TestHelper::printSummaryLine("isBoardValueValid (yes)", correct_valid,
                               CHECK_CHAR_VALID_COUNT);
  TestHelper::printSummaryLine("isBoardValueValid (no)", correct_invalid,
                               CHECK_CHAR_INVALID_COUNT);

  unsigned int mark2 =
      calculateMark(correct_board_value_empty, correct_board_value_black,
                    correct_board_value_white, correct_valid_fraction,
                    correct_invalid_fraction,
                    true); // finished
  assert(mark2 <= TOTAL_MARKS);
  TestHelper::printMark(mark2, TOTAL_MARKS);

  TestHelper::waitForEnter();
  return 0;
}

int calculateMark(bool correct_board_value_empty,
                  bool correct_board_value_black,
                  bool correct_board_value_white, double correct_valid_fraction,
                  double correct_invalid_fraction, bool is_finished) {
  unsigned int mark = 0;
  if (is_finished)
    mark = COMPILE_AND_NOT_CRASH_MARKS;
  else
    mark = COMPILE_AND_START_MARKS;

  if (correct_board_value_empty && correct_board_value_black &&
      correct_board_value_white) {
    mark += 1;
  }

  mark += (int)(2 * correct_valid_fraction * correct_invalid_fraction);

  assert(mark <= TOTAL_MARKS);
  return mark;
}