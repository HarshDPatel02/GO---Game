//
//  TestArtificialIntelligence4.cpp
//
//  A test program for the ArtificialIntelligence module.
//
//  This program is to be used with Assignment 4: Part E for
//    CS115, 202310.
//
//  Do not modify this file.
//

#include "ArtificialIntelligence.h"
#include "ArtificialIntelligence.h"  // repeated to test for #include errors

#include <cassert>
#include <cctype>
#include <string>
#include <iostream>
#include <iomanip>

#include "TestHelper.h"
#include "BoardSize.h"
#include "BoardValue.h"
#include "Board.h"

using namespace std;



bool testMove (Board& r_board,
               unsigned int& r_pass_count,
               const Move& move,
               bool is_print_incorrect);
void fillBoardCenter (Board& r_board,
                      char board_value);
void fillBoardEyes (Board& r_board,
                    char board_value);
void fillBoardJunctions (Board& r_board,
                         char board_value);
int countInRectangle (const Board& board,
                      int column_min, int column_max,
                      int row_min, int row_max,
                      char board_value);
bool checkRectangle (const Board& board,
                     int column_min, int column_max,
                     int row_min, int row_max,
                     char board_value,
                     int desired_count,
                     const string& area_name,
                     bool is_print_incorrect);
unsigned int checkJunctions (const Board& board,
                             char board_value,
                             int desired_count);

int calculateMark (bool correct_move_bytes,
                   bool correct_choose_1,
                   double correct_choose_small_fraction,
                   double correct_choose_large_fraction,
                   double territory_fraction_fraction,
                   double correct_black_eye_fraction,
                   double correct_white_eye_fraction,
                   double correct_black_junction_fraction,
                   double correct_white_junction_fraction,
                   bool is_finished);

const int RANDOM_DISTRIBUTION_MARKS   = 2;
const int COMPILE_AND_START_MARKS     = 5;
const int DID_NOT_CRASH_RUNNING_MARKS = 5;
const int COMPILE_AND_NOT_CRASH_MARKS = COMPILE_AND_START_MARKS + DID_NOT_CRASH_RUNNING_MARKS;
const int COMBINED_TEST_MARKS         = 20 - RANDOM_DISTRIBUTION_MARKS;
const int TOTAL_MARKS                 = COMPILE_AND_NOT_CRASH_MARKS + COMBINED_TEST_MARKS;

const int BOARD_SIZE_CORRECT = 19;
const int BOARD_CELL_COUNT   = BOARD_SIZE_CORRECT * BOARD_SIZE_CORRECT;
const int TRIAL_SMALL        = 5;
const int TRIAL_LARGE        = 25;
const int TRIAL_TERRITORY    = 25;
const int TRIAL_COUNT        = 1 + TRIAL_SMALL + TRIAL_LARGE;
const int TRIAL_JUNCTIONS    = 4;

const char BOARD_VALUE_EMPTY_CORRECT = '.';
const char BOARD_VALUE_BLACK_CORRECT = 'O';
const char BOARD_VALUE_WHITE_CORRECT = '@';



