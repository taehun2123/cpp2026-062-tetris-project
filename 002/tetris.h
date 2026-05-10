#ifndef TETRIS_ASSIGNMENT_002_TETRIS_H
#define TETRIS_ASSIGNMENT_002_TETRIS_H

#include <array>
#include <string>
#include <vector>

constexpr int BOARD_WIDTH = 10;
constexpr int BOARD_HEIGHT = 20;

struct Position {
  int x = 0;
  int y = 0;
};

class Board {
 public:
  Board();

  bool inBounds(Position position) const;
  char cell(Position position) const;
  void setCell(Position position, char value);
  std::string toText() const;

 private:
  std::array<std::array<char, BOARD_WIDTH>, BOARD_HEIGHT> cells_{};
};

class Piece {
 public:
  Piece();

  const std::vector<std::vector<int>>& shape() const;

 private:
  std::vector<std::vector<int>> shape_;
};

struct GameState {
  Board board;
  Piece current_piece;
  Position piece_position{4, 0};
  bool running = true;
};

void init_game(GameState& state);
std::string board_to_string(const GameState& state);

#endif
