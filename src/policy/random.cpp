#include <cstdlib>

#include "../state/state.hpp"
#include "./random.hpp"


Move Random::get_move(State *state, int depth){
  if(!state->legal_actions.size())
    state->get_legal_actions();
  
  auto actions = state->legal_actions;
  return actions[(rand()+depth)%actions.size()];
}