#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <array>
#include <vector>
#include <cassert>

#include "config.hpp"
#include "state/state.hpp"


const std::string file_log = "gamelog.txt";
const std::string file_state = "state";
const std::string file_action = "action";


void launch_executable(std::string filename) {
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
  //filename may has path in it. 
  //like "./build/player.exe" or ".\player.exe"
  size_t pos;
  std::string command = "start /min " + filename + " " + file_state + " " + file_action;
  if((pos = filename.rfind("/"))!=std::string::npos || (pos = filename.rfind("\\"))!=std::string::npos)
    filename = filename.substr(pos+1, std::string::npos);
  std::string kill = "timeout /t " + std::to_string(timeout) + " > NUL && taskkill /im " + filename + " > NUL 2>&1";
  system(command.c_str());
  system(kill.c_str());
#elif __linux__
  std::string command = "timeout " + std::to_string(timeout) + "s " + filename + " " + file_state + " " + file_action;
  system(command.c_str());
#elif __APPLE__
  // May require installing the command by:
  // brew install coreutils
  std::string command = "gtimeout " + std::to_string(timeout) + "s " + filename + " " + file_state + " " + file_action;
  system(command.c_str());
#endif
}


bool valid_move(Move move, std::vector<Move>& legal_moves){
  if(move.first.first>BOARD_H || move.first.second>BOARD_H){
    return false;
  }
  if(move.second.first>BOARD_W || move.second.second>BOARD_W){
    return false;
  }
  for(Move mv: legal_moves){
    if(mv==move){
      return true;
    }
  }
  return false;
}


int main(int argc, char** argv) {
  assert(argc == 3);
  std::ofstream log("gamelog.txt");
  std::string player_filename[3];
  player_filename[1] = argv[1];
  player_filename[2] = argv[2];
  
  std::cout << "Player White File: " << player_filename[1] << std::endl;
  std::cout << "Player Black File: " << player_filename[2] << std::endl;
  
  State game; game.get_legal_actions();
  State *temp;
  std::string data;
  int step=1;
  while (game.game_state == UNKNOWN || game.game_state == NONE) {
    // std::cout << "test\n";
    // Output current state
    std::cout << step << " step" << std::endl;
    data = game.encode_output();
    std::cout << data << std::endl;
    log << data << std::endl;
    
    data = game.encode_state();
    std::ofstream fout(file_state);
    fout << data;
    fout.close();
    // Run external program
    launch_executable(player_filename[game.player+1]);
    // Read action
    std::ifstream fin(file_action);
    Move action(Point(-1, -1), Point(-1, -1));
    int total = 0;
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    system("cls");
#else
    system("clear");
#endif
    while (true) {
      int x, y, n, m;
      if (!(fin >> x)) break;
      if (!(fin >> y)) break;
      if (!(fin >> n)) break;
      if (!(fin >> m)) break;
      action.first.first = x;
      action.first.second = y;
      action.second.first = n;
      action.second.second = m;
      total ++;
    }
    fin.close();

    if (remove(file_action.c_str()) != 0)
      std::cerr << "Error removing file: " << file_action << "\n";
    // Take action
    if (!valid_move(action, game.legal_actions)){
      // If action is invalid.
      std::cout << "Invalid Action\n";
      std::cout << action.first.first << " " << action.first.second << " " << action.second.first << " " << action.second.second << "\n";
      data = game.encode_output();
      std::cout << data;
      log << data;
      break;
    }else{
      temp = game.next_state(action);
      std::cout << "Depth: " << total << std::endl;
      std::cout << action.first.first << " " << action.first.second << " " << action.second.first << " " << action.second.second << "\n";
    }
    game = *temp;
    
    step += 1;
    if(step>MAX_STEP) break;
  }
  log.close();
  
  data = game.encode_output();
  std::cout << data << std::endl;
  log << data << std::endl;
  if(game.game_state == WIN)
    std::cout << "Player" << game.player+1 << " wins\n";
  else
    std::cout << "Draw\n";
  
  // Reset state file
  if (remove(file_state.c_str()) != 0)
    std::cerr << "Error removing file: " << file_state << "\n";
  return 0;
}
