//
//  TestBoardSize1.cpp
//
//  A test program for the BoardSize module.
//
//  This program is to be used with Assignment 1: Part C for
//    CS115, 202310.
//
//  Do not modify this file.
//

#include "BoardSize.h"
#include "BoardSize.h" // repeated to test for #include errors

#include <cassert>
#include <iomanip>
#include <iostream>
#include <string>

#include "TestHelper.h"

using namespace std;

bool testIsOnBoard(int row, int column, bool correct_validity,
                   bool is_print_result_and_correct, bool is_print_incorrect);

int calculateMark(bool correct_board_size, double correct_list_fraction,
                  double correct_grid_yes_fraction,
                  double correct_grid_no_fraction, bool is_finished);

const int COMPILE_AND_START_MARKS = 2;
const int DID_NOT_CRASH_RUNNING_MARKS = 2;
const int COMPILE_AND_NOT_CRASH_MARKS =
    COMPILE_AND_START_MARKS + DID_NOT_CRASH_RUNNING_MARKS;
const int COMBINED_TEST_MARKS = 6;
const int TOTAL_MARKS = COMPILE_AND_NOT_CRASH_MARKS + COMBINED_TEST_MARKS;

const int BOARD_SIZE_CORRECT = 19;

int main() {
  TestHelper::startup("TestBoardSize1", COMPILE_AND_START_MARKS, TOTAL_MARKS);

  //
  //  Test constants
  //

  cout << "Testing BOARD_SIZE constant" << endl;
  bool correct_board_size =
      TestHelper::testConstant(BOARD_SIZE, BOARD_SIZE_CORRECT, "BOARD_SIZE");
  cout << endl;

  cout << endl;
  cout << endl;

  //
  //  Print results 1
  //

  TestHelper::printSummaryHeaderPartial(24, 1, COMPILE_AND_START_MARKS);
  TestHelper::printSummaryLine("BOARD_SIZE", correct_board_size);
  cout << endl;

  unsigned int highest1 = calculateMark(true, 0.0, 0.0, 0.0, // isOnBoard
                                        false);              // finished
  unsigned int mark1 =
      calculateMark(correct_board_size, 0.0, 0.0, 0.0, // isOnBoard
                    false);                            // finished
  assert(mark1 <= TOTAL_MARKS);
  TestHelper::printMarkPartial(mark1, highest1, TOTAL_MARKS);

  cout << endl;
  cout << endl;

  //
  //  Test checking if places are on the board
  //

  cout << "Testing isOnBoard:" << endl;
  cout << "------------------" << endl;
  cout << endl;
  unsigned int yes_correct = 0;
  unsigned int no_correct = 0;

  cout << left;
  cout << setw(30) << "Is row 0, column 0 valid? ";
  if (testIsOnBoard(0, 0, true, true, true))
    yes_correct++;
  cout << endl;

  cout << setw(30) << "Is row 1, column 2 valid? ";
  if (testIsOnBoard(1, 2, true, true, true))
    yes_correct++;
  cout << endl;

  cout << setw(30) << "Is row 5, column 17 valid? ";
  if (testIsOnBoard(15, 7, true, true, true))
    yes_correct++;
  cout << endl;

  cout << setw(30) << "Is row 18, column 18 valid? ";
  if (testIsOnBoard(18, 18, true, true, true))
    yes_correct++;
  cout << endl;

  cout << setw(30) << "Is row 3, column 19 valid? ";
  if (testIsOnBoard(3, 19, false, true, true))
    no_correct++;
  cout << endl;

  cout << setw(30) << "Is row 6, column -1 valid? ";
  if (testIsOnBoard(6, -1, false, true, true))
    no_correct++;
  cout << endl;

  cout << setw(30) << "Is row 19, column 9 valid? ";
  if (testIsOnBoard(19, 9, false, true, true))
    no_correct++;
  cout << endl;

  cout << setw(30) << "Is row -1, column 16 valid? ";
  if (testIsOnBoard(-1, 16, false, true, true))
    no_correct++;
  cout << endl;

  cout << "Testing all coordinates from -10, -10 to 25, 25:" << endl;
  unsigned int grid_yes_trials = 0;
  unsigned int grid_no_trials = 0;
  unsigned int grid_yes_correct = 0;
  unsigned int grid_no_correct = 0;
  bool grid_all_correct = true;
  for (int r = -10; r <= 25; r++) {
    for (int c = -10; c <= 25; c++) {
      bool is_valid =
          r >= 0 && r < BOARD_SIZE_CORRECT && c >= 0 && c < BOARD_SIZE_CORRECT;
      if (is_valid)
        grid_yes_trials++;
      else
        grid_no_trials++;

      bool is_correct = testIsOnBoard(r, c, is_valid, false, grid_all_correct);
      if (is_correct) {
        if (is_valid)
          grid_yes_correct++;
        else
          grid_no_correct++;
      } else if (grid_all_correct) {
        cout << "             For row = " << r << ", column = " << c << endl;
        grid_all_correct = false;
      }
    }
  }

  if (grid_all_correct)
    cout << "* Correct" << endl;
  else {
    unsigned int trial_count = grid_yes_trials + grid_no_trials;
    unsigned int correct_count = grid_yes_correct + grid_no_correct;
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

  double list_correct_fraction = (yes_correct + no_correct) / 8.0;
  double grid_yes_correct_fraction =
      (double)(grid_yes_correct) / (double)(grid_yes_trials);
  double grid_no_correct_fraction =
      (double)(grid_no_correct) / (double)(grid_no_trials);
  double grid_total_correct_fraction =
      (double)(grid_yes_correct + grid_no_correct) /
      (double)(grid_yes_trials + grid_no_trials);

  TestHelper::printSummaryHeaderEndOfFile(24, 1, DID_NOT_CRASH_RUNNING_MARKS);
  TestHelper::printSummaryLine("BOARD_SIZE", correct_board_size);
  TestHelper::printSummaryLine("isOnBoard (yes)", yes_correct, 4);
  TestHelper::printSummaryLine("isOnBoard (no)", no_correct, 4);
  TestHelper::printSummaryLine("isOnBoard (grid)", grid_total_correct_fraction);

  unsigned int mark3 =
      calculateMark(correct_board_size, list_correct_fraction,
                    grid_yes_correct_fraction, grid_no_correct_fraction,
                    true); // finished
  assert(mark3 <= TOTAL_MARKS);
  TestHelper::printMark(mark3, TOTAL_MARKS);

  TestHelper::waitForEnter();
  return 0;
}

bool testIsOnBoard(int row, int column, bool correct_validity,
                   bool is_print_result_and_correct, bool is_print_incorrect) {
  bool reported_validity = isOnBoard(row, column);
  if (is_print_result_and_correct)
    cout << reported_validity << endl;

  if (reported_validity == correct_validity) {
    if (is_print_result_and_correct)
      cout << "* Correct" << endl;
    return true;
  }

  if (reported_validity == false && correct_validity == true) {
    if (is_print_incorrect)
      cout << "* Incorrect: Should be valid" << endl;
    return false;
  }

  assert(reported_validity == true);
  assert(correct_validity == false);
  if (is_print_incorrect) {
    if (row < 0)
      cout << "* Incorrect: Should be invalid if row is negative" << endl;
    else if (row == BOARD_SIZE_CORRECT)
      cout << "* Incorrect: Should be invalid if row == BOARD_SIZE" << endl;
    else if (row > BOARD_SIZE_CORRECT)
      cout << "* Incorrect: Should be invalid if row > BOARD_SIZE" << endl;
    else if (column < 0)
      cout << "* Incorrect: Should be invalid if column is negative" << endl;
    else if (column == BOARD_SIZE_CORRECT)
      cout << "* Incorrect: Should be invalid if column == BOARD_SIZE" << endl;
    else if (column > BOARD_SIZE_CORRECT)
      cout << "* Incorrect: Should be invalid if column > BOARD_SIZE" << endl;
    else
      cout << "* Incorrect: But why?  This may be a test program error" << endl;
  }
  return false;
}

int calculateMark(bool correct_board_size, double correct_list_fraction,
                  double correct_grid_yes_fraction,
                  double correct_grid_no_fraction, bool is_finished) {
  unsigned int mark = 0;
  if (is_finished)
    mark = COMPILE_AND_NOT_CRASH_MARKS;
  else
    mark = COMPILE_AND_START_MARKS;

  if (correct_board_size)
    mark += 2;
  mark += (int)(2 * correct_list_fraction);
  mark += (int)(2 * correct_grid_yes_fraction * correct_grid_no_fraction);

  assert(mark <= TOTAL_MARKS);
  return mark;
}