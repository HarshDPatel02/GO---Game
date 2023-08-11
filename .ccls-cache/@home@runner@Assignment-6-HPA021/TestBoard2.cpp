//
//  TestBoard2.cpp
//
//  A test program for the Board module.
//
//  This program is to be used with Assignment 2: Part D for
//    CS115, 202310.
//
//  Do not modify this file.
//

#include "Board.h"
#include "Board.h" // repeated to test for #include errors

#include <cassert>
#include <cctype>
#include <iostream>
#include <string>

#include "BoardSize.h"
#include "BoardValue.h"
#include "TestHelper.h"

using namespace std;

int testBoardCountWithValue(const Board &board, int correct_black,
                            int correct_white, bool is_print_correct);
bool testBoardSetAt(Board &board, int row, int column, char value,
                    const string &value_name, bool is_print_correct);

struct PrintResult {
  unsigned int basic_print;
  unsigned int star_points;
  unsigned int star_points_max;
  unsigned int row_headers;
  unsigned int column_headers;
};
PrintResult createZeroPrintResult();
PrintResult testBoardPrint(const Board &board);
string trimRight(const string &str);
void checkPrintedCropped(const Board &board,
                         const vector<string> &captured_lines,
                         unsigned int row_offset, unsigned int column_offset,
                         PrintResult &result);
void checkPrintedCropped(const Board &board, const vector<string> &cropped,
                         PrintResult &result);
bool isCorrectStarPoint(int row, int column);
void checkPrintedRowHeadings(const vector<string> &captured_lines,
                             unsigned int row_offset, PrintResult &result);
bool isAllDigits(const string &str);
void checkPrintedColumnHeadings(const vector<string> &captured_lines,
                                unsigned int column_offset,
                                PrintResult &result);

int calculateMark(
    bool correct_board_size, bool correct_board_bytes,
    unsigned int correct_board_value_constants,
    double correct_after_clear_fraction, unsigned int correct_count_with_value,
    unsigned int correct_set_at, unsigned int correct_count_after_set,
    unsigned int correct_load, double correct_basic_print_fraction,
    double correct_star_point_fraction, double correct_row_header_fraction,
    double correct_column_header_fraction, bool is_finished);

const int EXISTING_WORK_MARKS = 4;
const int COMPILE_AND_START_MARKS = 8 - EXISTING_WORK_MARKS;
const int DID_NOT_CRASH_RUNNING_MARKS = 8;
const int COMPILE_AND_NOT_CRASH_MARKS =
    COMPILE_AND_START_MARKS + DID_NOT_CRASH_RUNNING_MARKS;
const int COMBINED_TEST_MARKS = 29 + EXISTING_WORK_MARKS;
const int TOTAL_MARKS = COMPILE_AND_NOT_CRASH_MARKS + COMBINED_TEST_MARKS;
const int PART_F_MAX_MARKS = TOTAL_MARKS - 5;
const int PART_D_MAX_MARKS = PART_F_MAX_MARKS - 5;

const int BOARD_SIZE_CORRECT = 19;
const int BOARD_CELL_COUNT = BOARD_SIZE_CORRECT * BOARD_SIZE_CORRECT;
const int PRINT_BASIC_WIDTH = BOARD_SIZE_CORRECT * 2 - 1;

const char BOARD_VALUE_EMPTY_CORRECT = '.';
const char BOARD_VALUE_BLACK_CORRECT = 'O';
const char BOARD_VALUE_WHITE_CORRECT = '@';
const unsigned int BOARD_VALUE_COUNT = 3;
const char BOARD_VALUE_STAR_POINT_CORRECT = '*';

const unsigned int BASIC_PRINT_CORRECT_MAX = 10;
const unsigned int ROW_HEADING_CORRECT_MAX = 8;
const unsigned int COLUMN_HEADING_CORRECT_MAX = 8;

const unsigned int LOAD_COUNT = 3;
const unsigned int LOAD_BOARD_VALUE_COUNT = LOAD_COUNT * BOARD_VALUE_COUNT;
const unsigned int LOAD_BASIC_PRINT_CORRECT_MAX =
    LOAD_COUNT * BASIC_PRINT_CORRECT_MAX;
const unsigned int LOAD_ROW_HEADING_CORRECT_MAX =
    LOAD_COUNT * ROW_HEADING_CORRECT_MAX;
const unsigned int LOAD_COLUMN_HEADING_CORRECT_MAX =
    LOAD_COUNT * COLUMN_HEADING_CORRECT_MAX;

