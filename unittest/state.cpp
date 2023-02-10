#include <iostream>
#include <cstdlib>

#include "../src/state/state.hpp"
#include "../src/config.hpp"


int main(){
  srand(RANDOM_SEED);
  
  State state;
  state.print();
  state.get_legal_actions();
  
  for(int i=0; i<50; i++){
    std::cout << "\n" << i << "step " << (state.player?"black":"white") << "'s turn\n";
    auto all_actions = state.legal_actions;
    state = *state.next_state(all_actions[rand() % all_actions.size()]);
    state.print();
    if(state.game_state == WIN){
      std::cout << "\n" << i+1 << "step " << (state.player?"black":"white") << " WIN!\n";
      auto win_action = state.legal_actions.back();
      std::cout << win_action.first.first << ", " << win_action.first.second << " → "\
                << win_action.second.first << ", " << win_action.second.second << "\n";
      state = *state.next_state(win_action);
      state.print();
      break;
    }
  }
  return 0;
}