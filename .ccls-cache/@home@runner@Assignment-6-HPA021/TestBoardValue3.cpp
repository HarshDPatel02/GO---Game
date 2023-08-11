//
//  TestBoardValue3.cpp
//
//  A test program for the BoardValue module.  It is the
//    TestBoardValue2 program expanded for Assignment 3.
//
//  This program is to be used with Assignment 3: Part C for
//    CS115, 202310.
//
//  Do not modify this file.
//

#include "BoardValue.h"
#include "BoardValue.h"  // repeated to test for #include errors

#include <cassert>
#include <string>
#include <iostream>

#include "TestHelper.h"

using namespace std;



int calculateMark (bool correct_board_value_empty,
                   bool correct_board_value_black,
                   bool correct_board_value_white,
                   bool correct_board_value_marked,
                   double correct_valid_fraction,
                   double correct_invalid_fraction,
                   double correct_player_fraction,
                   double correct_not_player_fraction,
                   double correct_other_player_fraction,
                   bool is_finished);

const int EXISTING_WORK_MARKS         = 1;
const int COMPILE_AND_START_MARKS     = 2 - EXISTING_WORK_MARKS;
const int DID_NOT_CRASH_RUNNING_MARKS = 2;
const int COMPILE_AND_NOT_CRASH_MARKS = COMPILE_AND_START_MARKS + DID_NOT_CRASH_RUNNING_MARKS;
const int COMBINED_TEST_MARKS         = 6 + EXISTING_WORK_MARKS;
const int TOTAL_MARKS                 = COMPILE_AND_NOT_CRASH_MARKS + COMBINED_TEST_MARKS;

const char BOARD_VALUE_EMPTY_CORRECT  = '.';
const char BOARD_VALUE_BLACK_CORRECT  = 'O';
const char BOARD_VALUE_WHITE_CORRECT  = '@';
const char BOARD_VALUE_MARKED_CORRECT = '#';

const char CHECK_CHAR_FIRST            = (char)(32);
const char CHECK_CHAR_LAST             = (char)(126);
const int  CHECK_CHAR_TOTAL_COUNT      = CHECK_CHAR_LAST - CHECK_CHAR_FIRST;
const int  CHECK_CHAR_VALID_COUNT      = 4;
const int  CHECK_CHAR_PLAYER_COUNT     = 2;
const int  CHECK_CHAR_INVALID_COUNT    = CHECK_CHAR_TOTAL_COUNT - CHECK_CHAR_VALID_COUNT  + 1;
const int  CHECK_CHAR_NOT_PLAYER_COUNT = CHECK_CHAR_TOTAL_COUNT - CHECK_CHAR_PLAYER_COUNT + 1;