int main() {
  TestHelper::startup("TestBoard2", COMPILE_AND_START_MARKS, TOTAL_MARKS);

  //
  //  Test constants and Board struct
  //

  cout << "Testing constants and definitions:" << endl;
  cout << "----------------------------------" << endl;
  cout << endl;

  cout << "Testing BOARD_SIZE constant (from Assignment 1)" << endl;
  bool correct_board_size =
      TestHelper::testConstant(BOARD_SIZE, BOARD_SIZE_CORRECT, "BOARD_SIZE");
  cout << endl;

  cout << "Checking Board size in bytes" << endl;
  size_t board_bytes = sizeof(Board);
  size_t board_bytes_single = sizeof(char);
  size_t board_bytes_row = sizeof(char) * BOARD_SIZE_CORRECT;
  size_t board_bytes_int =
      sizeof(int) * BOARD_SIZE_CORRECT * BOARD_SIZE_CORRECT;
  size_t board_bytes_correct =
      sizeof(char) * BOARD_SIZE_CORRECT * BOARD_SIZE_CORRECT;
  bool correct_board_bytes = false;
  if (board_bytes == board_bytes_single)
    cout << "* Incorrect: Board should contain a 2D array, not a single value"
         << endl;
  else if (board_bytes == board_bytes_row)
    cout << "* Incorrect: Board should contain a 2D array, not a 1D array"
         << endl;
  else if (board_bytes == board_bytes_int)
    cout << "* Incorrect: Board should contain a 2D array of chars, not of ints"
         << endl;
  else if (board_bytes < board_bytes_correct)
    cout << "* Incorrect: Board should contain more data (array too small?)"
         << endl;
  else if (board_bytes >
           board_bytes_correct * 4 / 3) // extra space because padding happens
    cout << "* Incorrect: Board should contain less data (array too large?)"
         << endl;
  else {
    cout << "* Correct" << endl;
    correct_board_bytes = true;
  }
  cout << endl;

  cout << "Creating Board" << endl;
  {
    Board board0; // will be destroyed at end of scope
    cout << "* Completed without crashing" << endl;
    cout << endl;

    (void)(board0); // no-op to silence compiler warnings about unused local
                    // variable

    cout << "Destroying Board" << endl;
  } // board0 is destroyed here
  cout << "* Completed without crashing" << endl;
  cout << endl;

  cout << "Testing BOARD_VALUE_EMPTY constant (from Part C)" << endl;
  bool correct_board_value_empty = TestHelper::testConstant(
      BOARD_VALUE_EMPTY, BOARD_VALUE_EMPTY_CORRECT, "BOARD_VALUE_EMPTY");
  cout << endl;

  cout << "Testing BOARD_VALUE_BLACK constant (from Part C)" << endl;
  bool correct_board_value_black = TestHelper::testConstant(
      BOARD_VALUE_BLACK, BOARD_VALUE_BLACK_CORRECT, "BOARD_VALUE_BLACK");
  cout << endl;

  cout << "Testing BOARD_VALUE_WHITE constant (from Part C)" << endl;
  bool correct_board_value_white = TestHelper::testConstant(
      BOARD_VALUE_WHITE, BOARD_VALUE_WHITE_CORRECT, "BOARD_VALUE_WHITE");
  cout << endl;

  cout << endl;
  cout << endl;

  //
  //  Print results 1
  //

  unsigned int correct_board_value = (correct_board_value_empty ? 1 : 0) +
                                     (correct_board_value_black ? 1 : 0) +
                                     (correct_board_value_white ? 1 : 0);

  TestHelper::printSummaryHeaderPartial(28, 2, COMPILE_AND_START_MARKS);
  TestHelper::printSummaryLine("BOARD_SIZE", correct_board_size);
  TestHelper::printSummaryLine("Board declaration", correct_board_bytes);
  TestHelper::printSummaryLine("BOARD_VALUE_ constants", correct_board_value,
                               BOARD_VALUE_COUNT);

  unsigned int highest1 =
      calculateMark(true, true, BOARD_VALUE_COUNT, // board declaration
                    0.0, 0, 0, 0,                  // basic board manipulation
                    0,                             // load
                    0.0,                           // basic print
                    0.0,                           // print star points
                    0.0, 0.0,                      // print indexes
                    false);                        // finished
  unsigned int mark1 = calculateMark(correct_board_size, correct_board_bytes,
                                     correct_board_value, 0.0, 0, 0,
                                     0,        // basic board manipulation
                                     0,        // load
                                     0.0,      // basic print
                                     0.0,      // print star points
                                     0.0, 0.0, // print indexes
                                     false);   // finished
  assert(mark1 <= TOTAL_MARKS);
  TestHelper::printMarkPartial(mark1, highest1, TOTAL_MARKS);

  cout << endl;
  cout << endl;

  //
  //  Test Board functions
  //

  cout << "Testing basic Board functions:" << endl;
  cout << "------------------------------" << endl;
  cout << endl;

  cout << "Creating board (values are undefined)" << endl;
  Board board2;
  cout << "* Completed without crashing" << endl;
  cout << endl;

  cout << "Clearing board (boardClear)" << endl;
  boardClear(board2);
  cout << "* Completed without crashing" << endl;
  cout << endl;

  cout << "Testing board contents (boardGetAt)" << endl;
  unsigned int correct_after_clear = 0;
  bool is_print_wrong = true;
  for (int r = 0; r < BOARD_SIZE_CORRECT; r++)
    for (int c = 0; c < BOARD_SIZE_CORRECT; c++) {
      char returned1 = boardGetAt(board2, r, c);
      if (returned1 == BOARD_VALUE_EMPTY_CORRECT)
        correct_after_clear++;
      else if (is_print_wrong) {
        cout << "* Incorrect: row " << r << ", column " << c << " is '"
             << returned1 << "', should be BOARD_VALUE_EMPTY ('"
             << BOARD_VALUE_EMPTY_CORRECT << "')" << endl;
        is_print_wrong = false;
      }
    }
  if (correct_after_clear == BOARD_CELL_COUNT)
    cout << "* Correct" << endl;
  else {
    int wrong_count = BOARD_CELL_COUNT - correct_after_clear;
    if (wrong_count > 1)
      cout << "* " << (wrong_count - 1) << " more cells incorrect" << endl;
  }
  cout << endl;

  cout << "Counting cells by type (boardCountWithValue)" << endl;
  unsigned int correct_count_with_value =
      testBoardCountWithValue(board2, 0, 0, true);
  cout << endl;

  bool correct_set_at_1 = testBoardSetAt(
      board2, 4, 2, BOARD_VALUE_BLACK_CORRECT, "BOARD_VALUE_BLACK", true);
  cout << endl;

  cout << "Setting 9 more cells (boardSetAt)" << endl;
  unsigned int correct_set_at_9 = 0;
  if (testBoardSetAt(board2, 4, 3, BOARD_VALUE_BLACK_CORRECT,
                     "BOARD_VALUE_BLACK", false))
    correct_set_at_9++;
  if (testBoardSetAt(board2, 12, 12, BOARD_VALUE_BLACK_CORRECT,
                     "BOARD_VALUE_BLACK", false))
    correct_set_at_9++;
  if (testBoardSetAt(board2, 5, 0, BOARD_VALUE_BLACK_CORRECT,
                     "BOARD_VALUE_BLACK", false))
    correct_set_at_9++;
  if (testBoardSetAt(board2, 5, 3, BOARD_VALUE_BLACK_CORRECT,
                     "BOARD_VALUE_BLACK", false))
    correct_set_at_9++;
  if (testBoardSetAt(board2, 5, 3, BOARD_VALUE_WHITE_CORRECT,
                     "BOARD_VALUE_WHITE", false))
    correct_set_at_9++;
  if (testBoardSetAt(board2, 6, 18, BOARD_VALUE_WHITE_CORRECT,
                     "BOARD_VALUE_WHITE", false))
    correct_set_at_9++;
  if (testBoardSetAt(board2, 18, 1, BOARD_VALUE_WHITE_CORRECT,
                     "BOARD_VALUE_WHITE", false))
    correct_set_at_9++;
  if (testBoardSetAt(board2, 0, 15, BOARD_VALUE_WHITE_CORRECT,
                     "BOARD_VALUE_WHITE", false))
    correct_set_at_9++;
  if (testBoardSetAt(board2, 5, 3, BOARD_VALUE_EMPTY_CORRECT,
                     "BOARD_VALUE_EMPTY", false))
    correct_set_at_9++;
  if (correct_set_at_9 == 9)
    cout << "* Correct" << endl;
  cout << endl;

  cout << "Counting cells by type (boardCountWithValue)" << endl;
  unsigned int correct_count_after_set =
      testBoardCountWithValue(board2, 4, 3, true);
  cout << endl;

  cout << endl;
  cout << endl;

  //
  //  Print results 2
  //

  double correct_after_clear_fraction =
      (double)(correct_after_clear) / BOARD_CELL_COUNT;
  unsigned int correct_set_at = correct_set_at_9 + (correct_set_at_1 ? 1 : 0);

  TestHelper::printSummaryHeaderPartial(28, 2, COMPILE_AND_START_MARKS);
  TestHelper::printSummaryLine("BOARD_SIZE", correct_board_size);
  TestHelper::printSummaryLine("Board declaration", correct_board_bytes);
  TestHelper::printSummaryLine("BOARD_VALUE_ constants", correct_board_value,
                               BOARD_VALUE_COUNT);
  TestHelper::printSummaryLine("boardClear", correct_after_clear_fraction);
  TestHelper::printSummaryLine("boardCountWithValue", correct_count_with_value,
                               BOARD_VALUE_COUNT);
  TestHelper::printSummaryLine("boardSetAt", correct_set_at, 10);
  TestHelper::printSummaryLine("Counts after set", correct_count_after_set,
                               BOARD_VALUE_COUNT);

  unsigned int highest2 = calculateMark(
      true, true, BOARD_VALUE_COUNT,                 // board declaration
      1.0, BOARD_VALUE_COUNT, 10, BOARD_VALUE_COUNT, // basic board manipulation
      0,                                             // load
      0.0,                                           // basic print
      0.0,                                           // print star points
      0.0, 0.0,                                      // print indexes
      false);                                        // finished
  unsigned int mark2 = calculateMark(
      correct_board_size, correct_board_bytes, correct_board_value,
      correct_after_clear_fraction, correct_count_with_value, correct_set_at,
      correct_count_after_set,
      0,        // load
      0.0,      // basic print
      0.0,      // print star points
      0.0, 0.0, // print indexes
      false);   // finished
  assert(mark2 <= TOTAL_MARKS);
  TestHelper::printMarkPartial(mark2, highest2, TOTAL_MARKS);

  cout << endl;
  cout << endl;

  //
  //  Test loading Board
  //

  cout << "Testing boardLoad:" << endl;
  cout << "------------------" << endl;
  cout << endl;

  cout << "Loading \"empty.txt\"" << endl;
  Board board_empty;
  cout << "* Created without crashing" << endl;
  boardLoad(board_empty, "empty.txt");
  cout << "* Loaded without crashing" << endl;
  unsigned int correct_load_empty =
      testBoardCountWithValue(board_empty, 0, 0, true);
  cout << endl;

  cout << "Loading \"ear.txt\"" << endl;
  Board board_ear;
  cout << "* Created without crashing" << endl;
  boardLoad(board_ear, "ear.txt");
  cout << "* Loaded without crashing" << endl;
  unsigned int correct_load_ear =
      testBoardCountWithValue(board_ear, 59, 58, true);
  cout << endl;

  cout << "Loading \"shapes.txt\"" << endl;
  Board board_shapes;
  cout << "* Created without crashing" << endl;
  boardLoad(board_shapes, "shapes.txt");
  cout << "* Loaded without crashing" << endl;
  unsigned int correct_load_shapes =
      testBoardCountWithValue(board_shapes, 96, 96, true);
  cout << endl;

  cout << endl;
  cout << endl;

  //
  //  Print results 3
  //

  unsigned int correct_load =
      correct_load_empty + correct_load_ear + correct_load_shapes;

  TestHelper::printSummaryHeaderPartial(28, 2, COMPILE_AND_START_MARKS);
  TestHelper::printSummaryLine("BOARD_SIZE", correct_board_size);
  TestHelper::printSummaryLine("Board declaration", correct_board_bytes);
  TestHelper::printSummaryLine("BOARD_VALUE_ constants", correct_board_value,
                               BOARD_VALUE_COUNT);
  TestHelper::printSummaryLine("boardClear", correct_after_clear_fraction);
  TestHelper::printSummaryLine("boardCountWithValue", correct_count_with_value,
                               BOARD_VALUE_COUNT);
  TestHelper::printSummaryLine("boardSetAt", correct_set_at, 10);
  TestHelper::printSummaryLine("Counts after set", correct_count_after_set,
                               BOARD_VALUE_COUNT);
  TestHelper::printSummaryLine("boardLoad", correct_load,
                               LOAD_BOARD_VALUE_COUNT);

  unsigned int highest3 = calculateMark(
      true, true, BOARD_VALUE_COUNT,                 // board declaration
      1.0, BOARD_VALUE_COUNT, 10, BOARD_VALUE_COUNT, // basic board manipulation
      LOAD_BOARD_VALUE_COUNT,                        // load
      0.0,                                           // basic print
      0.0,                                           // print star points
      0.0, 0.0,                                      // print indexes
      false);                                        // finished
  unsigned int mark3 = calculateMark(
      correct_board_size, correct_board_bytes, correct_board_value,
      correct_after_clear_fraction, correct_count_with_value, correct_set_at,
      correct_count_after_set, correct_load,
      0.0,      // basic print
      0.0,      // print star points
      0.0, 0.0, // print indexes
      false);   // finished
  assert(mark3 <= TOTAL_MARKS);
  TestHelper::printMarkPartial(mark3, highest3, TOTAL_MARKS);

  cout << endl;
  cout << endl;

  //
  //  Test printing Board
  //

  cout << "Testing boardPrint:" << endl;
  cout << "-------------------" << endl;
  cout << endl;

  cout << "Printing board \"empty.txt\"" << endl;
  PrintResult print_result_empty = createZeroPrintResult();
  if (correct_load_empty == BOARD_VALUE_COUNT)
    print_result_empty = testBoardPrint(board_empty);
  else
    cout << "* Skipping: Not loaded correctly" << endl;
  cout << endl;

  cout << "Printing board \"ear.txt\"" << endl;
  PrintResult print_result_ear = createZeroPrintResult();
  if (correct_load_ear == BOARD_VALUE_COUNT)
    print_result_ear = testBoardPrint(board_ear);
  else
    cout << "* Skipping: Not loaded correctly" << endl;
  cout << endl;

  cout << "Printing board \"shapes.txt\"" << endl;
  PrintResult print_result_shapes = createZeroPrintResult();
  if (correct_load_shapes == BOARD_VALUE_COUNT)
    print_result_shapes = testBoardPrint(board_shapes);
  else
    cout << "* Skipping: Not loaded correctly" << endl;
  cout << endl;

  cout << endl;
  cout << endl;

  //
  //  Print results 4
  //

  unsigned int correct_basic_print = print_result_empty.basic_print +
                                     print_result_ear.basic_print +
                                     print_result_shapes.basic_print;
  double correct_basic_print_fraction =
      (double)(correct_basic_print) / LOAD_BASIC_PRINT_CORRECT_MAX;

  unsigned int correct_star_points = print_result_empty.star_points +
                                     print_result_ear.star_points +
                                     print_result_shapes.star_points;
  unsigned int correct_star_points_max = print_result_empty.star_points_max +
                                         print_result_ear.star_points_max +
                                         print_result_shapes.star_points_max;

  double correct_star_points_fraction = 0.0;
  if (correct_star_points_max > 0)
    correct_star_points_fraction =
        (double)(correct_star_points) / correct_star_points_max;

  unsigned int correct_row_headers = print_result_empty.row_headers +
                                     print_result_ear.row_headers +
                                     print_result_shapes.row_headers;
  double correct_row_headers_fraction =
      (double)(correct_row_headers) / LOAD_ROW_HEADING_CORRECT_MAX;
  unsigned int correct_column_headers = print_result_empty.column_headers +
                                        print_result_ear.column_headers +
                                        print_result_shapes.column_headers;
  double correct_column_headers_fraction =
      (double)(correct_column_headers) / LOAD_COLUMN_HEADING_CORRECT_MAX;

  TestHelper::printSummaryHeaderFinal(28, 2, DID_NOT_CRASH_RUNNING_MARKS);
  TestHelper::printSummaryLine("BOARD_SIZE", correct_board_size);
  TestHelper::printSummaryLine("Board declaration", correct_board_bytes);
  TestHelper::printSummaryLine("BOARD_VALUE_ constants", correct_board_value,
                               BOARD_VALUE_COUNT);
  TestHelper::printSummaryLine("boardClear", correct_after_clear_fraction);
  TestHelper::printSummaryLine("boardCountWithValue", correct_count_with_value,
                               BOARD_VALUE_COUNT);
  TestHelper::printSummaryLine("boardSetAt", correct_set_at, 10);
  TestHelper::printSummaryLine("Counts after set", correct_count_after_set,
                               BOARD_VALUE_COUNT);
  TestHelper::printSummaryLine("boardLoad", correct_load,
                               LOAD_BOARD_VALUE_COUNT);
  TestHelper::printSummaryLine("boardPrint (Part D)",
                               correct_basic_print_fraction);
  TestHelper::printSummaryLine("boardPrint (star points)",
                               correct_star_points_fraction);
  TestHelper::printSummaryLine("boardPrint (row headers)",
                               correct_row_headers_fraction);
  TestHelper::printSummaryLine("boardPrint (column heads)",
                               correct_column_headers_fraction);

  unsigned int mark4 = calculateMark(
      correct_board_size, correct_board_bytes, correct_board_value,
      correct_after_clear_fraction, correct_count_with_value, correct_set_at,
      correct_count_after_set, correct_load, correct_basic_print_fraction,
      correct_star_points_fraction, correct_row_headers_fraction,
      correct_column_headers_fraction,
      true); // finished
  assert(mark4 <= TOTAL_MARKS);
  TestHelper::printMark(mark4, TOTAL_MARKS);

  if (mark4 == PART_D_MAX_MARKS && correct_star_points == 0 &&
      correct_row_headers == 0 && correct_column_headers == 0) {
    cout << "*** Perfect for Part D! ***" << endl;
    cout << endl;
  }
  if (mark4 == PART_F_MAX_MARKS && correct_row_headers == 0 &&
      correct_column_headers == 0) {
    cout << "*** Perfect for Part F! ***" << endl;
    cout << endl;
  }

  TestHelper::waitForEnter();
  return 0;
}

