#include "tetris.h"

#include <sstream>

Board::Board() {
  for (auto& row : cells_) row.fill('.');
}

bool Board::inBounds(Position position) const {
  return position.x >= 0 && position.x < BOARD_WIDTH && position.y >= 0
         && position.y < BOARD_HEIGHT;
}

char Board::cell(Position position) const {
  return inBounds(position) ? cells_[position.y][position.x] : '?';
}

void Board::setCell(Position position, char value) {
  if (inBounds(position)) cells_[position.y][position.x] = value;
}

std::string Board::toText() const {
  std::ostringstream out;
  out << "+----------+\n";
  for (const auto& row : cells_) {
    out << '|';
    for (char cell : row) out << cell;
    out << "|\n";
  }
  out << "+----------+\n";
  return out.str();
}

Piece::Piece() : shape_{{1, 1, 1, 1}} {}

const std::vector<std::vector<int>>& Piece::shape() const { return shape_; }

void init_game(GameState& state) { state = GameState{}; }

std::string board_to_string(const GameState& state) {
  auto rendered = state.board;

  const auto& shape = state.current_piece.shape();
  for (int y = 0; y < static_cast<int>(shape.size()); ++y) {
    for (int x = 0; x < static_cast<int>(shape[y].size()); ++x) {
      if (shape[y][x] == 0) continue;
      rendered.setCell({state.piece_position.x + x, state.piece_position.y + y},
                       '#');
    }
  }
  return rendered.toText();
}