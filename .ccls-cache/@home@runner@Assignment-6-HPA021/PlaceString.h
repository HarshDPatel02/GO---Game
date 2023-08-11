#pragma once
#include <string>
using namespace std;

const int INVALID_COORDINATE = -1;

int stringToInteger(const string str);
int placeStringToRow(const string str);
int placeStringToColumn(const string str);
bool isPlaceStringWellFormed(const string str);