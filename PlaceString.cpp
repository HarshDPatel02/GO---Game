#include <iostream>
#include <string>
using namespace std;

const int INVALID_COORDINATE = -1;

int stringToInteger(const string str);
int placeStringToRow(const string str);
int placeStringToColumn(const string str);
bool isPlaceStringWellFormed(const string str);

int stringToInteger(const string str_in) {
  int num = 0;
  if (str_in.length() == 0) {
    return INVALID_COORDINATE;
  } else {
    for (int i = 0; i < str_in.length(); i++) {
      num = (num * 10) + int(str_in[i] - '0');

      if (str_in.length() == 0 || str_in[i] < '0' || str_in[i] > '9') {
        return INVALID_COORDINATE;
      }
    }
  }
  return num;
}

int placeStringToRow(const string str_in) {
  int num = 0;

  for (int i = 1; i < str_in.length(); i++) {

    num = (num * 10) + int(str_in[i] - '0');

    if (str_in[i] < '0' || str_in[i] > '9') {
      return INVALID_COORDINATE;
    }
  }

  if (str_in.length() < 2 || str_in.length() == 0) {
    return INVALID_COORDINATE;
  }
  return num;
}

int placeStringToColumn(const string str_in) {

  char column = str_in[0];

  if (str_in[0] == 'A') {
    return 0;
  } else if (str_in[0] == 'B') {
    return 1;
  } else if (str_in[0] == 'C') {
    return 2;
  } else if (str_in[0] == 'D') {
    return 3;
  } else if (str_in[0] == 'E') {
    return 4;
  } else if (str_in[0] == 'F') {
    return 5;
  } else if (str_in[0] == 'G') {
    return 6;
  } else if (str_in[0] == 'H') {
    return 7;
  } else if (str_in[0] == 'J') {
    return 8;
  } else if (str_in[0] == 'K') {
    return 9;
  } else if (str_in[0] == 'L') {
    return 10;
  } else if (str_in[0] == 'M') {
    return 11;
  } else if (str_in[0] == 'N') {
    return 12;
  } else if (str_in[0] == 'P') {
    return 13;
  } else if (str_in[0] == 'Q') {
    return 14;
  } else if (str_in[0] == 'R') {
    return 15;
  } else if (str_in[0] == 'S') {
    return 16;
  } else if (str_in[0] == 'T') {
    return 17;
  } else if (str_in[0] == 'U') {
    return 18;
  } else if (str_in[0] == 'V') {
    return 19;
  } else if (str_in[0] == 'W') {
    return 20;
  } else if (str_in[0] == 'X') {
    return 21;
  } else if (str_in[0] == 'Y') {
    return 22;
  } else if (str_in[0] == 'Z') {
    return 23;
  } else {
    return INVALID_COORDINATE;
  }

  if (str_in[0] < 'A' || str_in[0] > 'Z') {
    return INVALID_COORDINATE;
  }

  return column;
}

bool isPlaceStringWellFormed(const string str_in) {
  bool g = true;
  if (placeStringToRow(str_in) == -1 || placeStringToColumn(str_in) == -1) {
    return g = false;
  }
  return g;
}
