#include <cassert>
#include <fstream>
#include <iomanip>
#include <string>

#include "Board.h"
#include "BoardValue.h"

using namespace std;

bool Board ::isOnBoard(int row_in, int column_in) const {
  if (row_in < 0)
    return false;
  if (row_in >= Board_Size)
    return false;
  if (column_in < 0)
    return false;
  if (column_in >= Board_Size)
    return false;
  return true;
}

Board::Board() {
  Board_Size = BOARD_SIZE_DEFAULT;
  Board_total_places = calculatePlaceCount();
  p_Board_Data = new char[Board_total_places];
  for (int a = 0; a < Board_total_places; ++a) {
    p_Board_Data[a] = BOARD_VALUE_EMPTY;
  }
}
int Board::getSize() const { return Board_Size; }

Board::Board(int size_in) {
  assert(size_in >= BOARD_SIZE_MIN || size_in <= BOARD_SIZE_MAX);
  Board_Size = size_in;
  Board_total_places = calculatePlaceCount();
  p_Board_Data = new char[Board_total_places];
  for (int i = 0; i < Board_total_places; i++) {
    p_Board_Data[i] = BOARD_VALUE_EMPTY;
  }
}

Board::~Board() {
  assert(isInvariantTrue());
  destroyData();
}

void Board::destroyData() { delete[] p_Board_Data; }

Board::Board(const Board &board_in) {
  p_Board_Data = nullptr;
  copyData(board_in);
}

Board &Board::operator=(const Board &board_in) {
  if (this != &board_in) {
    destroyData();
    copyData(board_in);
  }
  return *this;
}

bool Board::operator==(const Board &board_in) const {
  // cheacking the class invraiant is valid or not
  assert(isInvariantTrue());
  assert(board_in.isInvariantTrue());

  // checking the board size and count is valid or not :
  if (Board_Size != board_in.Board_Size ||
      Board_total_places != board_in.Board_total_places) {
    return false;
  }

  for (int i = 0; i < Board_Size; i++) {
    for (int j = 0; j < Board_Size; j++) {
      if (p_Board_Data[toIndex(i, j)] != board_in.p_Board_Data[toIndex(i, j)]) {
        return false;
      }
    }
  }
  return true;
}

void Board::copyData(const Board &board_in) {

  Board_Size = board_in.Board_Size;
  Board_total_places = board_in.Board_total_places;

  p_Board_Data = new char[Board_total_places];
  for (int i = 0; i < Board_Size; i++) {
    for (int j = 0; j < Board_Size; j++) {
      p_Board_Data[toIndex(i, j)] = board_in.p_Board_Data[toIndex(i, j)];
    }
  }
}

char Board::getAt(int row_in, int column_in) const {
  assert(isOnBoard(row_in, column_in));
  return p_Board_Data[toIndex(row_in, column_in)];
  assert(isInvariantTrue());
}

int Board ::toIndex(int row_in, int column_in) const {
  assert(isOnBoard(row_in, column_in));
  return row_in * Board_Size + column_in;
}

void Board::print() const {
  cout << "   ";
  printColumnLetters();
  for (int i = 0; i < Board_Size; i++) {
    printRowNumbers(i);
    cout << " ";
    for (int j = 0; j < Board_Size; j++) {
      if (getAt(i, j) == BOARD_VALUE_EMPTY && isAlignedForStarPoint(i) &&
          isAlignedForStarPoint(j)) {
        cout << "* ";
      } else {
        cout << getAt(i, j) << " ";
      }
    }

    printRowNumbers(i);
    cout << endl;
  }
  cout << "   ";
  printColumnLetters();
  assert(isInvariantTrue());
}

int Board::countWithValue(char value_in) const {
  assert(isBoardValueValid(value_in));

  int total = 0;
  for (int i = 0; i < Board_Size; i++) {
    for (int j = 0; j < Board_Size; j++) {
      if (p_Board_Data[toIndex(i, j)] == value_in) {
        total++;
      }
    }
  }
  return total;
  assert(isInvariantTrue());
}

void Board::setAt(int row_in, int column_in, char value_in) {
  assert(isInvariantTrue());
  assert(isOnBoard(row_in, column_in));
  assert(isBoardValueValid(value_in));
  p_Board_Data[toIndex(row_in, column_in)] = value_in;
}

