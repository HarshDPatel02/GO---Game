#pragma once
#include <string>

using namespace std;

struct Move {
  bool is_played;
  int row, column;
};

struct SortableMove : public Move {
  float net_score;
};

bool operator== (const SortableMove& lhs_in, const SortableMove& rhs_in);
bool operator< (const SortableMove& lhs_in,const SortableMove& rhs_in);