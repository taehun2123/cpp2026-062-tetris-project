#include "tetris.h"

#include <sstream>

void init_game(GameState& state) {
  state.board.assign(BOARD_HEIGHT, std::vector<char>(BOARD_WIDTH, '.'));
  state.block_x = 4;
  state.block_y = 0;
  state.current_block = {{1, 1, 1, 1}};  // I 블록, 네 칸
  state.running = true;
}

std::string board_to_string(const GameState& state) {
  auto rendered = state.board;  // 원본을 건드리지 않을 복사본

  for (int y = 0; y < static_cast<int>(state.current_block.size()); ++y) {
    for (int x = 0; x < static_cast<int>(state.current_block[y].size()); ++x) {
      if (state.current_block[y][x] == 0) continue;
      const int board_x = state.block_x + x;
      const int board_y = state.block_y + y;
      if (board_y >= 0 && board_y < BOARD_HEIGHT && board_x >= 0
          && board_x < BOARD_WIDTH) {
        rendered[board_y][board_x] = '#';
      }
    }
  }

  std::ostringstream out;
  out << "+----------+\n";
  for (const auto& row : rendered) {
    out << '|';
    for (char cell : row) out << cell;
    out << "|\n";
  }
  out << "+----------+\n";
  return out.str();
}
