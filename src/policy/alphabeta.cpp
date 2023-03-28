#include <ctime>
#include <cmath>
#include <tuple>
#include <utility>
#include "alphabeta.hpp"
#include "../state/state.hpp"


/*
Declaration of AlphaBeta
*/
//Evaluate state with alphabeta pruning
int AlphaBeta::eval(State *state, int depth, int alpha, int beta){
  GameState now_res = state->game_state;
  //return value of this state if the game is end or meet the depth
  if(now_res == WIN){
    delete state;
    return 100000;
  }
  if(now_res == DRAW){
    delete state;
    return 0;
  }
  if(depth == 0){
    int score = state->evaluate();
    delete state;
    return score;
  }
  
  //AlphaBeta Pruning
  for(auto move: state->legal_actions){
    //negative max
    int score = -eval(state->next_state(move), depth-1, -beta, -alpha);
    alpha = std::max(score, alpha);
    if(alpha >= beta){
      delete state;
      return alpha;
    }
  }
  
  delete state;
  return alpha;
};


//Run AlphaBeta pruning and get best move
Move AlphaBeta::get_move(State *state, int depth){
  Move best_action;
  int alpha = M_MAX-10;

  auto all_moves = state->legal_actions;
  for(Move move: all_moves){
    int score = -eval(state->next_state(move), depth-1, M_MAX, -alpha);
    if(score > alpha){
      best_action = move;
      alpha = score;
    }
  }

  return best_action;
};