

//
//  Game.h
//

#pragma once

#include <string>
#include <vector>

#include "ArtificialIntelligence.h"
#include "Board.h"
#include "BoardValue.h"
#include "History.h"
#include "PlaceString.h"
#include "Search.h"

// uses composition
class Game {
public:
  Game();
  Game(int getSize);
  Game(const std::string &filename_in);

  void printBoard() const;
  void printWinner() const;
  const Board &getBoard() const;
  bool isInHistory(const Board &board_in) const;
  void blackPass();
  bool blackPlay(int row_in, int column_in);
  bool whiteAi();
  void undo2Moves();

private:
  void printRemoved(const StonesRemoved &removed_in) const;

private:
  History gameHistory;
  Board board;
  ArtificialIntelligence white;
};