int main ()
{
	TestHelper::startup("TestArtificialIntelligence4", COMPILE_AND_START_MARKS, TOTAL_MARKS);

	//
	//  Test constants and ArtificialIntelligence struct
	//

	cout << "Creating ArtificialIntelligence" << endl;
	{
		ArtificialIntelligence ai0(BOARD_VALUE_WHITE_CORRECT);  // will be destroyed at end of scope
		cout << "* Completed without crashing" << endl;
		cout << endl;

		(void)(ai0);  // no-op to silence compiler warnings about unused local variable

		cout << "Destroying ArtificialIntelligence" << endl;
	}  // ai0 is destroyed here
	cout << "* Completed without crashing" << endl;
	cout << endl;

	cout << "Checking Move size in bytes" << endl;
	size_t move_bytes         = sizeof(Move);
	size_t move_bytes_pass    = sizeof(bool);
	size_t move_bytes_place   = sizeof(int) * 2;
	size_t move_bytes_correct = sizeof(int) * 2 + sizeof(bool);
	bool correct_move_bytes = false;
	if(move_bytes == move_bytes_pass)
		cout << "* Incorrect: Move should also contain a board place" << endl;
	else if(move_bytes == move_bytes_place)
		cout << "* Incorrect: Move should also contain whether white passed" << endl;
	else if(move_bytes < move_bytes_correct)
		cout << "* Incorrect: Move should contain more data" << endl;
	else if(move_bytes > move_bytes_correct * 2)  // extra space because padding happens
		cout << "* Incorrect: Move should contain less data" << endl;
	else
	{
		cout << "* Correct" << endl;
		correct_move_bytes = true;
	}
	cout << endl;

	cout << "Creating Move" << endl;
	{
		Move move0;  // will be destroyed at end of scope
		cout << "* Completed without crashing" << endl;
		cout << endl;

		(void)(move0);  // no-op to silence compiler warnings about unused local variable

		cout << "Destroying Move" << endl;
	}  // move0 is destroyed here
	cout << "* Completed without crashing" << endl;
	cout << endl;

	cout << endl;
	cout << endl;


	//
	//  Print results 1
	//

	TestHelper::printSummaryHeaderPartial(24, 2, COMPILE_AND_START_MARKS);
	TestHelper::printSummaryLine("Move declaration", correct_move_bytes);

	unsigned int highest1 = calculateMark(true,         // Move declaration
	                                      0, 0.0, 0.0,  // chooseMove
	                                      0.0,          // fraction of territory controlled
	                                      0.0, 0.0,     // fraction passing
	                                      0.0, 0.0,     // fraction capturing territory
	                                      false);       // finished
	unsigned int mark1 = calculateMark(correct_move_bytes,
	                                   0, 0.0, 0.0,  // chooseMove
	                                   0.0,          // fraction of territory controlled
	                                   0.0, 0.0,     // fraction passing
	                                   0.0, 0.0,     // fraction capturing territory
	                                   false);       // finished
	assert(mark1 <= TOTAL_MARKS);
	TestHelper::printMarkPartial(mark1, highest1, TOTAL_MARKS);

	cout << endl;
	cout << endl;


	//
	//  Test choosing moves (Part 1)
	//

	cout << "Testing filling territory (chooseMove):" << endl;
	cout << "---------------------------------------" << endl;
	cout << endl;

	cout << "Creating empty board" << endl;
	Board board0;
	cout << "* Completed without crashing" << endl;
	cout << endl;

	cout << "Loading mostly-full board (faster)" << endl;
	board0.load("19to5.txt");
	cout << "* Completed without crashing" << endl;
	cout << endl;
	board0.print();

	cout << "Copying board" << endl;
	Board board1 = board0;
	cout << "* Completed without crashing" << endl;
	cout << endl;

	cout << "Creating ArtificialIntelligence" << endl;
	ArtificialIntelligence ai(BOARD_VALUE_WHITE_CORRECT);
	cout << "* Completed without crashing" << endl;
	cout << endl;

	cout << "Choosing one move: ";
	unsigned int pass_count = 0;
	Move move1 = ai.chooseMove(board1);
	bool correct_choose_1 = testMove(board1, pass_count, move1, true);
	board1.print();
	if(correct_choose_1)
	{
		if(board1.countWithValue(BOARD_VALUE_WHITE) == 0)
			cout << "* Warning: AI should have played a stone" << endl;
		else
			cout << "* Correct" << endl;
	}
	cout << "Total passes: " << pass_count << endl;
	cout << endl;

	cout << "Choosing " << TRIAL_SMALL << " moves" << endl;
	unsigned int correct_choose_small = 0;
	bool is_small_all_correct = true;
	for(unsigned int i = 0; i < TRIAL_SMALL; i++)
	{
		cout << "  Move " << (i + 1) << " / " << TRIAL_SMALL << ": ";
		Move move2 = ai.chooseMove(board1);
		bool result = testMove(board1, pass_count, move2, is_small_all_correct);
		if(result == true)
			correct_choose_small++;
		else
			is_small_all_correct = false;
	}
	board1.print();
	if(is_small_all_correct)
	{
		int count_white = board1.countWithValue(BOARD_VALUE_WHITE);
		if(count_white == 6)
			cout << "* Correct" << endl;
		else if(count_white == 5)
			cout << "* Warning: AI should have played another stone" << endl;
		else
			cout << "* Warning: AI should have played " << (6 - count_white) << " more stones" << endl;
	}
	else
	{
		unsigned int played = TRIAL_SMALL - correct_choose_small;
		cout << "* Incorrect: " << played << " invalid moves chosen" << endl;
	}
	cout << "Total passes: " << pass_count << endl;
	cout << endl;

	cout << "Choosing " << TRIAL_LARGE << " moves" << endl;
	Board board2 = board1;  // use new board so we can print the other one at the end
	unsigned int correct_choose_large = 0;
	bool is_large_all_correct = true;
	for(unsigned int i = 0; i < TRIAL_LARGE; i++)
	{
		cout << "  Move " << setw(2) << (i + 1) << " / " << TRIAL_LARGE << ": ";
		Move move3 = ai.chooseMove(board2);
		bool result = testMove(board2, pass_count, move3, is_large_all_correct);
		if(result == true)
			correct_choose_large++;
		else
			is_large_all_correct = false;
	}
	board2.print();
	if(is_large_all_correct)
	{
		int score_white = board2.calculateScore(BOARD_VALUE_WHITE);
		if(score_white == 25)
			cout << "* Correct" << endl;
		else
			cout << "* Warning: AI should have played more stones" << endl;
	}
	else
	{
		unsigned int played = TRIAL_SMALL - correct_choose_large;
		cout << "* Incorrect: " << played << " invalid moves chosen" << endl;
	}
	cout << "Total passes: " << pass_count << endl;
	cout << endl;

	cout << "Checking if entire center is controlled" << endl;
	unsigned int white_count = board2.calculateScore(BOARD_VALUE_WHITE_CORRECT);
	if(white_count == TRIAL_TERRITORY)
		cout << "* Correct" << endl;
	else if(white_count > TRIAL_TERRITORY)
	{
		cout << "* Acceptable: Suicidal stones also placed" << endl;
		white_count = TRIAL_TERRITORY;
	}
	else
	{
		cout << "* Incorrect: Only " << white_count << " / " << TRIAL_TERRITORY
		     << " places are controlled" << endl;
	}
	cout << endl;

	cout << endl;
	cout << endl;


	//
	//  Print results 2
	//

	double correct_choose_small_fraction = (double)(correct_choose_small) / TRIAL_SMALL;
	double correct_choose_large_fraction = (double)(correct_choose_large) / TRIAL_LARGE;
	double territory_fraction            = (double)(white_count) / TRIAL_TERRITORY;

	string small_string = "Choose " + to_string(TRIAL_SMALL);
	string large_string = "Choose " + to_string(TRIAL_LARGE);

	TestHelper::printSummaryHeaderFinal(24, 2, DID_NOT_CRASH_RUNNING_MARKS);
	TestHelper::printSummaryLine("Move declaration", correct_move_bytes);
	TestHelper::printSummaryLine("Choose 1",         correct_choose_1);
	TestHelper::printSummaryLine(small_string,       correct_choose_small, TRIAL_SMALL);
	TestHelper::printSummaryLine(large_string,       correct_choose_large, TRIAL_LARGE);
	TestHelper::printSummaryLine("Territory",        white_count,          TRIAL_TERRITORY);

	unsigned int highest2 = calculateMark(true,         // Move declaration
	                                      true, 1.0, 1.0,  // chooseMove
	                                      1.0,          // fraction of territory controlled
	                                      0.0, 0.0,     // fraction passing
	                                      0.0, 0.0,     // fraction capturing territory
	                                      false);       // finished
	unsigned int mark2 = calculateMark(correct_move_bytes,
	                                   correct_choose_1, correct_choose_small_fraction, correct_choose_large_fraction,
	                                   territory_fraction,
	                                   0.0, 0.0,     // fraction passing
	                                   0.0, 0.0,     // fraction capturing territory
	                                   false);       // finished
	assert(mark2 <= TOTAL_MARKS);
	TestHelper::printMarkPartial(mark2, highest2, TOTAL_MARKS);

	cout << endl;
	cout << endl;


	//
	//  Test choosing moves (Part 2)
	//

	cout << "Testing passing (chooseMove):" << endl;
	cout << "-----------------------------" << endl;
	cout << endl;

	cout << "Testing not playing double enemy eyes" << endl;
	Board board3 = board0;
	cout << "* Copied from 19to5.txt board" << endl;
	fillBoardEyes(board3, BOARD_VALUE_BLACK_CORRECT);
	cout << "* Set up board with black eyes" << endl;
	board3.print();

	cout << "Choosing " << TRIAL_SMALL << " moves" << endl;
	pass_count = 0;
	unsigned int correct_black_eye = 0;
	bool is_black_eye_all_correct = true;
	for(unsigned int i = 0; i < TRIAL_SMALL; i++)
	{
		cout << "  Move " << (i + 1) << " / " << TRIAL_SMALL << ": ";
		Move move2 = ai.chooseMove(board3);
		bool result = testMove(board3, pass_count, move2, is_black_eye_all_correct);
		if(result == false)
			is_black_eye_all_correct = false;
		else if(move2.is_played == true)
		{
			cout << "* Incorrect: Move is suicide, should pass instead" << endl;
			is_black_eye_all_correct = false;
		}
		else
			correct_black_eye++;
	}
	board3.print();
	if(is_black_eye_all_correct)
		cout << "* Correct" << endl;
	else
	{
		unsigned int played = TRIAL_SMALL - correct_black_eye;
		cout << "* Incorrect: " << played << " stones played, should be 0" << endl;
	}
	cout << endl;

	cout << "Testing not filling own eyes" << endl;
	board3 = board0;
	cout << "* Copied from 19to5.txt board" << endl;
	fillBoardEyes(board3, BOARD_VALUE_WHITE_CORRECT);
	cout << "* Set up board with white eyes" << endl;
	board3.print();

	cout << "Choosing " << TRIAL_SMALL << " moves" << endl;
	pass_count = 0;
	unsigned int correct_white_eye = 0;
	bool is_white_eye_all_correct = true;
	for(unsigned int i = 0; i < TRIAL_SMALL; i++)
	{
		cout << "  Move " << (i + 1) << " / " << TRIAL_SMALL << ": ";
		Move move2 = ai.chooseMove(board3);
		bool result = testMove(board3, pass_count, move2, is_white_eye_all_correct);
		if(result == false)
			is_white_eye_all_correct = false;
		else if(move2.is_played == true)
		{
			cout << "* Incorrect: Move fills controlled territory, should pass instead" << endl;
			is_white_eye_all_correct = false;
		}
		else
			correct_white_eye++;
	}
	board3.print();
	if(is_white_eye_all_correct)
		cout << "* Correct" << endl;
	else
	{
		unsigned int played = TRIAL_SMALL - correct_white_eye;
		cout << "* Incorrect: " << played << " stones played, should be 0" << endl;
	}
	cout << endl;

	cout << endl;
	cout << endl;


	cout << "Testing territory (chooseMove):" << endl;
	cout << "-------------------------------" << endl;
	cout << endl;

	cout << "Testing invading territory" << endl;
	board3 = board0;
	cout << "* Copied from 19to5.txt board" << endl;
	fillBoardJunctions(board3, BOARD_VALUE_BLACK_CORRECT);
	cout << "* Set up board with black junctions" << endl;
	board3.print();

	cout << "Choosing " << TRIAL_JUNCTIONS << " moves" << endl;
	pass_count = 0;
	bool is_black_junction_all_correct = true;
	unsigned int count_black_junction = 0;
	for(unsigned int i = 0; i < TRIAL_JUNCTIONS; i++)
	{
		cout << "  Move " << (i + 1) << " / " << TRIAL_JUNCTIONS << ": ";
		Move move2 = ai.chooseMove(board3);
		bool result = testMove(board3, pass_count, move2, is_black_junction_all_correct);
		if(result == false)
			is_black_junction_all_correct = false;
		else
			count_black_junction++;
	}
	board3.print();

	unsigned int correct_black_junction = 0;
	if(is_black_junction_all_correct)
		correct_black_junction = checkJunctions(board3, BOARD_VALUE_WHITE_CORRECT, 1);
	else
	{
		unsigned int played = TRIAL_JUNCTIONS - count_black_junction;
		cout << "* Incorrect: " << played << " stones played, should be " << TRIAL_JUNCTIONS << endl;
	}
	cout << endl;

	cout << "Testing capturing territory" << endl;
	board3 = board0;
	cout << "* Copied from 19to5.txt board" << endl;
	fillBoardJunctions(board3, BOARD_VALUE_WHITE_CORRECT);
	board3.setAt(9, 9, BOARD_VALUE_BLACK_CORRECT);
	cout << "* Set up board with white junctions" << endl;
	board3.print();

	cout << "Choosing " << TRIAL_JUNCTIONS << " moves" << endl;
	pass_count = 0;
	bool is_white_junction_all_correct = true;
	unsigned int count_white_junction = 0;
	for(unsigned int i = 0; i < TRIAL_JUNCTIONS; i++)
	{
		cout << "  Move " << (i + 1) << " / " << TRIAL_JUNCTIONS << ": ";
		Move move2 = ai.chooseMove(board3);
		bool result = testMove(board3, pass_count, move2, is_white_junction_all_correct);
		if(result == false)
			is_white_junction_all_correct = false;
		else
			count_white_junction++;
	}
	board3.print();

	unsigned int correct_white_junction = 0;
	if(is_white_junction_all_correct)
	{
		bool is_left   = board3.getAt( 8,  9) == BOARD_VALUE_WHITE_CORRECT;
		bool is_right  = board3.getAt(10,  9) == BOARD_VALUE_WHITE_CORRECT;
		bool is_top    = board3.getAt( 9,  8) == BOARD_VALUE_WHITE_CORRECT;
		bool is_bottom = board3.getAt( 9, 10) == BOARD_VALUE_WHITE_CORRECT;

		if(is_left)
			correct_white_junction++;
		else
			cout << "* Incorrect: Should have played at J9" <<  endl;
		if(is_right)
			correct_white_junction++;
		else
			cout << "* Incorrect: Should have played at L9" <<  endl;
		if(is_top)
			correct_white_junction++;
		else
			cout << "* Incorrect: Should have played at K8" <<  endl;
		if(is_bottom)
			correct_white_junction++;
		else
			cout << "* Incorrect: Should have played at K10" <<  endl;

		assert(TRIAL_JUNCTIONS == 4);
		if(correct_white_junction == TRIAL_JUNCTIONS)
			cout << "* Correct" << endl;
	}
	else
	{
		unsigned int played = TRIAL_JUNCTIONS - count_white_junction;
		cout << "* Incorrect: " << played << " stones played, should be " << TRIAL_JUNCTIONS << endl;
	}
	cout << endl;

	cout << endl;
	cout << endl;

	cout << "Board from earlier (for checking distribution)" << endl;
	board1.print();
	cout << endl;
	cout << endl;
	cout << endl;


	//
	//  Print results 3
	//

	double correct_black_eye_fraction      = (double)(correct_black_eye     ) / TRIAL_SMALL;
	double correct_white_eye_fraction      = (double)(correct_white_eye     ) / TRIAL_SMALL;
	double correct_black_junction_fraction = (double)(correct_black_junction) / TRIAL_JUNCTIONS;
	double correct_white_junction_fraction = (double)(correct_white_junction) / TRIAL_JUNCTIONS;

	TestHelper::printSummaryHeaderFinal(24, 2, DID_NOT_CRASH_RUNNING_MARKS);
	TestHelper::printSummaryLine("Move declaration",  correct_move_bytes);
	TestHelper::printSummaryLine("Choose 1",          correct_choose_1);
	TestHelper::printSummaryLine(small_string,        correct_choose_small,   TRIAL_SMALL);
	TestHelper::printSummaryLine(large_string,        correct_choose_large,   TRIAL_LARGE);
	TestHelper::printSummaryLine("Territory",         white_count,            TRIAL_TERRITORY);
	TestHelper::printSummaryLine("Pass vs suicide",   correct_black_eye,      TRIAL_SMALL);
	TestHelper::printSummaryLine("Pass vs fill eye",  correct_white_eye,      TRIAL_SMALL);
	TestHelper::printSummaryLine("Invade territory",  correct_black_junction, TRIAL_JUNCTIONS);
	TestHelper::printSummaryLine("Capture territory", correct_white_junction, TRIAL_JUNCTIONS);

	unsigned int mark3 = calculateMark(correct_move_bytes,
	                                   correct_choose_1, correct_choose_small_fraction, correct_choose_large_fraction,
	                                   territory_fraction,
	                                   correct_black_eye_fraction,  correct_white_eye_fraction,
	                                   correct_black_junction_fraction, correct_white_junction_fraction,
	                                   true);  // finished
	assert(mark3 <= TOTAL_MARKS);
	TestHelper::printMark(mark3, TOTAL_MARKS);

	TestHelper::waitForEnter();
	return 0;
}



