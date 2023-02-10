#include <iostream>

#include "config.hpp"
#include "./state/state.hpp"
#include "./policy/minimax.hpp"
#include "./policy/random.hpp"


int main(){
  srand(RANDOM_SEED);
  
  State *game = new State(); 
  game->get_legal_actions();
  game->print();
  std::cout << std::endl;
  
  while (1){
    if(game->game_state == WIN) break;
    auto action_white = MiniMax::get_move(game, 6);
    game = game->next_state(action_white);
    game->print();
    std::cout << std::endl;
    
    if(game->game_state == WIN) break;
    auto action_black = MiniMax::get_move(game, 6);
    game = game->next_state(action_black);
    game->print();
    std::cout << std::endl;
  }
  
  game = game->next_state(game->legal_actions.back());
  game->print();
  
  return 0;
}