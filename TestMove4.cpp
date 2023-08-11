//
//  TestMove4.cpp
//
//  A test program for the Move module.
//
//  This program is to be used with Assignment 4: Part C for
//    CS115, 202310.
//
//  Do not modify this file.
//

#include "Move.h"
#include "Move.h"  // repeated to test for #include errors

#include <cassert>
#include <string>
#include <iostream>

#include "TestHelper.h"

using namespace std;



int calculateMark (bool correct_move_bytes,
                   bool correct_sortable_bytes,
                   bool correct_inheritance,
                   double correct_equality_fraction,
                   double correct_less_than_fraction,
                   bool is_finished);

const int COMPILE_AND_START_MARKS     = 2;
const int DID_NOT_CRASH_RUNNING_MARKS = 1;
const int COMPILE_AND_NOT_CRASH_MARKS = COMPILE_AND_START_MARKS + DID_NOT_CRASH_RUNNING_MARKS;
const int COMBINED_TEST_MARKS         = 7;
const int TOTAL_MARKS                 = COMPILE_AND_NOT_CRASH_MARKS + COMBINED_TEST_MARKS;

const int CHECK_ROW_SIZE = 3;
const int CHECK_COUNT    = CHECK_ROW_SIZE * CHECK_ROW_SIZE;



