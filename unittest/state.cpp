#include <iostream>
#include <cstdlib>

#include "../src/state/state.hpp"



int main(){
  srand(1);
  
  State state;
  state.print();
  state.get_legal_actions();
  
  for(int i=0; i<50; i++){
    std::cout << "\n";
    auto all_actions = state.legal_actions;
    state = *state.next_state(all_actions[rand() % all_actions.size()]);
    state.print();
  }
  return 0;
}