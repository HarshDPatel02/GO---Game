// My name is Harsh DineshKumar Patel
// Student Id number is :200490101

#include <iostream>
#include <string>
using namespace std;

const int INVALID_COORDINATE = -1;

int stringToInteger(const string str);
int placeStringToRow(const string str);
int placeStringToColumn(const string str);
bool isPlaceStringWellFormed(const string str);

int main() {
  cout << "it will test place-string" << endl;

  string str_in;
  cout << "Enter your string : ";
  cin >> str_in;

  cout << stringToInteger(str_in) << endl;
  cout << placeStringToRow(str_in) << endl;
  cout << placeStringToColumn(str_in) << endl;
  cout << isPlaceStringWellFormed(str_in) << endl;

  return 0;
}

int stringToInteger(const string str_in) {
  int num = 0;

  for (int i = 0; i < str_in.length(); i++) {

    num = (num * 10) + int(str_in[i] - '0');

    if (str_in.length() == 0 || str_in[i] < '0' || str_in[i] > '9') {
      return INVALID_COORDINATE;
    }
  }

  return num;
}

int placeStringToRow(const string str_in) {
  int num = 0;

  if (str_in.length() < 2 || str_in.length() == 0 || str_in.at(0) == 1) {
    return INVALID_COORDINATE;
  }
  return num;
}

int placeStringToColumn(const string str_in) {
  char column = str_in[0];
  if (str_in[0] < 'I' || str_in[0] > 'O') {
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
