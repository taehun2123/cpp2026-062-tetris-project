#include <iostream>
#include <istream>
#include <string>

#include "tetris.h"

namespace {

const char* btext(bool value) { return value ? "true" : "false"; }

void cmd_state(const GameState& state) {
  std::cout << "state tick=" << state.tick << " x=" << state.active.position.x
            << " y=" << state.active.position.y
            << " running=" << btext(state.running) << '\n';
}

}  // namespace

int main() {
  std::ios_base::sync_with_stdio(false);
  std::istream& input = std::cin;

  GameState state;
  init_game(state);

  std::string token;
  while (input >> token) {
    if (token == "LEFT" || token == "RIGHT" || token == "DOWN"
        || token == "DROP" || token == "QUIT") {
      handle_game_action(state, parse_action(token));
    } else if (token == "tick") {
      int n = 0;
      input >> n;
      for (int i = 0; i < n; ++i) {
        update_game(state);
      }
    } else if (token == "move") {
      int dx = 0;
      int dy = 0;
      input >> dx >> dy;
      const bool ok = move_piece(state, dx, dy);
      std::cout << "move(" << dx << "," << dy << ")=" << btext(ok)
                << " x=" << state.active.position.x
                << " y=" << state.active.position.y << '\n';
    } else if (token == "cells") {
      int x = 0;
      int y = 0;
      input >> x >> y;
      std::cout << "cells(" << x << "," << y << ")=";
      bool first = true;
      for (Position cell : state.active.piece.cells({x, y})) {
        if (!first) std::cout << ' ';
        std::cout << '(' << cell.x << ',' << cell.y << ')';
        first = false;
      }
      std::cout << '\n';
    } else if (token == "paint") {
      int x = 0;
      int y = 0;
      char value = '.';
      input >> x >> y >> value;
      state.board.setCell({x, y}, value);
    } else if (token == "render") {
      std::cout << board_to_string(state);
    } else if (token == "state") {
      cmd_state(state);
    } else if (token == "parse") {
      std::string action_token;
      input >> action_token;
      std::cout << "parse(" << action_token
                << ")=" << action_to_string(parse_action(action_token)) << '\n';
    } else if (token == "equal_default") {
      GameState left;
      GameState right;
      init_game(left);
      init_game(right);
      std::cout << "equal_default=" << btext(left.active == right.active)
                << '\n';
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
