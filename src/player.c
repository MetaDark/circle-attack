#include "player.h"

#include "color-polyfill.h"

void player_update(Player *this) {
  object_update(&this->obj);

  // HACK:
  /* if (this->obj.y_pos < 0) { */
  /*   this->obj.y_pos = 0; */
  /* } else if (this->obj.y_pos > 144) { */
  /*   this->obj.y_pos = 144; */
  /* } */
}

void player_draw(Player *this, GContext *ctx) {
  // Draw the three circles
  graphics_context_set_fill_color(ctx, GColorPictonBlue);
  graphics_fill_circle(ctx, GPoint(this->obj.x_pos, this->obj.y_pos), this->obj.size);

  graphics_context_set_fill_color(ctx, GColorBlack);
  graphics_fill_circle(ctx, GPoint(this->obj.x_pos, this->obj.y_pos), this->obj.size / 2);

  graphics_context_set_fill_color(ctx, GColorWhite);
  graphics_fill_circle(ctx, GPoint(this->obj.x_pos, this->obj.y_pos), this->obj.size / 4);

  // Draw the cannon
  graphics_context_set_fill_color(ctx, GColorPictonBlue);
  graphics_fill_rect(ctx, (GRect) {
      .origin = { this->obj.x_pos + this->obj.size, this->obj.y_pos - 5 },
        .size = { 5, 10 }
    }, 0, GCornerNone);
}

void player_health_update(Player *this, int delta) {
  this->health += delta;

  // HACK:
  /* if (this->health <= 0) { */
  /*   game_over(); */
  /* } */
}

void player_health_draw(Player *this, GContext *ctx) {
  graphics_context_set_fill_color(ctx, GColorWhite);
  graphics_fill_rect(ctx, (GRect) {
      .origin = { 4, 4 },
        .size = { (this->health * 144 / 100) - 8, 12}
    }, 0, GCornerNone);

  if (this->health > 66) {
    graphics_context_set_fill_color(ctx, GColorIslamicGreen);
  } else if (this->health > 33) {
    graphics_context_set_fill_color(ctx, GColorChromeYellow);
  } else {
    graphics_context_set_fill_color(ctx, GColorDarkCandyAppleRed);
  }

  graphics_fill_rect(ctx, (GRect) {
      .origin = { 5, 5 },
        .size = { (this->health * 144 / 100) - 10, 10 }
    }, 0, GCornerNone);
}
