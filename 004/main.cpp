#include <cstdint>
#include <iostream>
#include <istream>
#include <memory>
#include <string>

#include "tetris.h"

namespace {

const char* btext(bool value) { return value ? "true" : "false"; }

}  // namespace

int main() {
  std::ios_base::sync_with_stdio(false);
  std::istream& input = std::cin;

  std::uint32_t seed = 888;
  std::unique_ptr<GameCore> game;
  auto ensure_game = [&]() -> GameCore& {
    if (!game) {
      game = std::make_unique<GameCore>(seed);
    }
    return *game;
  };
  auto reset_game = [&]() { game = std::make_unique<GameCore>(seed); };

  std::string token;
  while (input >> token) {
    if (token == "seed") {
      input >> seed;
      game.reset();
    } else if (token == "action") {
      std::string action_token;
      input >> action_token;
      ensure_game().applyInput(parse_action(action_token));
    } else if (token == "LEFT" || token == "RIGHT" || token == "DOWN"
               || token == "ROTATE" || token == "DROP" || token == "QUIT") {
      ensure_game().applyInput(parse_action(token));
    } else if (token == "tick") {
      int n = 0;
      input >> n;
      for (int i = 0; i < n; ++i) {
        ensure_game().step();
      }
    } else if (token == "cell") {
      int x = 0;
      int y = 0;
      char value = '.';
      input >> x >> y >> value;
      ensure_game().setBoardCellForTesting({x, y}, value);
    } else if (token == "row") {
      int y = 0;
      std::string cells;
      input >> y >> cells;
      for (int x = 0; x < BOARD_WIDTH && x < static_cast<int>(cells.size());
           ++x) {
        ensure_game().setBoardCellForTesting({x, y}, cells[x]);
      }
    } else if (token == "snap") {
      std::cout << ensure_game().snapshot();
    } else if (token == "summary") {
      const LocalSnapshot s = ensure_game().snapshot();
      std::cout << "summary tick=" << s.tick << " run=" << btext(s.running)
                << " cur=" << s.current_tetromino
                << " rot=" << s.current_rotation << " pos=" << s.block_position
                << " next=" << s.next_tetromino << " score=" << s.score
                << " level=" << s.level << " lines=" << s.lines_cleared
                << " interval=" << ensure_game().fallIntervalForTesting()
                << '\n';
    } else if (token == "interval") {
      std::cout << "interval=" << ensure_game().fallIntervalForTesting()
                << '\n';
    } else if (token == "parse") {
      std::string action_token;
      input >> action_token;
      std::cout << "parse(" << action_token
                << ")=" << action_to_string(parse_action(action_token)) << '\n';
    } else if (token == "reset") {
      reset_game();
    } else if (token == "end") {
      break;
    } else {
      std::cerr << "unknown command: " << token << '\n';
      return 2;
    }
  }
  return 0;
}
