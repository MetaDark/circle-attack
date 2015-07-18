#include "opponent.h"

void opponent_init(Opponent *this) {
  opponent_respawn(this);
}

void opponent_respawn(Opponent *this) {
  /* GRect bounds = layer_get_bounds(this->obj.layer); */
  this->dying = 0;
  this->obj.x_pos = 144 + 24;
  this->obj.y_pos = rand() % 144 - 20;
  this->obj.x_vel = -(rand() % 4 + 1);
  this->obj.y_vel = 0;
  this->obj.size = rand() % 25 + 5;
#ifdef PBL_COLOR
  // 85 - 170 % 1 << 24
  this->color = GColorFromRGB(rand() % 191 + 32, rand() % 191 + 32, rand() % 191 + 32);
#endif
}

void opponent_kill(Opponent *this) {
  this->dying = 1;
}

void opponent_update(Opponent *this) {
  if (this->dying) {
    this->obj.size -= this->obj.size / 2 + 1;
    if (this->obj.size <= 0) {
      opponent_respawn(this);
    }
  } else {
    object_update(&this->obj);
  }
}

void opponent_draw(Opponent *this, Layer *layer, GContext *ctx) {
#ifdef PBL_COLOR
  graphics_context_set_fill_color(ctx, this->color);
#else
  graphics_context_set_fill_color(ctx, GColorWhite);
#endif

  graphics_fill_circle(ctx, GPoint(this->obj.x_pos, this->obj.y_pos), this->obj.size);

  graphics_context_set_fill_color(ctx, GColorBlack);
  graphics_fill_circle(ctx, GPoint(this->obj.x_pos, this->obj.y_pos), this->obj.size / 2);

  graphics_context_set_fill_color(ctx, GColorWhite);
  graphics_fill_circle(ctx, GPoint(this->obj.x_pos, this->obj.y_pos), this->obj.size / 4);
}
