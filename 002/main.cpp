#include <iostream>
#include <istream>
#include <string>

#include "tetris.h"

namespace {

const char* btext(bool value) { return value ? "true" : "false"; }

void cmd_state(const GameState& state) {
  const auto& shape = state.current_piece.shape();
  std::cout << "state pos=" << state.piece_position.x << ","
            << state.piece_position.y << " run=" << btext(state.running)
            << " shape=" << shape.size() << "x"
            << (shape.empty() ? 0u : shape.front().size()) << " cells=";
  for (const auto& row : shape) {
    for (int value : row) {
      std::cout << value;
    }
  }
  std::cout << '\n';
}

}  // namespace

int main() {
  std::ios_base::sync_with_stdio(false);
  std::istream& input = std::cin;

  GameState state;
  init_game(state);

  std::string token;
  while (input >> token) {
    if (token == "move") {
      int x = 0;
      int y = 0;
      input >> x >> y;
      state.piece_position = {x, y};
    } else if (token == "paint") {
      int x = 0;
      int y = 0;
      char value = '.';
      input >> x >> y >> value;
      state.board.setCell({x, y}, value);
    } else if (token == "render") {
      std::cout << board_to_string(state);
    } else if (token == "board") {
      std::cout << state.board.toText();
    } else if (token == "cell") {
      int x = 0;
      int y = 0;
      input >> x >> y;
      std::cout << "cell(" << x << "," << y << ")=" << state.board.cell({x, y})
                << '\n';
    } else if (token == "inb") {
      int x = 0;
      int y = 0;
      input >> x >> y;
      std::cout << "inb(" << x << "," << y
                << ")=" << btext(state.board.inBounds({x, y})) << '\n';
    } else if (token == "state") {
      cmd_state(state);
    } else if (token == "reset") {
      init_game(state);
    } else if (token == "end") {
      break;
    } else {
      std::cerr << "unknown command: " << token << '\n';
      return 2;
    }
  }
  return 0;
}