int testBoardCountWithValue(const Board &board, int correct_black,
                            int correct_white, bool is_print_correct) {
  assert(correct_black <= BOARD_CELL_COUNT);
  assert(correct_white <= BOARD_CELL_COUNT);

  assert(correct_black + correct_white <= BOARD_CELL_COUNT);
  int correct_empty = BOARD_CELL_COUNT - correct_black - correct_white;
  assert(correct_empty >= 0);
  assert(correct_empty <= BOARD_CELL_COUNT);

  int empty_count = boardCountWithValue(board, BOARD_VALUE_EMPTY_CORRECT);
  int black_count = boardCountWithValue(board, BOARD_VALUE_BLACK_CORRECT);
  int white_count = boardCountWithValue(board, BOARD_VALUE_WHITE_CORRECT);

  unsigned int result = 0;
  if (empty_count == correct_empty)
    result++;
  else {
    cout << "* Incorrect: Counted " << empty_count << " BOARD_VALUE_EMPTY ('"
         << BOARD_VALUE_EMPTY_CORRECT << "'), should be " << correct_empty
         << endl;
  }
  if (black_count == correct_black)
    result++;
  else {
    cout << "* Incorrect: Counted " << black_count << " BOARD_VALUE_BLACK ('"
         << BOARD_VALUE_BLACK_CORRECT << "'), should be " << correct_black
         << endl;
  }
  if (white_count == correct_white)
    result++;
  else {
    cout << "* Incorrect: Counted " << white_count << " BOARD_VALUE_WHITE ('"
         << BOARD_VALUE_WHITE_CORRECT << "'), should be " << correct_white
         << endl;
  }
  if (result == BOARD_VALUE_COUNT)
    if (is_print_correct)
      cout << "* Correct" << endl;

  return result;
}

