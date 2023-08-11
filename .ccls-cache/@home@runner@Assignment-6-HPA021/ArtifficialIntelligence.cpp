#include <cassert>
#include <iostream>
#include <string>
using namespace std;
#include "ArtificialIntelligence.h"
#include "Game.h"

ArtificialIntelligence::ArtificialIntelligence(char us_value_in) {
  if ((!isBoardValueValid(us_value_in)) || (!isBoardValuePlayer(us_value_in))) {
    cout << "invalid value for AI :" << endl;
  }
  boardValue = us_value_in;
  assert(isInvariantTrue());
}

bool ArtificialIntelligence::isInvariantTrue() const {
  return isBoardValueValid(boardValue) && isBoardValuePlayer(boardValue);
}

float ArtificialIntelligence::calculateNetScore(const Board &board_in) const {
  char otherPlayerValue = getOtherPlayer(boardValue);
  float us_score = (float)board_in.calculateScore(boardValue);
  float them_score = (float)board_in.calculateScore(otherPlayerValue);

  float Diff = us_score - them_score;

  return Diff;
}

MoveResult ArtificialIntelligence::getPlayResult(const Game& game,
                                                 int row_in,
                                                 int column_in) const {
Board board = game.getBoard();
  // checking if place is on board and empty
  MoveResult result;

  if (game.getBoard().isOnBoard(row_in, column_in) &&
      game.getBoard().getAt(row_in, column_in) == BOARD_VALUE_EMPTY) {

    Board copy = game.getBoard();

    StonesRemoved remove = copy.playStone(row_in, column_in, boardValue);

    if (remove.us > 0) {
      result.legal = false;
      return result;
    } 
       else {
      result.legal = true;
      result.netScore = calculateNetScore(copy);
    }
  } 
  
    else {
    result.legal = false;
  }

  return result;
}

Move ArtificialIntelligence::chooseMove (const Game& game_in) const{

  int MAX_MOVES = (BOARD_SIZE_MAX * BOARD_SIZE_MAX) + 1;
  SortableMove possibleMoves[MAX_MOVES];
  int newMoves = 0;

  possibleMoves[newMoves].is_played = false;
  possibleMoves[newMoves].net_score = calculateNetScore(game_in.getBoard()) + 0.5;
  newMoves++;
  MoveResult result;
  // adding a legal move
  for (int i = 0; i < game_in.getBoard().getSize(); i++) {
    for (int j = 0; j < game_in.getBoard().getSize(); j++) {
     result = getPlayResult(game_in, i, j);
      if (result.legal) {
        possibleMoves[newMoves].row = i;
        possibleMoves[newMoves].column = j;
        possibleMoves[newMoves].is_played = true;
        possibleMoves[newMoves].net_score = result.netScore;
        newMoves++;
      }
    }
  }

  // sorting the moves in array
  mySort(possibleMoves, newMoves);

  // finding the highest scoring move
  double max_score = possibleMoves[newMoves].net_score;
  int highest_index = unsortedFindLargest(possibleMoves, newMoves);
  int index =
      binarySearchFirst(possibleMoves, newMoves, possibleMoves[highest_index]);

  srand(time(NULL));
  // Calculate how many moves are tied for highest score
  int range = newMoves - highest_index;

  // choosing one of the highest score as a random
  int random_index = rand() % range + highest_index;

  return possibleMoves[random_index];
}