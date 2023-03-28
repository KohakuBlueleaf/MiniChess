#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <array>
#include <vector>
#include <cassert>
#include "config.hpp"

#define elif else if

const char al[27] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";


typedef std::pair<size_t, size_t> Point;
typedef std::pair<Point, Point> Move;
class Board{
  public:
    char board[2][5][5] = {{
      //white
      {0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0},
      {1, 1, 1, 1, 1},
      {6, 5, 2, 3, 4},
    }, {
      //black
      {4, 3, 2, 5, 6},
      {1, 1, 1, 1, 1},
      {0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0},
    }};
};

enum GameState {
  UNKNOWN = 0,
  WIN,
  DRAW,
  NONE
};

const int M_MAX = -100000;
const int P_MAX = 100000;

class State{
  public: 
    GameState game_state = UNKNOWN;
    Board board;
    int player = 0;
    int score = -10000000;
    std::vector<Move> legal_actions;
    
    State(){};
    State(int player): player(player){};
    State(Board board): board(board){};
    State(Board board, int player): board(board), player(player){};
    
    State* next_state(Move move);
    void get_legal_actions();
    std::string encode_output();
    std::string encode_state();
};


bool valid_move(Move move, std::vector<Move>& legal_moves){
  if(move.first.first>4 || move.first.second>4){
    return false;
  }
  if(move.second.first>4 || move.second.second>4){
    return false;
  }
  for(Move mv: legal_moves){
    if(mv==move){
      return true;
    }
  }
  return false;
}

State* State::next_state(Move move){
  Board next = this->board;
  Point from = move.first, to = move.second;
  
  if(!valid_move(move, legal_actions)){
    return NULL;
  }
  
  int8_t moved = next.board[this->player][from.first][from.second];
  
  if(moved==3){
    // std::cout << "move knight" << std::endl;
  }
  //promotion for pawn
  if(moved == 1 && (to.first==4 || to.first==0)){
    moved = 5;
  }
  if(next.board[1-this->player][to.first][to.second]){
    next.board[1-this->player][to.first][to.second] = 0;
  }
  
  next.board[this->player][from.first][from.second] = 0;
  next.board[this->player][to.first][to.second] = moved;
  
  State* next_state = new State(next, 1-this->player);
  
  if(this->game_state != WIN)
    next_state->get_legal_actions();
  return next_state;
}


static const int move_table_rook_bishop[8][4][2] = {
  {{0, 1}, {0, 2}, {0, 3}, {0, 4}},
  {{0, -1}, {0, -2}, {0, -3}, {0, -4}},
  {{1, 0}, {2, 0}, {3, 0}, {4, 0}},
  {{-1, 0}, {-2, 0}, {-3, 0}, {-4, 0}},
  {{1, 1}, {2, 2}, {3, 3}, {4, 4}},
  {{1, -1}, {2, -2}, {3, -3}, {4, -4}},
  {{-1, 1}, {-2, 2}, {-3, 3}, {-4, 4}},
  {{-1, -1}, {-2, -2}, {-3, -3}, {-4, -4}},
};
static const int move_table_knight[8][2] = {
  {1, 2}, {1, -2},
  {-1, 2}, {-1, -2},
  {2, 1}, {2, -1},
  {-2, 1}, {-2, -1},
};
static const int move_table_king[8][2] = {
  {1, 0}, {0, 1}, {-1, 0}, {0, -1}, 
  {1, 1}, {1, -1}, {-1, 1}, {-1, -1},
};

