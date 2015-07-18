#include "player.h"

void player_init(Player *this) {
  this->health = 100;
  this->obj.x_pos = 0;
  this->obj.y_pos = 84; // HARD CODED HACK
  this->obj.x_vel = 0;
  this->obj.y_vel = 0;
  this->obj.size = 20;
}

void player_update(Player *this) {
  object_update(&this->obj);
}

void player_draw(Player *this, Layer *layer, GContext *ctx) {
  // Draw the three circles
  graphics_context_set_fill_color(ctx, PLAYER_COLOR);
  graphics_fill_circle(ctx, GPoint(this->obj.x_pos, this->obj.y_pos), this->obj.size);

  graphics_context_set_fill_color(ctx, GColorBlack);
  graphics_fill_circle(ctx, GPoint(this->obj.x_pos, this->obj.y_pos), this->obj.size / 2);

  graphics_context_set_fill_color(ctx, GColorWhite);
  graphics_fill_circle(ctx, GPoint(this->obj.x_pos, this->obj.y_pos), this->obj.size / 4);

  // Draw the cannon
  graphics_context_set_fill_color(ctx, PLAYER_COLOR);
  graphics_fill_rect(ctx, (GRect) {
      .origin = { this->obj.x_pos + this->obj.size, this->obj.y_pos - 5 },
      .size = { 5, 10 }
  }, 0, GCornerNone);
}

void player_health_update(Player *this, int delta) {
  this->health += delta;
}

void player_health_draw(Player *this, Layer *layer, GContext *ctx) {
  GRect bounds = layer_get_bounds(layer);

#ifdef PBL_COLOR
  graphics_context_set_fill_color(ctx, GColorWhite);
#else
  graphics_context_set_fill_color(ctx, GColorBlack);
#endif
  graphics_fill_rect(ctx, (GRect) {
      .origin = { 4, 4 },
      .size = { bounds.size.w - 8, 12}
  }, 0, GCornerNone);

#ifdef PBL_COLOR
  if (this->health > 66) {
    graphics_context_set_fill_color(ctx, GColorIslamicGreen);
  } else if (this->health > 33) {
    graphics_context_set_fill_color(ctx, GColorChromeYellow);
  } else {
    graphics_context_set_fill_color(ctx, GColorDarkCandyAppleRed);
  }
#else
  graphics_context_set_fill_color(ctx, GColorWhite);
#endif

  graphics_fill_rect(ctx, (GRect) {
      .origin = { 5, 5 },
      .size = { (this->health * bounds.size.w / 100) - 10, 10 }
   }, 0, GCornerNone);
}
