#include <iostream>
#include <istream>
#include <string>

#include "tetris.h"

namespace {

bool has_baseline_shape(const GameState& state) {
  return state.board.size() == BOARD_HEIGHT && !state.board.empty()
         && state.board.front().size() == BOARD_WIDTH
         && state.current_block.size() == 1
         && state.current_block.front().size() == 4;
}

void print_state_summary(const GameState& state) {
  if (!has_baseline_shape(state)) {
    std::cout << "invalid\n";
    return;
  }

  std::cout << "board=" << state.board.size() << "x"
            << state.board.front().size() << '\n';
  std::cout << "block=" << state.block_x << "," << state.block_y << '\n';
  std::cout << "shape=" << state.current_block.size() << "x"
            << state.current_block.front().size() << '\n';
  std::cout << "running=" << (state.running ? "true" : "false") << '\n';
}

void print_empty_render_after_offset(GameState& state, int y) {
  state.block_y = y;
  std::cout << board_to_string(state);
}

}  // namespace

int main() {
  std::istream& input = std::cin;
  std::string command;
  input >> command;

  GameState state;
  init_game(state);

  if (command == "initial") {
    std::cout << board_to_string(state);
    print_state_summary(state);
  } else if (command == "state") {
    print_state_summary(state);
  } else if (command == "nonmutating") {
    (void)board_to_string(state);
    bool unmutated = true;
    for (int x = 4; x < 8; ++x)
      if (state.board[0][x] != '.') unmutated = false;
    std::cout << (unmutated ? "ok" : "mutated") << '\n';
  } else if (command == "offset") {
    state.block_x = 8;
    state.block_y = 0;
    std::cout << board_to_string(state);
    print_state_summary(state);
  } else if (command == "clip_left") {
    state.block_x = -2;
    state.block_y = 0;
    std::cout << board_to_string(state);
    print_state_summary(state);
  } else if (command == "offset_y") {
    state.block_x = 4;
    state.block_y = 3;
    std::cout << board_to_string(state);
    print_state_summary(state);
  } else if (command == "clip_top") {
    print_empty_render_after_offset(state, -2);
    print_state_summary(state);
  } else if (command == "clip_bottom") {
    print_empty_render_after_offset(state, BOARD_HEIGHT);
    print_state_summary(state);
  } else {
    std::cerr << "unknown command\n";
    return 2;
  }

  return 0;
}
