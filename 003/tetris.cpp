#include "tetris.h"

bool ActivePiece::operator==(const ActivePiece& other) const {
  // TODO: position 비교 + piece.cells({0,0}) 비교
  return position == other.position
         && piece.cells({0, 0}) == other.piece.cells({0, 0});
}

// 익명 namespace 에 helpers 를 두세요:
//   bool can_place(const Board& board, const Piece& piece, Position origin)
//     → piece.cells(origin) 각 좌표에 대해 inBounds + cell('.') 검사
//   void lock_piece(Board& board, const Piece& piece, Position origin)
//     → piece.cells(origin) 각 좌표를 '#' 으로 기록
//   bool spawn_piece(GameState& state)
//     → active 리셋 + can_place 로 top-out 검사 후 반환

namespace {
bool can_place(const Board& board, const Piece& piece, Position origin) {
  for (const auto& p : piece.cells(origin)) {
    if (!board.inBounds(p) || board.cell(p) != '.') return false;
  }
  return true;
}
void lock_piece(Board& board, const Piece& piece, Position origin) {
  for (auto& i : piece.cells(origin)) board.setCell(i, '#');
}
bool spawn_piece(GameState& state) {
  state.active = ActivePiece{};
  return can_place(state.board, state.active.piece, state.active.position);
}
}  // namespace

bool move_piece(GameState& state, int dx, int dy) {
  Position next{state.active.position.x + dx, state.active.position.y + dy};
  if (can_place(state.board, state.active.piece, next)) {
    state.active.position = next;
    return true;
  }
  return false;
}

void hard_drop(GameState& state) {
  while (move_piece(state, 0, 1)) {
  }
  lock_piece(state.board, state.active.piece, state.active.position);
  state.running = spawn_piece(state);
}

void update_game(GameState& state) {
  if (!state.running) return;
  ++state.tick;
  // TODO: tick % fall_interval == 0 && !move_piece(0, 1) → lock + spawn
  if (state.tick % state.fall_interval == 0 && !move_piece(state, 0, 1)) {
    lock_piece(state.board, state.active.piece, state.active.position);
    state.running = spawn_piece(state);
  }
}

void handle_game_action(GameState& state, GameAction action) {
  if (!state.running && action != GameAction::Quit) return;
  switch (action) {
    case GameAction::Left:
      move_piece(state, -1, 0);
      break;
    case GameAction::Right:
      move_piece(state, 1, 0);
      break;
    case GameAction::Down:
      if (!move_piece(state, 0, 1)) {
        lock_piece(state.board, state.active.piece, state.active.position);
        state.running = spawn_piece(state);
      }
      break;
    case GameAction::Drop:
      hard_drop(state);
      break;
    case GameAction::Quit:
      state.running = false;
      break;
    case GameAction::None:
      break;
  }
}

GameAction parse_action(const std::string& token) {
  if (token == "LEFT") return GameAction::Left;
  if (token == "RIGHT") return GameAction::Right;
  if (token == "DOWN") return GameAction::Down;
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
    case GameAction::Drop:
      return "DROP";
    case GameAction::Quit:
      return "QUIT";
    default:
      return "NONE";
  }
}