void Board ::load(const string &filename_in) {
  assert(isInvariantTrue());
  assert(filename_in != "");

  // open file with name filename_in
  // fin will read from that file
  // use fin like you use cin
  ifstream fin(filename_in);

  if (!fin) {
    cout << "Error: Could not open file \"" << filename_in << "\"" << endl;
    return; // end function immediately
  }

  // read in the board size
  string line;
  int board_size_read;
  fin >> board_size_read;
  if (!fin) {
    cout << "Error: File \"" << filename_in
         << "\" does not start with board size" << endl;
    return; // end function immediately
  }
  if (board_size_read > BOARD_SIZE_MAX) {
    cout << "Error: File \"" << filename_in << "\" has board size "
         << board_size_read << ", but maximum is " << BOARD_SIZE_MAX << endl;
    return; // end function immediately
  }
  if (board_size_read < BOARD_SIZE_MIN) {
    cout << "Error: File \"" << filename_in << "\" has board size "
         << board_size_read << ", but minimum is " << BOARD_SIZE_MIN << endl;
    return; // end function immediately
  }
  assert(fin);
  string dummy;
  getline(fin, dummy); // throw away whatever else is on the line

  // replace this board with a new one of the correct size
  *this = Board(board_size_read);

  // read in board state
  bool is_print_error = true;
  for (int r = 0; r < getSize(); r++) {
    string line;
    getline(fin, line);
    if (!fin) {
      if (is_print_error) {
        cout << "Error: Could not read line " << r << " of file \""
             << filename_in << "\"" << endl;
        cout << "       Replacing with '" << BOARD_VALUE_EMPTY << "'s" << endl;
        is_print_error = false;
      }
      line = string(BOARD_VALUE_EMPTY, getSize()); // set to '.'s
    } else if ((int)(line.length()) < getSize()) {
      if (is_print_error) {
        cout << "Error: Line " << r << " of file \"" << filename_in
             << "\" only contains " << line.length() << " / " << getSize()
             << " characters" << endl;
        cout << "       Adding '" << BOARD_VALUE_EMPTY << "'s to end" << endl;
        is_print_error = false;
      }
      line += string(BOARD_VALUE_EMPTY, getSize()); // add '.'s
    }

    for (int c = 0; c < getSize(); c++) {
      if (isBoardValueValid(line[c]))
        setAt(r, c, line[c]);
      else {
        setAt(r, c, BOARD_VALUE_EMPTY);
        if (is_print_error) {
          cout << "Error: Line " << r << ", character " << c << " of file \""
               << filename_in << "\" is an invalid '" << line[c]
               << "' character" << endl;
          cout << "       Substituting '" << BOARD_VALUE_EMPTY << "'" << endl;
          is_print_error = false;
        }
      }
    }
  }

  assert(isInvariantTrue());
  // file will be closed automatically when fin goes out of scope
}

bool Board::isAlignedForStarPoint(int index_in) const {
  assert(index_in >= 0);
  if (index_in % STAR_POINT_SPACING ==
      (Board_Size / 2) % (STAR_POINT_SPACING)) {
    return true;
  }
  return false;
}

void Board::printRowNumbers(int row_in) const {

  if (row_in < 10) {
    cout << " " << row_in;
  } else {
    cout << row_in;
  }
}

void Board ::printColumnLetters() const {

  // column headings
  for (int c = 0; c < Board_Size; c++) {
    char letter = 'A' + c;
    if (letter >= 'I')
      letter++;
    if (letter >= 'O')
      letter++;
    cout << letter << " ";
  }

  // don't need to leave space at end of line
  cout << endl;
}

bool Board::isInvariantTrue() const {

  // checking size is valid or not
  if (Board_Size < BOARD_SIZE_MIN || Board_Size > BOARD_SIZE_MAX) {
    return false;
  }
  // checking total
  if (calculatePlaceCount() != Board_total_places) {
    return false;
  }
  if (p_Board_Data == nullptr)
    return false;

  for (int i = 0; i < Board_Size; i++) {
    for (int j = 0; j < Board_Size; j++) {
      if (!isBoardValueValid(p_Board_Data[toIndex(i, j)])) {
        return false;
      }
    }
  }
  return true;
}

void Board::replaceAll(char old_value_in, char new_value_in) {
  assert(isInvariantTrue());
  assert(old_value_in == BOARD_VALUE_BLACK ||
         old_value_in == BOARD_VALUE_WHITE ||
         old_value_in == BOARD_VALUE_EMPTY);
  assert(isInvariantTrue());

  for (int i = 0; i < Board_Size; i++) {
    for (int j = 0; j < Board_Size; j++) {
      if (p_Board_Data[toIndex(i, j)] == old_value_in) {
        p_Board_Data[toIndex(i, j)] = new_value_in;
      }
    }
  }
  assert(isInvariantTrue());
}