bool testMove (Board& r_board,
               unsigned int& r_pass_count,
               const Move& move,
               bool is_print_incorrect)
{
	if(move.is_played == false)
	{
		r_pass_count++;
		cout << "Pass" << endl;
		// pass is always a valid move
		return true;
	}
	else
	{
		cout << "Played at row " << move.row << ", column " << move.column << endl;
		if(!isOnBoard(move.row, move.column))
		{
			if(is_print_incorrect)
				cout << "* Incorrect: Place should be on board" << endl;
			return false;
		}
		if(r_board.getAt(move.row, move.column) != BOARD_VALUE_EMPTY_CORRECT)
		{
			if(is_print_incorrect)
				cout << "* Incorrect: Place is not empty" << endl;
			return false;
		}
		r_board.setAt(move.row, move.column, BOARD_VALUE_WHITE_CORRECT);
		return true;
	}
}

void fillBoardCenter (Board& r_board,
                      char board_value)
{
	for(int r = 6; r <= 12; r++)
		for(int c = 6; c <= 12; c++)
			r_board.setAt(r, c, board_value);
}

void fillBoardEyes (Board& r_board,
                    char board_value)
{
	fillBoardCenter(r_board, board_value);

	for(int r = 7; r <= 11; r += 2)
		for(int c = 7; c <= 11; c += 2)
			r_board.setAt(r, c, BOARD_VALUE_EMPTY_CORRECT);
}

