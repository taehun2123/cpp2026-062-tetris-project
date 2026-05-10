#include <sstream>

#include "tetris.h"

Board::Board() {
  for (auto& row : cells_) {
    row.fill('.');
  }
}

bool Board::inBounds(Position position) const {
  return position.x >= 0 && position.x < BOARD_WIDTH && position.y >= 0
         && position.y < BOARD_HEIGHT;
}

char Board::cell(Position position) const {
  return inBounds(position) ? cells_[position.y][position.x] : '?';
}

void Board::setCell(Position position, char value) {
  if (inBounds(position)) {
    cells_[position.y][position.x] = value;
  }
}

std::string Board::toText() const {
  std::ostringstream out;
  out << "+----------+\n";
  for (const auto& row : cells_) {
    out << '|';
    for (char cell_value : row) {
      out << cell_value;
    }
    out << "|\n";
  }
  out << "+----------+\n";
  return out.str();
}

Piece::Piece()
    : relative_cells_{
          {Position{0, 0}, Position{1, 0}, Position{2, 0}, Position{3, 0}}} {}

std::array<Position, 4> Piece::cells(Position origin) const {
  std::array<Position, 4> result{};
  for (std::size_t i = 0; i < relative_cells_.size(); ++i) {
    result[i] = {origin.x + relative_cells_[i].x,
                 origin.y + relative_cells_[i].y};
  }
  return result;
}

void init_game(GameState& state) { state = GameState{}; }

std::string board_to_string(const GameState& state) {
  Board rendered = state.board;
  for (Position position : state.active.piece.cells(state.active.position)) {
    rendered.setCell(position, '#');
  }
  return rendered.toText();
}