bool Board::isANeighbourWithValue(int row_in, int column_in,
                                  char value_in) const {
  assert(isInvariantTrue());
  assert(isOnBoard(row_in, column_in));
  assert(isBoardValueValid(value_in));

  if (isOnBoard(row_in - 1, column_in)) {
    // return true;
    if (p_Board_Data[toIndex(row_in - 1, column_in)] == value_in) {
      return true;
    }
  }
  if (isOnBoard(row_in + 1, column_in)) {
    // return true;
    if (p_Board_Data[toIndex(row_in + 1, column_in)] == value_in) {
      return true;
    }
  }

  if (isOnBoard(row_in, column_in - 1)) {
    // return true;
    if (p_Board_Data[toIndex(row_in, column_in - 1)] == value_in) {
      return true;
    }
  }

  if (isOnBoard(row_in, column_in + 1)) {
    // return true;
    if (p_Board_Data[toIndex(row_in, column_in + 1)] == value_in) {
      return true;
    }
  }
  return false;
}

void Board::fillConnected(char old_value_in, char new_value_in,
                          char neighbour_value_in) {
  assert(isInvariantTrue());
  assert(isBoardValueValid(old_value_in) &&
         isBoardValueValid(neighbour_value_in) &&
         isBoardValueValid(new_value_in));

  bool board_changed = true;
  while (board_changed) {
    board_changed = false;
    for (int i = 0; i < Board_Size; i++) {
      for (int j = 0; j < Board_Size; j++) {
        if (p_Board_Data[toIndex(i, j)] == old_value_in) {
          if (isANeighbourWithValue(i, j, new_value_in) ||
              isANeighbourWithValue(i, j, neighbour_value_in)) {
            setAt(i, j, new_value_in);
            board_changed = true;
          }
        }
      }
    }
    assert(isInvariantTrue());
  }
}

int Board::calculateScore(char us_value_in) const {
  assert(isInvariantTrue());
  assert(isBoardValuePlayer(us_value_in));

  int total = 0;
  char firstPlayer = getOtherPlayer(us_value_in);
  Board copy = *this;

  copy.fillConnected(BOARD_VALUE_EMPTY, firstPlayer, firstPlayer);
  copy.fillConnected(BOARD_VALUE_EMPTY, us_value_in, us_value_in);
  for (int i = 0; i < Board_Size; i++) {
    for (int j = 0; j < Board_Size; j++) {
      if (copy.p_Board_Data[toIndex(i, j)] == us_value_in) {
        total++;
      }
    }
  }
  return total;
}

int Board::calculatePlaceCount() const { return Board_Size * Board_Size; }

int Board::capturePlayer(char player_value_in) {
  assert(isBoardValuePlayer(player_value_in));

  int number = 0;

  for (int i = 0; i < Board_Size; i++) {
    for (int j = 0; j < Board_Size; j++) {
      if (p_Board_Data[toIndex(i, j)] == player_value_in) {
        p_Board_Data[toIndex(i, j)] = BOARD_VALUE_MARKED;
      }
    }
  }

  fillConnected(BOARD_VALUE_MARKED, player_value_in, BOARD_VALUE_EMPTY);

  for (int i = 0; i < Board_Size; i++) {
    for (int j = 0; j < Board_Size; j++) {
      if (p_Board_Data[toIndex(i, j)] == BOARD_VALUE_MARKED) {
        number++;
      }
    }
  }

  for (int i = 0; i < Board_Size; i++) {
    for (int j = 0; j < Board_Size; j++) {
      if (p_Board_Data[toIndex(i, j)] == BOARD_VALUE_MARKED) {
        p_Board_Data[toIndex(i, j)] = BOARD_VALUE_EMPTY;
      }
    }
  }

  return number;
}

StonesRemoved Board::playStone(int row_in, int column_in, char us_value_in) {
  assert(isInvariantTrue());
  assert(isOnBoard(row_in, column_in));
  char position = p_Board_Data[toIndex(row_in, column_in)];
  assert(position = BOARD_VALUE_EMPTY);
  assert(isBoardValuePlayer(us_value_in));

  setAt(row_in, column_in, us_value_in);
  char a = getOtherPlayer(us_value_in);
  char b = capturePlayer(a);

  int c = capturePlayer(us_value_in);

  StonesRemoved d;
  d.them = b;
  d.us = c;

  return d;
}