bool testBoardSetAt(Board &board, int row, int column, char value,
                    const string &value_name, bool is_print_test_and_correct) {
  assert(row < BOARD_SIZE_CORRECT);
  assert(column < BOARD_SIZE_CORRECT);
  assert(value_name != "");

  if (is_print_test_and_correct) {
    cout << "Setting place " << (char)(column + 'A') << row << " (row " << row
         << ", column " << column << ") to " << value_name << " ('" << value
         << "')" << endl;
  }

  boardSetAt(board, row, column, value);
  if (is_print_test_and_correct)
    cout << "* Completed without crashing" << endl;

  char returned2 = boardGetAt(board, row, column);
  if (returned2 == value) {
    if (is_print_test_and_correct)
      cout << "* Correct" << endl;
    return true;
  } else {
    cout << "* Incorrect: Value at " << (char)(column + 'A') << row << " (row "
         << row << ", column " << column << ") is now '" << returned2
         << "', should be " << value_name << " ('" << value << "')" << endl;
    return false;
  }
}

PrintResult createZeroPrintResult() {
  PrintResult result;
  result.basic_print = 0;
  result.star_points = 0;
  result.star_points_max = 0;
  result.row_headers = 0;
  result.column_headers = 0;
  return result;
}

PrintResult testBoardPrint(const Board &board) {
  static const unsigned int HEIGHT_PART_D = BOARD_SIZE_CORRECT;
  static const unsigned int WIDTH_PART_D = PRINT_BASIC_WIDTH;

  static const unsigned int PART_G_ROW_BORDER = 1;
  static const unsigned int PART_G_COLUMN_BORDER = 3;
  static const unsigned int HEIGHT_PART_G =
      HEIGHT_PART_D + 2 * PART_G_ROW_BORDER;
  static const unsigned int WIDTH_PART_G_MAX =
      WIDTH_PART_D + 2 * PART_G_COLUMN_BORDER;
  static const unsigned int WIDTH_PART_G_MIN = WIDTH_PART_G_MAX - 1;
  // width can vary if numbers are printed left-aligned

  // Step 1: Capture board printing

  TestHelper::printCaptureBlockLine();
  TestHelper::startCapture();
  boardPrint(board);
  string captured = TestHelper::endCapture(true);
  TestHelper::printCaptureBlockLine();
  vector<string> captured_lines = TestHelper::splitWithoutEmpty(captured, '\n');

  PrintResult result = createZeroPrintResult();
  if (captured_lines.empty()) {
    cout << "* Incorrect: Nothing printed" << endl;
    return result;
  }
  result.basic_print++;

  // Step 2: Check output height

  assert(HEIGHT_PART_D < HEIGHT_PART_G);
  unsigned int crop_line_start = 0;
  unsigned int crop_line_end = captured_lines.size();
  if (captured_lines.size() < HEIGHT_PART_D)
    cout << "* Incorrect: Should print at least " << HEIGHT_PART_D << " lines"
         << endl;
  else if (captured_lines.size() > HEIGHT_PART_G)
    cout << "* Incorrect: Should print at most " << HEIGHT_PART_G << " lines"
         << endl;
  else if (captured_lines.size() == HEIGHT_PART_D) {
    assert(crop_line_start <= crop_line_end);
    assert(crop_line_end - crop_line_start == BOARD_SIZE_CORRECT);

    result.basic_print++;
  } else if (captured_lines.size() == HEIGHT_PART_G) {
    crop_line_start++;
    crop_line_end--;
    assert(crop_line_start <= crop_line_end);
    assert(crop_line_end - crop_line_start == BOARD_SIZE_CORRECT);

    result.basic_print++;
  } else {
    cout << "* Incorrect: Should print " << HEIGHT_PART_D
         << " (Parts D - F) or " << HEIGHT_PART_G << " (Part G) lines" << endl;
  }

  // Step 3: Trim spaces at end of lines

  for (unsigned int r = 0; r < captured_lines.size(); r++) {
    captured_lines[r] = trimRight(captured_lines[r]);
  }

  // Step 4: Check output width
  //  -> more complex because lines might be different

  assert(crop_line_start < captured_lines.size());
  unsigned int shortest_line_width = captured_lines[crop_line_start].length();
  unsigned int longest_line_width = shortest_line_width;
  unsigned int shortest_line_index = 0;
  unsigned int longest_line_index = 0;

  for (unsigned int r = crop_line_start; r < crop_line_end; r++) {
    unsigned int line_width = captured_lines[r].length();
    if (line_width < shortest_line_width) {
      shortest_line_width = line_width;
      shortest_line_index = r;
    }
    if (line_width > longest_line_width) {
      longest_line_width = line_width;
      longest_line_index = r;
    }
  }

  if (shortest_line_width == BOARD_SIZE_CORRECT &&
      longest_line_width == BOARD_SIZE_CORRECT) {
    cout << "* Incorrect: All lines " << longest_line_index << " have "
         << longest_line_width << " / " << WIDTH_PART_D << " characters"
         << " - missing spaces?" << endl;
  } else if (longest_line_width < WIDTH_PART_D) {
    cout << "* Incorrect: Longest line " << longest_line_index << " has only "
         << longest_line_width << " / " << WIDTH_PART_D
         << " (Parts D - F) characters" << endl;
  } else if (shortest_line_width < WIDTH_PART_D) {
    cout << "* Incorrect: Shortest line " << shortest_line_index << " has only "
         << shortest_line_width << " / " << WIDTH_PART_D
         << " (Parts D - F) characters" << endl;
  } else if (shortest_line_width > WIDTH_PART_G_MAX) {
    cout << "* Incorrect: Shortest line " << shortest_line_index << " has "
         << shortest_line_width << " / " << WIDTH_PART_G_MAX
         << " (Part G) characters" << endl;
  } else if (longest_line_width > WIDTH_PART_G_MAX) {
    cout << "* Incorrect: Longest line " << longest_line_index << " has "
         << longest_line_width << " / " << WIDTH_PART_G_MAX
         << " (Part G) characters" << endl;
  } else if (shortest_line_width == WIDTH_PART_D &&
             longest_line_width == WIDTH_PART_D) {
    result.basic_print++;

    if (captured_lines.size() == HEIGHT_PART_D)
      checkPrintedCropped(board, captured_lines, 0, 0, result);
    else {
      cout << "* Incorrect: Headings printed for columns but not rows" << endl;
      assert(captured_lines.size() == HEIGHT_PART_G);
      checkPrintedCropped(board, captured_lines, PART_G_ROW_BORDER, 0, result);
      checkPrintedColumnHeadings(captured_lines, 0, result);
    }
  } else if (shortest_line_width >= WIDTH_PART_G_MIN &&
             longest_line_width <= WIDTH_PART_G_MAX) {
    result.basic_print++;

    if (captured_lines.size() == HEIGHT_PART_D) {
      cout << "* Incorrect: Headings printed for rows but not columns" << endl;
      checkPrintedCropped(board, captured_lines, 0, PART_G_COLUMN_BORDER,
                          result);
      checkPrintedRowHeadings(captured_lines, 0, result);
    } else {
      assert(captured_lines.size() == HEIGHT_PART_G);
      checkPrintedCropped(board, captured_lines, PART_G_ROW_BORDER,
                          PART_G_COLUMN_BORDER, result);
      checkPrintedRowHeadings(captured_lines, PART_G_ROW_BORDER, result);
      checkPrintedColumnHeadings(captured_lines, PART_G_COLUMN_BORDER, result);
    }
  } else {
    cout << "* Incorrect: All lines should have " << WIDTH_PART_D
         << " (Parts D - F) or " << WIDTH_PART_G_MAX << " (Part G) characters"
         << endl;
  }

  if (result.basic_print == BASIC_PRINT_CORRECT_MAX &&
      result.star_points == result.star_points_max &&
      result.row_headers == ROW_HEADING_CORRECT_MAX &&
      result.column_headers == COLUMN_HEADING_CORRECT_MAX) {
    cout << "* Correct for Part G" << endl;
  } else if (result.basic_print == BASIC_PRINT_CORRECT_MAX &&
             result.star_points == result.star_points_max &&
             result.row_headers == 0 && result.column_headers == 0) {
    cout << "* Correct for Part F" << endl;
  } else if (result.basic_print == BASIC_PRINT_CORRECT_MAX &&
             result.star_points == 0 && result.row_headers == 0 &&
             result.column_headers == 0) {
    cout << "* Correct for Part D" << endl;
  }
  return result;
}

