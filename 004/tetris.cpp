#include "tetris.h"

#include <algorithm>
// 9
GameCore::GameCore(std::uint32_t seed) : rng_{seed} {
  resetBoard();
  next_tetromino_ = 0;
  spawnNewBlock();
}
// 10
void GameCore::applyInput(GameAction action) {
  if (!running_ && action != GameAction::Quit) return;

  switch (action) {
    case GameAction::Left:
      moveBlock(-1, 0);
      break;
    case GameAction::Right:
      moveBlock(1, 0);
      break;
    case GameAction::Down:
      if (!moveBlock(0, 1)) {
        lockBlock();
        spawnNewBlock();
      }
      break;
    case GameAction::Rotate:
      rotateBlock();
      break;
    case GameAction::Drop:
      hardDrop();
      break;
    case GameAction::Quit:
      running_ = false;
      break;
    default:
      break;
  }
}
// 11
void GameCore::step() {
  if (!running_) return;
  if (++tick_ % fall_interval_ == 0 && !moveBlock(0, 1)) {
    lockBlock();
    spawnNewBlock();
  }
}
// 12
LocalSnapshot GameCore::snapshot() const {
  LocalSnapshot s;
  s.tick = tick_;
  s.current_tetromino = current_tetromino_;
  s.current_rotation = current_rotation_;
  s.block_position = block_position_;
  s.next_tetromino = next_tetromino_;
  s.score = score_;
  s.level = level_;
  s.lines_cleared = lines_cleared_;
  s.running = running_;

  Board rendered = board_;
  for (int y = 0; y < BLOCK_SIZE; ++y) {
    for (int x = 0; x < BLOCK_SIZE; ++x) {
      if (current_block_[y][x] == 0) continue;
      const Position bp = block_position_ + Position{x, y};
      rendered[bp.y][bp.x] = '#';
    }
  }
  for (int y = 0; y < BOARD_HEIGHT; ++y)
    s.board_rows[y].assign(rendered[y].begin(), rendered[y].end());

  return s;
}
// 13
void GameCore::setBoardCellForTesting(Position p, char c) {
  if (0 <= p.x && p.x < BOARD_WIDTH && 0 <= p.y && p.y < BOARD_HEIGHT)
    board_[p.y][p.x] = c;
}
// 14
int GameCore::fallIntervalForTesting() const { return fall_interval_; }

// 1
void GameCore::resetBoard() {
  for (auto& row : board_) row.fill('.');
}
// 7
void GameCore::spawnNewBlock() {
  current_tetromino_ = next_tetromino_;
  current_rotation_ = 0;
  current_block_ = tetromino_block(current_tetromino_, current_rotation_);
  block_position_ = {3, 0};
  next_tetromino_ = rng_() % TETROMINO_COUNT;

  if (!isValidPosition(block_position_, current_block_)) running_ = false;
}
// 2
bool GameCore::isValidPosition(Position position, const Block& block) const {
  for (int y = 0; y < BLOCK_SIZE; ++y) {
    for (int x = 0; x < BLOCK_SIZE; ++x) {
      if (block[y][x] == 0) continue;
      int board_x = position.x + x, board_y = position.y + y;

      if (board_x < 0 || board_x >= BOARD_WIDTH || board_y < 0
          || board_y >= BOARD_HEIGHT)
        return false;

      if (board_[board_y][board_x] != '.') return false;
    }
  }
  return true;
}
// 3
bool GameCore::moveBlock(int dx, int dy) {
  Position nxt_pos = block_position_ + Position{dx, dy};
  if (!isValidPosition(nxt_pos, current_block_)) return false;
  block_position_ = nxt_pos;
  return true;
}
// 4
void GameCore::rotateBlock() {
  int rot_count = tetromino_rotation_count(current_tetromino_);
  int nxt_rot = (current_rotation_ + 1) % rot_count;
  const Block& nxt_block = tetromino_block(current_tetromino_, nxt_rot);
  if (isValidPosition(block_position_, nxt_block)) {
    current_block_ = nxt_block;
    current_rotation_ = nxt_rot;
  }
}
// 5
void GameCore::hardDrop() {
  while (moveBlock(0, 1)) {
  }

  lockBlock();
  spawnNewBlock();
}
// 6
void GameCore::lockBlock() {
  for (int y = 0; y < BLOCK_SIZE; ++y) {
    for (int x = 0; x < BLOCK_SIZE; ++x) {
      if (current_block_[y][x] == 0) continue;

      int bx = block_position_.x + x, by = block_position_.y + y;
      if (by >= 0 && by < BOARD_HEIGHT && bx >= 0 && bx < BOARD_WIDTH)
        board_[by][bx] = '#';
    }
  }
  checkLines();
}
// 8
void GameCore::checkLines() {
  int cleared = 0;
  for (int y = BOARD_HEIGHT - 1; y >= 0; --y) {
    const bool full = std::all_of(board_[y].begin(), board_[y].end(),
                                  [](char cell) { return cell == '#'; });
    if (!full) continue;

    for (int move_y = y; move_y > 0; --move_y)
      board_[move_y] = board_[move_y - 1];
    board_[0].fill('.');

    ++cleared;
    ++y;
  }
  score_ += cleared * 100 * level_;
  lines_cleared_ += cleared;
  level_ = lines_cleared_ / 10 + 1;
  fall_interval_ = std::max(1, 11 - level_);
}