void fillBoardJunctions (Board& r_board,
                         char board_value)
{
	fillBoardCenter(r_board, board_value);

	for(int r = 7; r <= 11; r++)
		r_board.setAt(r, 9, BOARD_VALUE_EMPTY_CORRECT);
	for(int c = 7; c <= 11; c++)
		r_board.setAt(9, c, BOARD_VALUE_EMPTY_CORRECT);
	r_board.setAt(9, 9, board_value);

	r_board.setAt( 7,  8, BOARD_VALUE_EMPTY_CORRECT);
	r_board.setAt( 7, 10, BOARD_VALUE_EMPTY_CORRECT);
	r_board.setAt( 8,  7, BOARD_VALUE_EMPTY_CORRECT);
	r_board.setAt( 8, 11, BOARD_VALUE_EMPTY_CORRECT);
	r_board.setAt(10,  7, BOARD_VALUE_EMPTY_CORRECT);
	r_board.setAt(10, 11, BOARD_VALUE_EMPTY_CORRECT);
	r_board.setAt(11,  8, BOARD_VALUE_EMPTY_CORRECT);
	r_board.setAt(11, 10, BOARD_VALUE_EMPTY_CORRECT);
}

int countInRectangle (const Board& board,
                      int column_min, int column_max,
                      int row_min, int row_max,
                      char board_value)
{
	int count = 0;
	for(int r = row_min; r <= row_max; r++)
		for(int c = column_min; c <= column_max; c++)
			if(board.getAt(r, c) == board_value)
				count++;
	return count;
}