string trimRight(const string &str) {
  if (str.empty())
    return "";

  string::size_type end;
  for (end = str.size(); end > 0 && isspace(str[end - 1]); end--)
    ; // do nothing

  return str.substr(0, end);
}

void checkPrintedCropped(const Board &board,
                         const vector<string> &captured_lines,
                         unsigned int row_offset, unsigned int column_offset,
                         PrintResult &result) {
  assert(!captured_lines.empty());
  assert(captured_lines.size() >= row_offset);
  assert(captured_lines.size() + row_offset >= BOARD_SIZE_CORRECT);
  for (unsigned int r = 0; r < BOARD_SIZE_CORRECT; r++) {
    assert(r + row_offset < captured_lines.size());
    assert(captured_lines[r + row_offset].length() >= column_offset);
    assert(captured_lines[r + row_offset].length() - column_offset >=
           PRINT_BASIC_WIDTH);
  }

  //  Step 1: Crop captured output

  vector<string> cropped(BOARD_SIZE_CORRECT, "");
  for (unsigned int r = 0; r < BOARD_SIZE_CORRECT; r++) {
    assert(r < cropped.size());
    assert(r + row_offset < captured_lines.size());
    assert(captured_lines[r + row_offset].length() >= column_offset);
    assert(captured_lines[r + row_offset].length() - column_offset >=
           PRINT_BASIC_WIDTH);
    cropped[r] =
        captured_lines[r + row_offset].substr(column_offset, PRINT_BASIC_WIDTH);
  }

  //  Step 2: Check if the spaces are in the correct places

  for (unsigned int r = 0; r < BOARD_SIZE_CORRECT; r++) {
    for (unsigned int c = 0; c < PRINT_BASIC_WIDTH; c++) {
      bool is_space = cropped[r][c] == ' ';
      bool should_be_space = c % 2 != 0;
      if (is_space && !should_be_space) {
        cout << "* Incorrect: Should not be a space at row " << r << " column "
             << (c / 2) << endl;
        return; // end function immediately
      }
    }
  }
  result.basic_print++;

  for (unsigned int r = 0; r < BOARD_SIZE_CORRECT; r++) {
    for (unsigned int c = 0; c < PRINT_BASIC_WIDTH; c++) {
      bool is_space = cropped[r][c] == ' ';
      bool should_be_space = c % 2 != 0;
      if (!is_space && should_be_space) {
        cout << "* Incorrect: After row " << r << " column " << (c / 2)
             << " should be a space" << endl;
        return; // end function immediately
      }
    }
  }
  result.basic_print++;

  //  Step 3: Check if non-space characters are valid

  for (unsigned int r = 0; r < BOARD_SIZE_CORRECT; r++) {
    for (unsigned int c = 0; c < PRINT_BASIC_WIDTH; c += 2) {
      // ignore star points for now
      char here_value = cropped[r][c];
      if (here_value == BOARD_VALUE_STAR_POINT_CORRECT)
        here_value = BOARD_VALUE_EMPTY_CORRECT;

      switch (here_value) {
      case BOARD_VALUE_EMPTY_CORRECT:
      case BOARD_VALUE_BLACK_CORRECT:
      case BOARD_VALUE_WHITE_CORRECT:
        // do nothing
        break;
      default:
        if (isCorrectStarPoint(r, c / 2)) {
          cout << "* Incorrect: Row " << r << " column " << (c / 2)
               << " is bad character '" << here_value << "'" << endl;
          cout << "             Star points should be '*'" << endl;
        } else {
          cout << "* Incorrect: Row " << r << " column " << (c / 2)
               << " is bad character '" << here_value << "'" << endl;
        }
        return; // end function immediately
      }
    }
  }
  result.basic_print += 2;

  //  Step 4: Check if non-space characters are correct

  for (unsigned int r = 0; r < BOARD_SIZE_CORRECT; r++) {
    for (unsigned int c = 0; c < PRINT_BASIC_WIDTH; c += 2) {
      char correct_value = boardGetAt(board, r, c / 2);

      // ignore star points for now
      char here_value = cropped[r][c];
      if (here_value == BOARD_VALUE_STAR_POINT_CORRECT)
        here_value = BOARD_VALUE_EMPTY_CORRECT;

      if (here_value != correct_value) {
        cout << "* Incorrect: Row " << r << " column " << (c / 2) << " is '"
             << here_value << "', should be '" << correct_value << "'" << endl;
        return; // end function immediately
      }
    }
  }
  result.basic_print += 3;

  //  Step 5: Check star points

  checkPrintedCropped(board, cropped, result);
}

