#include "utils.h"
#include "background.h"

void background_init(Background *this) {
  for (size_t i = 0; i < ARRAYSIZE(this->streaks); ++i) {
    streak_init(&this->streaks[i]);
  }
}

void background_update(Background *this) {
  Streak *streak;
  for (size_t i = 0; i < ARRAYSIZE(this->streaks); ++i) {
    streak = &this->streaks[i];
    streak_update(streak);

    // Respawn streak if it goes out of bounds
    if (streak->obj.x_pos + streak->obj.size < 0) {
      streak_respawn(streak);
    }
  }
}

void background_draw(Background *this, Layer *layer, GContext *ctx) {
  for (size_t i = 0; i < ARRAYSIZE(this->streaks); ++i) {
    streak_draw(&this->streaks[i], layer, ctx);
  }
}
