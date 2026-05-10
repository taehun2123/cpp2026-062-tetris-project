#ifndef TETRIS_ASSIGNMENT_001_TETRIS_H
#define TETRIS_ASSIGNMENT_001_TETRIS_H

#include <string>
#include <vector>

constexpr int BOARD_WIDTH = 10;
constexpr int BOARD_HEIGHT = 20;

struct GameState {
  std::vector<std::vector<char>> board;
  int block_x = 4;
  int block_y = 0;
  std::vector<std::vector<int>> current_block;
  bool running = true;
};

void init_game(GameState& state);
std::string board_to_string(const GameState& state);

#endif