void State::get_legal_actions(){
  this->game_state = NONE;
  std::vector<Move> all_actions;
  auto self_board = this->board.board[this->player];
  auto oppn_board = this->board.board[1 - this->player];
  
  int now_piece, oppn_piece;
  for(int i=0; i<5; i+=1){
    for(int j=0; j<5; j+=1){
      if((now_piece=self_board[i][j])){
        switch (now_piece){
          case 1: //pawn
            if(this->player && i<4){
              //black
              if(!oppn_board[i+1][j] && !self_board[i+1][j])
                all_actions.push_back(Move(Point(i, j), Point(i+1, j)));
              if(j<4 && (oppn_piece=oppn_board[i+1][j+1])){
                all_actions.push_back(Move(Point(i, j), Point(i+1, j+1)));
                if(oppn_piece==6){
                  this->game_state = WIN;
                  this->legal_actions = all_actions;
                  return;
                }
              }
              if(j>0 && (oppn_piece=oppn_board[i+1][j-1])){
                all_actions.push_back(Move(Point(i, j), Point(i+1, j-1)));
                if(oppn_piece==6){
                  this->game_state = WIN;
                  this->legal_actions = all_actions;
                  return;
                }
              }
            }else if(!this->player && i>0){
              //white
              if(!oppn_board[i-1][j] && !self_board[i-1][j])
                all_actions.push_back(Move(Point(i, j), Point(i-1, j)));
              if(j<4 && (oppn_piece=oppn_board[i-1][j+1])){
                all_actions.push_back(Move(Point(i, j), Point(i-1, j+1)));
                if(oppn_piece==6){
                  this->game_state = WIN;
                  this->legal_actions = all_actions;
                  return;
                }
              }
              if(j>0 && (oppn_piece=oppn_board[i-1][j-1])){
                all_actions.push_back(Move(Point(i, j), Point(i-1, j-1)));
                if(oppn_piece==6){
                  this->game_state = WIN;
                  this->legal_actions = all_actions;
                  return;
                }
              }
            }
            break;
          
          case 2: //rook
          case 4: //bishop
          case 5: //queen
            int st, end;
            switch (now_piece){
              case 2: st=0; end=4; break; //rook
              case 4: st=4; end=8; break; //bishop
              case 5: st=0; end=8; break; //queen
              default: st=0; end=-1;
            }
            for(int part=st; part<end; part+=1){
              auto move_list = move_table_rook_bishop[part];
              for(int k=0; k<4; k+=1){
                int p[2] = {move_list[k][0] + i, move_list[k][1] + j};
                
                if(p[0]>4 || p[0]<0 || p[1]>4 || p[1]<0) break;
                now_piece = self_board[p[0]][p[1]];
                if(now_piece) break;
                
                all_actions.push_back(Move(Point(i, j), Point(p[0], p[1])));
                
                oppn_piece = oppn_board[p[0]][p[1]];
                if(oppn_piece){
                  if(oppn_piece==6){
                    this->game_state = WIN;
                    this->legal_actions = all_actions;
                    return;
                  }else
                    break;
                };
              }
            }
            break;
          
          case 3: //knight
            for(auto move: move_table_knight){
              int x = move[0] + i;
              int y = move[1] + j;
              
              if(x>4 || x<0 || y>4 || y<0) break;
              now_piece = self_board[x][y];
              if(now_piece) continue;
              all_actions.push_back(Move(Point(i, j), Point(x, y)));
              
              oppn_piece = oppn_board[x][y];
              if(oppn_piece==6){
                this->game_state = WIN;
                this->legal_actions = all_actions;
                return;
              }
            }
            break;
          
          case 6: //king
            for(auto move: move_table_king){
              int p[2] = {move[0] + i, move[1] + j};
              
              if(p[0]>4 || p[0]<0 || p[1]>4 || p[1]<0) break;
              now_piece = self_board[p[0]][p[1]];
              if(now_piece) continue;
              
              all_actions.push_back(Move(Point(i, j), Point(p[0], p[1])));
              
              oppn_piece = oppn_board[p[0]][p[1]];
              if(oppn_piece==6){
                this->game_state = WIN;
                this->legal_actions = all_actions;
                return;
              }
            }
            break;
        }
      }
    }
  }
  
  this->legal_actions = all_actions;
}


const char piece_table[7][3] = {
  "..", "P", "R", "H", "B", "Q", "K" 
};
std::string State::encode_output(){
  std::stringstream ss;
  int now_piece;
  for(int i=0; i<5; i+=1){
    for(int j=0; j<5; j+=1){
      if((now_piece = this->board.board[0][i][j])){
        ss << "W" << piece_table[now_piece];
      }else if((now_piece = this->board.board[1][i][j])){
        ss << "B" << piece_table[now_piece];
      }else{
        ss << "..";
      }
      ss << " ";
    }
    ss << "\n";
  }
  return ss.str();
}
std::string State::encode_state(){
  std::stringstream ss;
  ss << this->player;
  ss << "\n";
  for(int pl=0; pl<2; pl+=1){
    for(int i=0; i<5; i+=1){
      for(int j=0; j<5; j+=1){
        ss << int(this->board.board[pl][i][j]);
        ss << " ";
      }
      ss << "\n";
    }
    ss << "\n";
  }
  return ss.str();
}


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
  
  while (game.game_state == UNKNOWN || game.game_state == NONE) {
    // std::cout << "test\n";
    // Output current state
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
    // system("cls");
#else
    // system("clear");
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
      //std::cout << "Read: " << x << ", " << y << std::endl;
      total ++;
    }
    fin.close();

    // std::cout << "Put: " << al[p.x] << p.y+1 << "\n";
    // log << "Put: " << al[p.x] << p.y+1 << "\n";
    // std::cout << "Depth: " << total << std::endl;
    // log << "Depth: " << total << std::endl;
    // Reset action file
    if (remove(file_action.c_str()) != 0)
      std::cerr << "Error removing file: " << file_action << "\n";
    // Take action
    if (!(temp = game.next_state(action))) {
      // If action is invalid.
      std::cout << "Invalid Action\n";
      data = game.encode_output();
      std::cout << data;
      log << data;
      break;
    }else{
      std::cout << action.first.first << " " << action.first.second << " " << action.second.first << " " << action.second.second << "\n";
    }
    game = *temp;
  }
  log.close();
  
  data = game.encode_output();
  std::cout << data << std::endl;
  log << data << std::endl;
  if(game.game_state == WIN)
    std::cout << "Player" << game.player+1 << " wins\n";
  
  // Reset state file
  if (remove(file_state.c_str()) != 0)
    std::cerr << "Error removing file: " << file_state << "\n";
  return 0;
}
