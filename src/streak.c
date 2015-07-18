#include "streak.h"

void streak_init(Streak *this) {
  streak_respawn(this);
}

void streak_respawn(Streak *this) {
  int velocity = (rand() % 3 + 2);
  this->obj.x_pos = 144;
  this->obj.y_pos = rand() % 148;
  this->obj.x_vel = -velocity;
  this->obj.y_vel = 0;
  this->obj.size = 15 * velocity;
}

void streak_update(Streak *this) {
  object_update(&this->obj);
}

void streak_draw(Streak *this, Layer *layer, GContext *ctx) {
  graphics_context_set_stroke_color(ctx, STREAK_COLOR);
  graphics_draw_line(ctx, (GPoint) {
      .x = this->obj.x_pos,
      .y = this->obj.y_pos
  }, (GPoint) {
      .x = this->obj.x_pos + this->obj.size,
      .y = this->obj.y_pos
  });
}
