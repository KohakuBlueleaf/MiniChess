#include <utility>
#include <cstdlib>

#include "../state/state.hpp"
#include "./minimax.hpp"


int MiniMax::eval(State *state, int depth){
  if(depth <= 0){
    return state->evaluate();
  }else if(state->game_state==WIN){
    return 100000;
  }
  int best_score = -100000;
  
  for(Move action: state->legal_actions){
    State *next = state->next_state(action);
    best_score = std::max(best_score, -eval(next, depth-1));
    delete next;
  }
  return best_score;
}


Move MiniMax::get_move(State *state, int depth){
  if(state->legal_actions.size() == 0)
    state->get_legal_actions();
  
  int best_score = M_MAX-10;
  Move best_action;
  
  for(Move action: state->legal_actions){
    State *next = state->next_state(action);
    int score = -eval(next, depth-1);
    if(score > best_score){
      best_score = score;
      best_action = action;
    }
    delete next;
  }
  return best_action;
}