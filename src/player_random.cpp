#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <array>

#include "./config.hpp"
#include "state/state.hpp"


State root;
void read_board(std::ifstream& fin) {
  Board board;
  int player;
  fin >> player;

  for (int pl=0; pl<2; pl++) {
    for (int i=0; i<5; i++) {
      for (int j=0; j<5; j++) {
        int c; fin >> c;
        // std::cout << c << " ";
        board.board[pl][i][j] = c;
      }
      // std::cout << std::endl;
    }
  }
  root = State(board, player);
  root.get_legal_actions();
}

void write_valid_spot(std::ofstream& fout) {
  int x;
  // Keep updating the output until getting killed.
  while(true) {
    // Choose a random spot.
    if(root.legal_actions.empty()) {
      std::cout << "empty" << std::endl;
    }
    x = int(int(rand()) % int(root.legal_actions.size()));
    auto move = root.legal_actions[x];
    fout << move.first.first << " " << move.first.second << " "\
         << move.second.first << " " << move.second.second << std::endl;
    
    // Remember to flush the output to ensure the last action is written to file.
    fout.flush();
    break;
  }
}

int main(int, char** argv) {
  srand(RANDOM_SEED);
  std::ifstream fin(argv[1]);
  std::ofstream fout(argv[2]);

  read_board(fin);
  write_valid_spot(fout);

  fin.close();
  fout.close();
  return 0;
}
