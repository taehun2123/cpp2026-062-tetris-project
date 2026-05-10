#ifndef TETRIS_ASSIGNMENT_003_TETRIS_H
#define TETRIS_ASSIGNMENT_003_TETRIS_H

#include <array>
#include <string>

constexpr int BOARD_WIDTH = 10;
constexpr int BOARD_HEIGHT = 20;

struct Position {
  int x = 0;
  int y = 0;

  bool operator==(const Position& other) const {
    return x == other.x && y == other.y;
  }
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

  std::array<Position, 4> cells(Position origin) const;

 private:
  // Ch03 은 이동 규칙 학습을 위해 4칸 I-piece 하나만 사용한다.
  std::array<Position, 4> relative_cells_{};
};

struct ActivePiece {
  Piece piece;
  Position position{4, 0};

  bool operator==(const ActivePiece& other) const;
};

enum class GameAction { None, Left, Right, Down, Drop, Quit };

struct GameState {
  Board board;
  ActivePiece active;
  int tick = 0;
  int fall_interval = 10;
  bool running = true;
};

void init_game(GameState& state);
bool move_piece(GameState& state, int dx, int dy);
void hard_drop(GameState& state);
void update_game(GameState& state);
void handle_game_action(GameState& state, GameAction action);
std::string board_to_string(const GameState& state);
GameAction parse_action(const std::string& token);
std::string action_to_string(GameAction action);

#endif
