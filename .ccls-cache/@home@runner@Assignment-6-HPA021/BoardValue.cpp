#include <iostream>
#include <string>
using namespace std;
#include "BoardValue.h"

bool isBoardValueValid(char value_in) {
  bool a = true;
  if (BOARD_VALUE_EMPTY == -1 || BOARD_VALUE_BLACK == -1 ||
      BOARD_VALUE_WHITE == -1 || BOARD_VALUE_MARKED == -1) {
    return false;
  } else {
    return value_in == BOARD_VALUE_EMPTY || value_in == BOARD_VALUE_BLACK ||
           value_in == BOARD_VALUE_WHITE || value_in == BOARD_VALUE_MARKED;
  }
  return a;
}

bool isBoardValuePlayer(char value_in) {
  if (value_in == BOARD_VALUE_BLACK || value_in == BOARD_VALUE_WHITE) {
    return true;
  }
  return false;
}

char getOtherPlayer(char player_value_in) {
  if (player_value_in == BOARD_VALUE_BLACK) {
    return BOARD_VALUE_WHITE;
  } else {
    return BOARD_VALUE_BLACK;
  }
}