void checkPrintedCropped(const Board &board, const vector<string> &cropped,
                         PrintResult &result) {
  assert(!cropped.empty());
  assert(cropped.size() == BOARD_SIZE_CORRECT);
  for (unsigned int r = 0; r < BOARD_SIZE_CORRECT; r++) {
    assert(cropped[r].length() == PRINT_BASIC_WIDTH);
  }

  static const unsigned int STAR_POINT_INDEX_COUNT = 3;
  static const unsigned int STAR_POINT_INDEXES[STAR_POINT_INDEX_COUNT] = {
      3,
      9,
      15,
  };

  //  Step 1: Stop if all star points are missing

  bool is_any_printed = false;
  for (unsigned int r = 0; r < BOARD_SIZE_CORRECT; r++) {
    for (unsigned int c = 0; c < PRINT_BASIC_WIDTH; c += 2) {
      if (isCorrectStarPoint(r, c / 2))
        if (boardGetAt(board, r, c / 2) == BOARD_VALUE_EMPTY_CORRECT)
          result.star_points_max++;

      if (cropped[r][c] == BOARD_VALUE_STAR_POINT_CORRECT)
        is_any_printed = true;
    }
  }
  if (result.star_points_max > 0 && !is_any_printed) {
    // cout << "* Warning: Star points not printed (Part F)" << endl;
    return; // no marks for star points
  }

  //  Step 2: Check star points

  int missed_count = 0;
  int extra_count = 0;
  for (unsigned int r = 0; r < BOARD_SIZE_CORRECT; r++) {
    for (unsigned int c = 0; c < PRINT_BASIC_WIDTH; c += 2) {
      if (isCorrectStarPoint(r, c / 2)) {
        if (boardGetAt(board, r, c / 2) == BOARD_VALUE_EMPTY_CORRECT) {
          if (cropped[r][c] != BOARD_VALUE_STAR_POINT_CORRECT) {
            if (missed_count == 0) {
              cout << "* Incorrect: Star point at row " << r << " column "
                   << (c / 2) << " is missing" << endl;
            }
            missed_count++;
          }
        }
      } else {
        if (cropped[r][c] == BOARD_VALUE_STAR_POINT_CORRECT) {
          if (extra_count == 0) {
            cout << "* Incorrect: Row " << r << " column " << (c / 2)
                 << " should not be a star point" << endl;
          }
          extra_count++;
        }
      }
    }
  }

  // star points are right minus wrong
  int wrong_count = missed_count;
  if (wrong_count < extra_count)
    wrong_count = extra_count;
  if (wrong_count < (int)(result.star_points_max))
    result.star_points = result.star_points_max - wrong_count;
  else
    result.star_points = 0;

  //  Step 3: Messages for additional wrong star points

  int misplaced_count = missed_count;
  if (misplaced_count > extra_count)
    misplaced_count = extra_count;
  if (misplaced_count > 1) {
    cout << "* Incorrect: " << (misplaced_count - 1)
         << " more star points misplaced" << endl;
    assert(misplaced_count <= missed_count);
    missed_count -= misplaced_count;
    assert(misplaced_count <= extra_count);
    extra_count -= misplaced_count;
  }
  if (extra_count > 1)
    cout << "* Incorrect: " << (extra_count - 1)
         << " more extra star points printed" << endl;
  if (missed_count > 1)
    cout << "* Incorrect: " << (missed_count - 1) << " more star points missing"
         << endl;
}

