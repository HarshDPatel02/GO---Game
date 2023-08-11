#pragma once
#include <string>
using namespace std;

const int STAR_POINT_SPACING = 6;
const int BOARD_SIZE_MIN = 1;
const int BOARD_SIZE_MAX = 24;
const int BOARD_SIZE_DEFAULT = 19;

struct StonesRemoved {
  int us;
  int them;
};

class Board {

public:
  Board();
  //
  // Size constructor
  //
  // purpose : to initialize  Board_Size, Board_Total_Places.
  //
  // parameter: size_in = it is value Board_Size
  //
  // Precondition:- (size_in >= BOARD_SIZE_MIN || size_in <= BOARD_SIZE_MAX)
  //
  // return:-N/A
  // Side effect : N/A
  Board(int size_in);
  Board(const Board &board_in);
  ~Board();
  //
  // getSize
  //
  // purpose: To get board Size number which is equal to 1 to 24;
  //
  // parameters:N/A
  //
  // Return: it will return Board_Size
  //
  // Side effect : N/A
  int getSize() const;
  Board &operator=(const Board &board_in);
  bool operator==(const Board &board_in) const;
  //
  // isOnBoard
  // purpose: to find the given place is on board or not
  // Parameter:
  // 1) row_in:- weather row it is on place or not
  // 2) column_in:-weather column it is on place or not
  //
  // Precondition:- N/A
  //
  // Return: if true them place is on the board and visa vera is true
  //
  // Side effect : N/A

  bool isOnBoard(int row_in, int column_in) const;
  char getAt(int row_in, int column_in) const;
  //   getAt

  //   Purpose:- Get the perfect position of row and column on the Board.
  //   Parameter(s):-
  //   1)row_in:- the row of the place to change , expressed as integer.
  //   2)column_in:- the column of the place to change, expressed as integer.
  // Precondition:-
  // 1)isOnBoard(row_in, column_in)
  // Return:- Array of the board.
  //  Side Effects= N/A.

  void print() const;
  int countWithValue(char value_in) const;
  int calculateScore(char us_value_in) const;
  void setAt(int row_in, int column_in, char value_in);
  void load(const std::string &filename_in);
  StonesRemoved playStone(int row_in, int column_in, char us_value_in);
  // playStone

  //   Purpose:- to capture stones.
  //   Parameter(s):-
  //   1)row_in:- the row of the place to change , expressed as integer.
  //   2)column_in:- the column of the place to change, expressed as integer.
  //   3)us_value_in:-  Board Value of current player.
  // Precondition:-
  // 1)isOnBoard(row_in, column_in)
  // 2)empty board value positon.
  // 3)isBoardValuePlayer(us_value_in);
  // Return:- Total number of stone played by the current player.
  //  Side Effects= N/A.

  void replaceAll(char old_value_in, char new_value_in);
  void fillConnected(char old_value_in, char new_value_in,
                     char neighbour_value_in);
  bool isANeighbourWithValue(int row_in, int column_in, char value_in) const;

private:
  int Board_Size;
  int Board_total_places;

  char *p_Board_Data;

  int toIndex(int row_in, int column_in) const;
  int calculatePlaceCount() const;
  bool isAlignedForStarPoint(int index_in) const;
  void printRowNumbers(int row_in) const;
  void printColumnLetters() const;
  int capturePlayer(char player_value_in);

  void copyData(const Board &board_in);
  void destroyData();
  //
  // isInvariantTrue
  // Purpose: it check weather the board size is legal and also check the number
  // of places are correct or not
  //
  // Parameters : 0
  //
  // Return : true if the condition match and visa vera is also true
  // Side effect: N/A

  bool isInvariantTrue() const;
};