int main ()
{
	TestHelper::startup("TestMove4", COMPILE_AND_START_MARKS, TOTAL_MARKS);


	//
	//  Test Move and SortableMove structs
	//

	cout << "Testing definitions:" << endl;
	cout << "--------------------" << endl;
	cout << endl;

	cout << "Checking Move size in bytes" << endl;
	size_t move_bytes           = sizeof(Move);
	size_t move_bytes_is_played = sizeof(bool);
	size_t move_bytes_place     = sizeof(int) * 2;
	size_t move_bytes_correct   = sizeof(int) * 2 + sizeof(bool);
	bool correct_move_bytes = false;
	if(move_bytes == move_bytes_is_played)
		cout << "* Incorrect: Move should also contain place" << endl;
	else if(move_bytes == move_bytes_place)
		cout << "* Incorrect: Move should also contain if played" << endl;
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

	cout << "Checking SortableMove size in bytes" << endl;
	size_t sortable_bytes           = sizeof(SortableMove);
	size_t sortable_bytes_as_move   = sizeof(Move);
	size_t sortable_bytes_correct   = sizeof(int) * 2 + sizeof(bool) + sizeof(float);
	bool correct_sortable_bytes = false;
	if(sortable_bytes == sortable_bytes_as_move)
		cout << "* Incorrect: SortableMove should also contain net score" << endl;
	else if(sortable_bytes < sortable_bytes_correct)
		cout << "* Incorrect: SortableMove should contain more data" << endl;
	else if(sortable_bytes > sortable_bytes_correct * 2)  // extra space because padding happens
		cout << "* Incorrect: SortableMove should contain less data" << endl;
	else
	{
		cout << "* Correct" << endl;
		correct_sortable_bytes = true;
	}
	cout << endl;

	bool correct_inheritance = false;
	cout << "Creating SortableMove" << endl;
	{
		SortableMove sortable0;  // will be destroyed at end of scope
		cout << "* Completed without crashing" << endl;
		cout << endl;

		cout << "Testing inheritance" << endl;
		if(dynamic_cast<SortableMove*>(&sortable0) == nullptr)
			cout << "* Incorrect: SortableMove should inherit from Move" << endl;
		else
		{
			cout << "* Correct" << endl;
			correct_inheritance = true;
		}
		cout << endl;

		cout << "Destroying SortableMove" << endl;
	}  // sortable0 is destroyed here
	cout << "* Completed without crashing" << endl;
	cout << endl;

	cout << endl;
	cout << endl;


	//
	//  Print results 1
	//

	TestHelper::printSummaryHeaderPartial(24, 1, COMPILE_AND_START_MARKS);
	TestHelper::printSummaryLine("Move struct",         correct_move_bytes);
	TestHelper::printSummaryLine("SortableMove struct", correct_sortable_bytes);
	TestHelper::printSummaryLine("Inheritance",         correct_inheritance);

	unsigned int highest1 = calculateMark(true, true, true,  // constants
	                                      0.0, 0.0,  // operators
	                                      false);    // finished
	unsigned int mark1 = calculateMark(correct_move_bytes, correct_sortable_bytes, correct_inheritance,
	                                   0.0, 0.0,  // operators
	                                   false);    // finished
	assert(mark1 <= TOTAL_MARKS);
	TestHelper::printMarkPartial(mark1, highest1, TOTAL_MARKS);

	cout << endl;
	cout << endl;


	//
	//  Test overloaded operators
	//

	cout << "Testing overloaded operators:" << endl;
	cout << "-----------------------------" << endl;
	cout << endl;

	cout << "Setting up SortableMoves" << endl;
	SortableMove sortable[3];
	cout << "* Created without crashing" << endl;
	sortable[0].net_score = -1.0f;
	sortable[1].net_score =  0.0f;
	sortable[2].net_score = 99.99f;
	cout << "* Initialized without crashing" << endl;
	cout << endl;

	unsigned int correct_equality = 0;
	bool is_print_wrong_equality = true;
	cout << "Testing equality test (operator==)" << endl;
	for(int i = 0; i < CHECK_ROW_SIZE; i++)
	{
		for(int j = 0; j < CHECK_ROW_SIZE; j++)
		{
			bool correct = i == j;
			bool result  = sortable[i] == sortable[j];

			if(result == correct)
				correct_equality++;
			else
			{
				if(is_print_wrong_equality)
				{
					if(result == true)
					{
						cout << "* Incorrect: " << sortable[i].net_score
						     << " should not equal " << sortable[j].net_score << endl;
					}
					else
					{
						cout << "* Incorrect: " << sortable[i].net_score
						     << " should equal " << sortable[j].net_score << endl;
					}
				}
				is_print_wrong_equality = false;
			}
		}
	}
	if(correct_equality == CHECK_COUNT)
		cout << "* Correct" << endl;
	else
	{
		int wrong_count = CHECK_COUNT - correct_equality;
		if(wrong_count > 1)
			cout << "* " << (wrong_count - 1) << " more incorrect" << endl;
	}
	cout << endl;

	unsigned int correct_less_than = 0;
	bool is_print_wrong_less_than = true;
	cout << "Testing less_than test (operator<)" << endl;
	for(int i = 0; i < CHECK_ROW_SIZE; i++)
	{
		for(int j = 0; j < CHECK_ROW_SIZE; j++)
		{
			bool correct = i < j;
			bool result  = sortable[i] < sortable[j];

			if(result == correct)
				correct_less_than++;
			else
			{
				if(is_print_wrong_less_than)
				{
					if(result == true)
					{
						cout << "* Incorrect: " << sortable[i].net_score
						     << " should not be less than " << sortable[j].net_score << endl;
					}
					else
					{
						cout << "* Incorrect: " << sortable[i].net_score
						     << " should be less than " << sortable[j].net_score << endl;
					}
				}
				is_print_wrong_less_than = false;
			}
		}
	}
	if(correct_less_than == CHECK_COUNT)
		cout << "* Correct" << endl;
	else
	{
		int wrong_count = CHECK_COUNT - correct_less_than;
		if(wrong_count > 1)
			cout << "* " << (wrong_count - 1) << " more incorrect" << endl;
	}
	cout << endl;

	cout << endl;
	cout << endl;


	//
	//  Print results 2
	//

	double correct_equality_fraction  = (double)(correct_equality ) / CHECK_COUNT;
	double correct_less_than_fraction = (double)(correct_less_than) / CHECK_COUNT;

	TestHelper::printSummaryHeaderFinal(24, 1, DID_NOT_CRASH_RUNNING_MARKS);
	TestHelper::printSummaryLine("Move struct",         correct_move_bytes);
	TestHelper::printSummaryLine("SortableMove struct", correct_sortable_bytes);
	TestHelper::printSummaryLine("Inheritance",         correct_inheritance);
	TestHelper::printSummaryLine("operator==",          correct_equality,  CHECK_COUNT);
	TestHelper::printSummaryLine("operator<",           correct_less_than, CHECK_COUNT);

	unsigned int mark2 = calculateMark(correct_move_bytes, correct_sortable_bytes, correct_inheritance,
	                                   correct_equality_fraction, correct_less_than_fraction,
	                                   true);  // finished
	assert(mark2 <= TOTAL_MARKS);
	TestHelper::printMark(mark2, TOTAL_MARKS);


	TestHelper::waitForEnter();
	return 0;
}



int calculateMark (bool correct_move_bytes,
                   bool correct_sortable_bytes,
                   bool correct_inheritance,
                   double correct_equality_fraction,
                   double correct_less_than_fraction,
                   bool is_finished)
{
	unsigned int mark = 0;
	if(is_finished)
		mark = COMPILE_AND_NOT_CRASH_MARKS;
	else
		mark = COMPILE_AND_START_MARKS;

	if(correct_move_bytes)
		mark += 1;
	if(correct_sortable_bytes)
		mark += 1;
	if(correct_inheritance)
		mark += 1;

	mark += (int)(2.0 * correct_equality_fraction);
	mark += (int)(2.0 * correct_less_than_fraction);

	assert(mark <= TOTAL_MARKS);
	return mark;
}