bool isCorrectStarPoint(int row, int column) {
  static const unsigned int STAR_POINT_INDEX_COUNT = 3;
  static const unsigned int STAR_POINT_INDEXES[STAR_POINT_INDEX_COUNT] = {
      3,
      9,
      15,
  };

  bool is_star_row = false;
  for (unsigned int i = 0; i < STAR_POINT_INDEX_COUNT; i++)
    if (row == STAR_POINT_INDEXES[i])
      is_star_row = true;
  if (!is_star_row)
    return false;

  bool is_star_column = false;
  for (unsigned int i = 0; i < STAR_POINT_INDEX_COUNT; i++)
    if (column == STAR_POINT_INDEXES[i])
      is_star_column = true;
  if (!is_star_column)
    return false;

  return true;
}

void checkPrintedRowHeadings(const vector<string> &captured_lines,
                             unsigned int row_offset, PrintResult &result) {
  static const unsigned int NUMBER_WIDTH = 2;
  static const unsigned int LEFT_SPACE = NUMBER_WIDTH;
  static const unsigned int RIGHT_SPACE = LEFT_SPACE + BOARD_SIZE_CORRECT * 2;
  static const unsigned int TOTAL_WIDTH = RIGHT_SPACE + NUMBER_WIDTH;

  assert(!captured_lines.empty());
  assert(captured_lines.size() >= row_offset);
  assert(captured_lines.size() + row_offset >= BOARD_SIZE_CORRECT);
  for (unsigned int r = 0; r < BOARD_SIZE_CORRECT; r++) {
    assert(r + row_offset < captured_lines.size());
    assert(captured_lines[r + row_offset].length() > RIGHT_SPACE);
  }

  //  Step 1: Check if there are spoaces between indexes and board

  for (unsigned int r = 0; r < BOARD_SIZE_CORRECT; r++) {
    assert(r + row_offset < captured_lines.size());
    assert(captured_lines[r + row_offset].length() >= LEFT_SPACE);
    if (captured_lines[r + row_offset][LEFT_SPACE] != ' ') {
      cout << "* Incorrect: Should be a space after row " << r << " left index"
           << endl;
      return; // end function immediately
    }
  }
  result.row_headers++;

  for (unsigned int r = 0; r < BOARD_SIZE_CORRECT; r++) {
    assert(r + row_offset < captured_lines.size());
    assert(captured_lines[r + row_offset].length() >= RIGHT_SPACE);
    if (captured_lines[r + row_offset][RIGHT_SPACE] != ' ') {
      cout << "* Incorrect: Should be a space before row " << r
           << " right index" << endl;
      return; // end function immediately
    }
  }
  result.row_headers++;

  //  Step 2: Crop out row indexes

  vector<string> left(BOARD_SIZE_CORRECT, "");
  vector<string> right(BOARD_SIZE_CORRECT, "");
  for (unsigned int r = 0; r < BOARD_SIZE_CORRECT; r++) {
    assert(r < left.size());
    assert(r < right.size());
    assert(r + row_offset < captured_lines.size());
    const string &line = captured_lines[r + row_offset];
    assert(line.length() > RIGHT_SPACE);
    left[r] = TestHelper::trim(line.substr(0, NUMBER_WIDTH));
    right[r] = TestHelper::trim(line.substr(RIGHT_SPACE + 1, NUMBER_WIDTH));
  }

  //  Step 3: Check if row indexes are present

  for (unsigned int r = 0; r < BOARD_SIZE_CORRECT; r++) {
    assert(r < left.size());
    if (left[r] == "") {
      cout << "* Incorrect: No left index for row " << r << endl;
      return; // end function immediately
    }
  }
  result.row_headers++;

  for (unsigned int r = 0; r < BOARD_SIZE_CORRECT; r++) {
    assert(r < right.size());
    if (right[r] == "") {
      cout << "* Incorrect: No right index for row " << r << endl;
      return; // end function immediately
    }
  }
  result.row_headers++;

  //  Step 4: Check if row indexes are numbers

  for (unsigned int r = 0; r < BOARD_SIZE_CORRECT; r++) {
    assert(r < left.size());
    if (!isAllDigits(left[r])) {
      cout << "* Incorrect: left index for row " << r << " should be a number"
           << endl;
      return; // end function immediately
    }
  }
  result.row_headers++;

  for (unsigned int r = 0; r < BOARD_SIZE_CORRECT; r++) {
    assert(r < right.size());
    if (!isAllDigits(right[r])) {
      cout << "* Incorrect: No right index for row " << r
           << " should be a number" << endl;
      return; // end function immediately
    }
  }
  result.row_headers++;

  //  Step 5: Check if row indexes are correct

  for (unsigned int r = 0; r < BOARD_SIZE_CORRECT; r++) {
    string correct = to_string(r);
    assert(r < left.size());
    if (left[r] != correct) {
      cout << "* Incorrect: Left index for row " << r << " is \"" << left[r]
           << "\", but should be \"" << correct << "\"" << endl;
      return; // end function immediately
    }
  }
  result.row_headers++;

  for (unsigned int r = 0; r < BOARD_SIZE_CORRECT; r++) {
    string correct = to_string(r);
    assert(r < right.size());
    if (right[r] != correct) {
      cout << "* Incorrect: Right index for row " << r << " is \"" << right[r]
           << "\", but should be \"" << correct << "\"" << endl;
      return; // end function immediately
    }
  }
  result.row_headers++;
}

