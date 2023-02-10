#include <iostream>
#include <sstream>

#include "./state.hpp"



State* State::next_state(Move move){
  Board next = this->board;
  Point from = move.first, to = move.second;
  
  int8_t moved = next.board[this->player][from.first][from.second];
  
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


//score of empty, pawn, rook, knight, bishop, queen, king
static const int score_table[7] = {0, 4, 20, 13, 15, 50, 100000};
int State::evaluate(){
  if(this->game_state == WIN){
    return 100000;
  }
  auto self_board = this->board.board[this->player];
  auto oppn_board = this->board.board[1 - this->player];
  
  int self_score=0, oppn_score=0;
  int8_t now_piece;
  for(int i=0; i<4; i+=1){
    for(int j=0; j<4; j+=1){
      if((now_piece = self_board[i][j])){
        self_score += score_table[now_piece];
      }else if((now_piece = oppn_board[i][j])){
        oppn_score += score_table[now_piece];
      }
    }
  }
  return self_score - oppn_score;
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
void State::print(){
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
  std::cout << ss.str();
}