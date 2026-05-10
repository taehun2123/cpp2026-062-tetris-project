#include <ostream>
#include <sstream>
#include <stdexcept>

#include "tetris.h"

namespace {

using Rotations = std::array<Block, 4>;

constexpr Block make_block(
    std::array<std::array<int, BLOCK_SIZE>, BLOCK_SIZE> rows) {
  return rows;
}

const std::array<Rotations, TETROMINO_COUNT>& tetrominoes() {
  static const std::array<Rotations, TETROMINO_COUNT> data{{
      Rotations{{
          make_block(
              {{{0, 0, 0, 0}, {1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}}}),
          make_block(
              {{{0, 0, 1, 0}, {0, 0, 1, 0}, {0, 0, 1, 0}, {0, 0, 1, 0}}}),
          make_block(
              {{{0, 0, 0, 0}, {0, 0, 0, 0}, {1, 1, 1, 1}, {0, 0, 0, 0}}}),
          make_block(
              {{{0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}}}),
      }},
      Rotations{{
          make_block(
              {{{0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}}),
          make_block(
              {{{0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}}),
          make_block(
              {{{0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}}),
          make_block(
              {{{0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}}),
      }},
      Rotations{{
          make_block(
              {{{0, 1, 0, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}}),
          make_block(
              {{{0, 1, 0, 0}, {0, 1, 1, 0}, {0, 1, 0, 0}, {0, 0, 0, 0}}}),
          make_block(
              {{{0, 0, 0, 0}, {1, 1, 1, 0}, {0, 1, 0, 0}, {0, 0, 0, 0}}}),
          make_block(
              {{{0, 1, 0, 0}, {1, 1, 0, 0}, {0, 1, 0, 0}, {0, 0, 0, 0}}}),
      }},
      Rotations{{
          make_block(
              {{{0, 1, 1, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}}),
          make_block(
              {{{0, 1, 0, 0}, {0, 1, 1, 0}, {0, 0, 1, 0}, {0, 0, 0, 0}}}),
          make_block(
              {{{0, 0, 0, 0}, {0, 1, 1, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}}}),
          make_block(
              {{{1, 0, 0, 0}, {1, 1, 0, 0}, {0, 1, 0, 0}, {0, 0, 0, 0}}}),
      }},
      Rotations{{
          make_block(
              {{{1, 1, 0, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}}),
          make_block(
              {{{0, 0, 1, 0}, {0, 1, 1, 0}, {0, 1, 0, 0}, {0, 0, 0, 0}}}),
          make_block(
              {{{0, 0, 0, 0}, {1, 1, 0, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}}}),
          make_block(
              {{{0, 1, 0, 0}, {1, 1, 0, 0}, {1, 0, 0, 0}, {0, 0, 0, 0}}}),
      }},
      Rotations{{
          make_block(
              {{{1, 0, 0, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}}),
          make_block(
              {{{0, 1, 1, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}, {0, 0, 0, 0}}}),
          make_block(
              {{{0, 0, 0, 0}, {1, 1, 1, 0}, {0, 0, 1, 0}, {0, 0, 0, 0}}}),
          make_block(
              {{{0, 1, 0, 0}, {0, 1, 0, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}}}),
      }},
      Rotations{{
          make_block(
              {{{0, 0, 1, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}}),
          make_block(
              {{{0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}}}),
          make_block(
              {{{0, 0, 0, 0}, {1, 1, 1, 0}, {1, 0, 0, 0}, {0, 0, 0, 0}}}),
          make_block(
              {{{1, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}, {0, 0, 0, 0}}}),
      }},
  }};
  return data;
}

}  // namespace

Position operator+(Position lhs, Position rhs) {
  return {lhs.x + rhs.x, lhs.y + rhs.y};
}

std::ostream& operator<<(std::ostream& out, Position position) {
  out << position.x << "," << position.y;
  return out;
}

std::ostream& operator<<(std::ostream& out, const LocalSnapshot& snapshot) {
  out << snapshot_to_text(snapshot);
  return out;
}

std::string snapshot_to_text(const LocalSnapshot& snapshot) {
  std::ostringstream out;
  out << "tick=" << snapshot.tick << "\n";
  out << "running=" << (snapshot.running ? 1 : 0) << "\n";
  out << "current=" << snapshot.current_tetromino << "\n";
  out << "rotation=" << snapshot.current_rotation << "\n";
  out << "position=" << snapshot.block_position << "\n";
  out << "next=" << snapshot.next_tetromino << "\n";
  out << "score=" << snapshot.score << "\n";
  out << "level=" << snapshot.level << "\n";
  out << "lines=" << snapshot.lines_cleared << "\n";
  for (const auto& row : snapshot.board_rows) {
    out << row << "\n";
  }
  return out.str();
}

GameAction parse_action(const std::string& token) {
  if (token == "LEFT") return GameAction::Left;
  if (token == "RIGHT") return GameAction::Right;
  if (token == "DOWN") return GameAction::Down;
  if (token == "ROTATE") return GameAction::Rotate;
  if (token == "DROP") return GameAction::Drop;
  if (token == "QUIT") return GameAction::Quit;
  return GameAction::None;
}

std::string action_to_string(GameAction action) {
  switch (action) {
    case GameAction::Left:
      return "LEFT";
    case GameAction::Right:
      return "RIGHT";
    case GameAction::Down:
      return "DOWN";
    case GameAction::Rotate:
      return "ROTATE";
    case GameAction::Drop:
      return "DROP";
    case GameAction::Quit:
      return "QUIT";
    case GameAction::None:
      return "NONE";
  }
  return "NONE";
}

const Block& tetromino_block(int tetromino, int rotation) {
  if (tetromino < 0 || tetromino >= TETROMINO_COUNT) {
    throw std::out_of_range("tetromino");
  }
  return tetrominoes()[tetromino][rotation % 4];
}

int tetromino_rotation_count(int tetromino) {
  if (tetromino == 1) {
    return 1;
  }
  if (tetromino == 0) {
    return 2;
  }
  return 4;
}