bool isAllDigits(const std::string &str) {
  for (unsigned int i = 0; i < str.length(); i++)
    if (!isdigit(str[i]))
      return false;
  return true;
}

void checkPrintedColumnHeadings(const vector<string> &captured_lines,
                                unsigned int column_offset,
                                PrintResult &result) {
  assert(!captured_lines.empty());
  assert(captured_lines.front().length() >= column_offset);
  assert(captured_lines.back().length() >= column_offset);

  static const unsigned int CORRECT_WIDTH = BOARD_SIZE_CORRECT * 2 - 1;
  static const string CORRECT_LETTERS = "ABCDEFGHJKLMNPQRSTU";
  assert(CORRECT_LETTERS.length() == BOARD_SIZE_CORRECT);

  //  Step 1: Check if the right amount is printed

  unsigned int top_width = CORRECT_WIDTH;
  if (top_width > captured_lines.front().length() - column_offset)
    top_width = captured_lines.front().length() - column_offset;
  string top = captured_lines.front().substr(column_offset, top_width);
  assert(top.length() == top_width);

  unsigned int bottom_width = CORRECT_WIDTH;
  if (bottom_width > captured_lines.back().length() - column_offset)
    bottom_width = captured_lines.back().length() - column_offset;
  string bottom = captured_lines.front().substr(column_offset, top_width);
  assert(bottom.length() == bottom_width);

  if (top_width < CORRECT_WIDTH) {
    cout << "* Incorrect: Top headings have only " << top_width << " / "
         << CORRECT_WIDTH << " characters" << endl;
    return; // end function immediately
  }
  result.column_headers++;

  if (bottom_width < CORRECT_WIDTH) {
    cout << "* Incorrect: Bottom headings have only " << top_width << " / "
         << CORRECT_WIDTH << " characters" << endl;
    return; // end function immediately
  }
  result.column_headers++;

  //  Step 2: Check if the spaces are in the correct places

  for (unsigned int c = 0; c < top.length(); c++) {
    bool is_space = top[c] == ' ';
    bool should_be_space = c % 2 != 0;
    if (is_space != should_be_space) {
      if (is_space)
        cout << "* Incorrect: Should not be a space above column " << (c / 2)
             << endl;
      else
        cout << "* Incorrect: After above column " << (c / 2)
             << " should be a space" << endl;
      return; // end function immediately
    }
  }
  result.column_headers++;

  for (unsigned int c = 0; c < bottom.length(); c++) {
    bool is_space = bottom[c] == ' ';
    bool should_be_space = c % 2 != 0;
    if (is_space != should_be_space) {
      if (is_space)
        cout << "* Incorrect: Should not be a space below column " << (c / 2)
             << endl;
      else
        cout << "* Incorrect: After below column " << (c / 2)
             << " should be a space" << endl;
      return; // end function immediately
    }
  }
  result.column_headers++;

  //  Step 3: Check if the column headings are uppercase letters

  for (unsigned int c = 0; c < top.length(); c += 2) {
    if (!isupper(top[c])) {
      if (isalpha(top[c]))
        cout << "* Incorrect: Above column " << (c / 2)
             << " should be a uppercase" << endl;
      else
        cout << "* Incorrect: Above column " << (c / 2) << " should be a letter"
             << endl;
      return; // end function immediately
    }
  }
  result.column_headers++;

  for (unsigned int c = 0; c < bottom.length(); c += 2) {
    if (!isupper(bottom[c])) {
      if (isalpha(bottom[c]))
        cout << "* Incorrect: Below column " << (c / 2)
             << " should be a uppercase" << endl;
      else
        cout << "* Incorrect: Below column " << (c / 2) << " should be a letter"
             << endl;
      return; // end function immediately
    }
  }
  result.column_headers++;

  //  Step 4: Check if the column headings are correct letters

  for (unsigned int c = 0; c < top.length(); c += 2) {
    assert(c / 2 < CORRECT_LETTERS.length());
    char correct_letter = CORRECT_LETTERS[c / 2];
    if (top[c] != correct_letter) {
      cout << "* Incorrect: Above column " << (c / 2) << " is '" << top[c]
           << "', should be '" << correct_letter << "'" << endl;
      return; // end function immediately
    }
  }
  result.column_headers++;

  for (unsigned int c = 0; c < bottom.length(); c += 2) {
    assert(c / 2 < CORRECT_LETTERS.length());
    char correct_letter = CORRECT_LETTERS[c / 2];
    if (bottom[c] != correct_letter) {
      cout << "* Incorrect: Below column " << (c / 2) << " is '" << bottom[c]
           << "', should be '" << correct_letter << "'" << endl;
      return; // end function immediately
    }
  }
  result.column_headers++;
}

int calculateMark(
    bool correct_board_size, bool correct_board_bytes,
    unsigned int correct_board_value_constants,
    double correct_after_clear_fraction, unsigned int correct_count_with_value,
    unsigned int correct_set_at, unsigned int correct_count_after_set,
    unsigned int correct_load, double correct_basic_print_fraction,
    double correct_star_point_fraction, double correct_row_header_fraction,
    double correct_column_header_fraction, bool is_finished) {
  unsigned int mark = 0;
  if (is_finished)
    mark = COMPILE_AND_NOT_CRASH_MARKS;
  else
    mark = COMPILE_AND_START_MARKS;

  // from Assignment 1 and Part C
  assert(EXISTING_WORK_MARKS == 4);
  if (correct_board_size)
    mark += 1;
  mark += correct_board_value_constants;

  // Part D
  if (correct_board_bytes)
    mark += 2;
  mark += (int)(4 * correct_after_clear_fraction);
  mark += TestHelper::markAnyAll(correct_count_with_value, BOARD_VALUE_COUNT);
  mark += TestHelper::markAnyAll(correct_set_at, 10);
  mark += TestHelper::markAnyAll(correct_count_after_set, BOARD_VALUE_COUNT);
  if (correct_load)
    mark += 2;
  mark += (int)(5 * correct_basic_print_fraction);

  // Part F
  mark += (int)(5 * correct_star_point_fraction);

  // Part G
  mark += (int)(2.5 *
                (correct_row_header_fraction + correct_column_header_fraction));

  assert(mark <= TOTAL_MARKS);
  return mark;
}