int main ()
{
	TestHelper::startup("TestBoardValue3", COMPILE_AND_START_MARKS, TOTAL_MARKS);


	//
	//  Test constants
	//

	cout << "Testing BOARD_VALUE_EMPTY constant (Assignment 2)" << endl;
	bool correct_board_value_empty = TestHelper::testConstant(BOARD_VALUE_EMPTY, BOARD_VALUE_EMPTY_CORRECT, "BOARD_VALUE_EMPTY");
	cout << endl;

	cout << "Testing BOARD_VALUE_BLACK constant (Assignment 2)" << endl;
	bool correct_board_value_black = TestHelper::testConstant(BOARD_VALUE_BLACK, BOARD_VALUE_BLACK_CORRECT, "BOARD_VALUE_BLACK");
	cout << endl;

	cout << "Testing BOARD_VALUE_WHITE constant (Assignment 2)" << endl;
	bool correct_board_value_white = TestHelper::testConstant(BOARD_VALUE_WHITE, BOARD_VALUE_WHITE_CORRECT, "BOARD_VALUE_WHITE");
	cout << endl;

	cout << "Testing BOARD_VALUE_MARKED constant (new)" << endl;
	bool correct_board_value_marked = TestHelper::testConstant(BOARD_VALUE_MARKED, BOARD_VALUE_MARKED_CORRECT, "BOARD_VALUE_MARKED");
	cout << endl;

	cout << endl;
	cout << endl;


	//
	//  Print results 1
	//

	TestHelper::printSummaryHeaderPartial(26, 2, COMPILE_AND_START_MARKS);
	TestHelper::printSummaryLine("BOARD_VALUE_EMPTY",        correct_board_value_empty);
	TestHelper::printSummaryLine("BOARD_VALUE_BLACK",        correct_board_value_black);
	TestHelper::printSummaryLine("BOARD_VALUE_WHITE",        correct_board_value_white);
	TestHelper::printSummaryLine("BOARD_VALUE_MARKED",       correct_board_value_marked);

	unsigned int highest1 = calculateMark(true, true, true, true,  // constants
	                                      0.0, 0.0,  // isBoardValueValid
	                                      0.0, 0.0,  // isPlayerBoardValue
	                                      0.0,       // getOtherPlayer
	                                      false);    // finished
	unsigned int mark1 = calculateMark(correct_board_value_empty, correct_board_value_black,
	                                   correct_board_value_white, correct_board_value_marked,
	                                   0.0, 0.0,  // isBoardValueValid
	                                   0.0, 0.0,  // isPlayerBoardValue
	                                   0.0,       // getOtherPlayer
	                                   false);    // finished
	assert(mark1 <= TOTAL_MARKS);
	TestHelper::printMarkPartial(mark1, highest1, TOTAL_MARKS);

	cout << endl;
	cout << endl;


	//
	//  Test functions
	//

	cout << "Testing isBoardValueValid:" << endl;
	cout << "--------------------------" << endl;
	cout << endl;

	unsigned int correct_valid   = 0;
	unsigned int correct_invalid = 0;
	bool is_print_wrong = true;

	cout << "Testing for all chars from '" << CHECK_CHAR_FIRST
	     << "' (" << (int)(CHECK_CHAR_FIRST) << ") to '" << CHECK_CHAR_LAST
	     << "' (" << (int)(CHECK_CHAR_LAST) << ")" << endl;
	for(int c = CHECK_CHAR_FIRST; c <= CHECK_CHAR_LAST; c++)
	{
		bool correct =   (c == BOARD_VALUE_EMPTY_CORRECT ) ? true :
		               ( (c == BOARD_VALUE_BLACK_CORRECT ) ? true :
		               ( (c == BOARD_VALUE_WHITE_CORRECT ) ? true :
					   ( (c == BOARD_VALUE_MARKED_CORRECT) ? true : false )));
		bool result = isBoardValueValid(c);

		if(result == correct)
		{
			if(result == true)
				correct_valid++;
			else
				correct_invalid++;
		}
		else
		{
			if(result == true)
			{
				if(is_print_wrong)
					cout << "* Incorrect: char '" << (char)(c) << "' should be invalid" << endl;
				is_print_wrong = false;
			}
			else
			{
				// not too many of these, so just print all
				cout << "* Incorrect: char '" << (char)(c) << "' should be valid" << endl;
			}
		}
	}

	if(correct_valid   == CHECK_CHAR_VALID_COUNT &&
	   correct_invalid == CHECK_CHAR_INVALID_COUNT)
	{
		cout << "* Correct" << endl;
	}
	else
	{
		int wrong_count = CHECK_CHAR_INVALID_COUNT - correct_invalid;
		if(wrong_count > 1)
			cout << "* " << (wrong_count - 1) << " more should be invalid" << endl;
	}

	cout << endl;
	cout << endl;


	cout << "Testing isBoardValuePlayer:" << endl;
	cout << "--------------------------" << endl;
	cout << endl;

	unsigned int correct_player     = 0;
	unsigned int correct_not_player = 0;
	is_print_wrong = true;

	cout << "Testing for all chars from '" << CHECK_CHAR_FIRST
	     << "' (" << (int)(CHECK_CHAR_FIRST) << ") to '" << CHECK_CHAR_LAST
	     << "' (" << (int)(CHECK_CHAR_LAST) << ")" << endl;
	for(int c = CHECK_CHAR_FIRST; c <= CHECK_CHAR_LAST; c++)
	{
		bool correct =   (c == BOARD_VALUE_BLACK_CORRECT) ? true :
		               ( (c == BOARD_VALUE_WHITE_CORRECT) ? true : false );
		bool result = isBoardValuePlayer(c);

		if(result == correct)
		{
			if(result == true)
				correct_player++;
			else
				correct_not_player++;
		}
		else
		{
			if(result == true)
			{
				if(is_print_wrong)
					cout << "* Incorrect: char '" << (char)(c) << "' should not be a player value" << endl;
				is_print_wrong = false;
			}
			else
			{
				// not too many of these, so just print all
				cout << "* Incorrect: char '" << (char)(c) << "' should be a player value" << endl;
			}
		}
	}

	if(correct_player     == CHECK_CHAR_PLAYER_COUNT &&
	   correct_not_player == CHECK_CHAR_NOT_PLAYER_COUNT)
	{
		cout << "* Correct" << endl;
	}
	else
	{
		int wrong_count = CHECK_CHAR_NOT_PLAYER_COUNT - correct_not_player;
		if(wrong_count > 1)
			cout << "* " << (wrong_count - 1) << " more should not be player values" << endl;
	}

	cout << endl;
	cout << endl;


	cout << "Testing getOtherPlayer:" << endl;
	cout << "-----------------------" << endl;
	cout << endl;

	unsigned int correct_other_player = 0;

	cout << "Testing with BOARD_VALUE_BLACK ('" << BOARD_VALUE_BLACK_CORRECT << "')";
	char other_player_black = getOtherPlayer(BOARD_VALUE_BLACK_CORRECT);
	cout << ": '" << other_player_black << "'" << endl;
	if(other_player_black == BOARD_VALUE_WHITE_CORRECT)
	{
		cout << "* Correct" << endl;
		correct_other_player++;
	}
	else
		cout << "* Incorrect: Should be '" << BOARD_VALUE_WHITE_CORRECT << "'" << endl;

	cout << "Testing with BOARD_VALUE_WHITE ('" << BOARD_VALUE_WHITE_CORRECT << "')";
	char other_player_white = getOtherPlayer(BOARD_VALUE_WHITE_CORRECT);
	cout << ": '" << other_player_white << "'" << endl;
	if(other_player_white == BOARD_VALUE_BLACK_CORRECT)
	{
		cout << "* Correct" << endl;
		correct_other_player++;
	}
	else
		cout << "* Incorrect: Should be '" << BOARD_VALUE_BLACK_CORRECT << "'" << endl;


	cout << endl;
	cout << endl;

	//
	//  Print results 2
	//

	double correct_valid_fraction   = (double)(correct_valid  ) / CHECK_CHAR_VALID_COUNT;
	double correct_invalid_fraction = (double)(correct_invalid) / CHECK_CHAR_INVALID_COUNT;

	double correct_player_fraction     = (double)(correct_player    ) / CHECK_CHAR_PLAYER_COUNT;
	double correct_not_player_fraction = (double)(correct_not_player) / CHECK_CHAR_NOT_PLAYER_COUNT;

	double correct_other_player_fraction = correct_other_player / 2.0;

	TestHelper::printSummaryHeaderFinal(26, 2, DID_NOT_CRASH_RUNNING_MARKS);
	TestHelper::printSummaryLine("BOARD_VALUE_EMPTY",        correct_board_value_empty);
	TestHelper::printSummaryLine("BOARD_VALUE_BLACK",        correct_board_value_black);
	TestHelper::printSummaryLine("BOARD_VALUE_WHITE",        correct_board_value_white);
	TestHelper::printSummaryLine("BOARD_VALUE_MARKED",       correct_board_value_marked);
	TestHelper::printSummaryLine("isBoardValueValid (yes)",  correct_valid,        CHECK_CHAR_VALID_COUNT);
	TestHelper::printSummaryLine("isBoardValueValid (no)",   correct_invalid,      CHECK_CHAR_INVALID_COUNT);
	TestHelper::printSummaryLine("isBoardValuePlayer (yes)", correct_player,       CHECK_CHAR_PLAYER_COUNT);
	TestHelper::printSummaryLine("isBoardValuePlayer (no)",  correct_not_player,   CHECK_CHAR_NOT_PLAYER_COUNT);
	TestHelper::printSummaryLine("getOtherPlayer",           correct_other_player, 2);

	unsigned int mark2 = calculateMark(correct_board_value_empty, correct_board_value_black,
	                                   correct_board_value_white, correct_board_value_marked,
	                                   correct_valid_fraction, correct_invalid_fraction,
	                                   correct_player_fraction, correct_not_player_fraction,
	                                   correct_other_player_fraction,
	                                   true);  // finished
	assert(mark2 <= TOTAL_MARKS);
	TestHelper::printMark(mark2, TOTAL_MARKS);


	TestHelper::waitForEnter();
	return 0;
}



int calculateMark (bool correct_board_value_empty,
                   bool correct_board_value_black,
                   bool correct_board_value_white,
                   bool correct_board_value_marked,
                   double correct_valid_fraction,
                   double correct_invalid_fraction,
                   double correct_player_fraction,
                   double correct_not_player_fraction,
                   double correct_other_player_fraction,
                   bool is_finished)
{
	unsigned int mark = 0;
	if(is_finished)
		mark = COMPILE_AND_NOT_CRASH_MARKS;
	else
		mark = COMPILE_AND_START_MARKS;

	// from Assignment 2
	assert(EXISTING_WORK_MARKS == 1);
	if(correct_board_value_empty &&
	   correct_board_value_black &&
	   correct_board_value_white)
	{
		mark += 1;
	}

	// new stuff
	if(correct_board_value_marked)
		mark += 1;
	if(correct_valid_fraction   == 1.0 &&
	   correct_invalid_fraction == 1.0)
	{
		mark += 1;
	}
	if(correct_player_fraction == 1.0 && correct_not_player_fraction == 1.0)
		mark += 2;
	if(correct_other_player_fraction == 1.0)
		mark += 2;

	assert(mark <= TOTAL_MARKS);
	return mark;
}