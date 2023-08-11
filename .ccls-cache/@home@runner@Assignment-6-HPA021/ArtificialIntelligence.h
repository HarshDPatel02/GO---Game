#pragma once

#include "Board.h"
#include "BoardValue.h"
#include "Move.h"
#include "PlaceString.h"
#include "Search.h"
class Game;

struct MoveResult {
  bool legal;
  float netScore;
};

// ArtificialIntelligence
// it will hepl player to get a move in a random manner .
//
// class Invariant
// isBoardValuePlayer(board_value);
class ArtificialIntelligence {

public:
  //   Purpose:- getting value for the constructor and getting instance object
  //   of the class Parameter(s):- 1) us_value_in :- value of the current player
  //   Percondition:-
  //   1) usBoardValuePlayer(us_value_in)
  // Return:- N/A
  //  Side Effects= changing value as per current player value.

  ArtificialIntelligence(char us_value_in);

  // chooseMove

  //   Purpose:- take a move to get player a highest value.
  //   Parameter(s):-
  //   1)board_in :- Board object.
  // Precondition:-
  // N/A
  // Return:- specified place of move
  //  Side Effects= N/A.
  //  Move chooseMove(const Board &board_in) const;
  Move chooseMove(const Game &game_in) const;

private:
  char boardValue;
  float calculateNetScore(const Board &board_in) const;
  MoveResult getPlayResult(const Game& game, int row_in,
                           int column_in) const;
  bool isInvariantTrue() const;
};