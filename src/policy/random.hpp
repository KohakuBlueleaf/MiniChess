#pragma once
#include "../state/state.hpp"

class Random{
public:
  static Move get_move(State *state, int depth);
};