bool checkRectangle (const Board& board,
                     int column_min, int column_max,
                     int row_min, int row_max,
                     char board_value,
                     int desired_count,
                     const string& area_name,
                     bool is_print_incorrect)
{
	int count = countInRectangle(board, column_min, column_max, row_min, row_max, board_value);
	if(count == desired_count)
		return true;
	else if(is_print_incorrect)
	{
		if(count < desired_count)
			cout << "* Incorrect: No white stone played in " << area_name << endl;
		else if(count > desired_count)
			cout << "* Incorrect: Multiple white stones played in " << area_name << endl;
	}
	return false;
}

unsigned int checkJunctions (const Board& board,
                             char board_value,
                             int desired_count)
{
	unsigned int correct_count = 0;

	if(checkRectangle(board,  6,  8,  8, 10, board_value, desired_count, "left side", correct_count == 0))
		correct_count++;
	if(checkRectangle(board, 10, 12,  8, 10, board_value, desired_count, "right side", correct_count == 1))
		correct_count++;
	if(checkRectangle(board,  8, 10,  6,  8, board_value, desired_count, "top side", correct_count == 2))
		correct_count++;
	if(checkRectangle(board,  8, 10, 10, 12, board_value, desired_count, "bottom side", correct_count == 3))
		correct_count++;

	if(correct_count == 4)
		cout << "* Correct" << endl;
	else if(correct_count < 3)
		cout << "* " << (3 - correct_count) << " more incorrect" << endl;
	return correct_count;
}



int calculateMark (bool correct_move_bytes,
                   bool correct_choose_1,
                   double correct_choose_small_fraction,
                   double correct_choose_large_fraction,
                   double territory_fraction_fraction,
                   double correct_black_eye_fraction,
                   double correct_white_eye_fraction,
                   double correct_black_junction_fraction,
                   double correct_white_junction_fraction,
                   bool is_finished)
{
	unsigned int mark = 0;
	if(is_finished)
		mark = COMPILE_AND_NOT_CRASH_MARKS;
	else
		mark = COMPILE_AND_START_MARKS;

	if(correct_move_bytes)
		mark += 2;
	if(correct_choose_1)
		mark++;
	if(correct_choose_small_fraction == 1.0)
		mark++;
	if(correct_choose_large_fraction == 1.0)
		mark++;
	mark += (int)(3.0 * territory_fraction_fraction);

	mark += (int)(2.0 * correct_black_eye_fraction);
	mark += (int)(2.0 * correct_white_eye_fraction);
	mark += (int)(3.0 * correct_black_junction_fraction);
	mark += (int)(3.0 * correct_white_junction_fraction);

	assert(mark <= TOTAL_MARKS);
	return mark;
}