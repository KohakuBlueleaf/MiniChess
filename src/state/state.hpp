#ifndef __STATE_H__
#define __STATE_H__

#include <cstdlib>
#include <vector>
#include <utility>


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
    int evaluate();
    void print();
};

#endif