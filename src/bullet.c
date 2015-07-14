#include "bullet.h"

void bullet_respawn(Bullet *this, int x_pos, int y_pos, int x_vel, int y_vel) {
  this->obj.x_pos = x_pos;
  this->obj.y_pos = y_pos;
  this->obj.x_vel = x_vel;
  this->obj.y_vel = y_vel;
}

void bullet_hide(Bullet *this) {
  bullet_respawn(this, -this->obj.size * 2, -this->obj.size * 2, 0, 0);
}

void bullet_update(Bullet *this) {
  object_update(&this->obj);

  // HACK
  // Stop and hide the bullet when it goes off the screen
  /* if (bullet.obj.x_pos - bullet.obj.size > 144) { */
  /*   bullet_hide(); */
  /* } */
}

void bullet_draw(Bullet *this, GContext *ctx) {
  graphics_context_set_fill_color(ctx, GColorWhite);
  graphics_fill_circle(ctx, GPoint(this->obj.x_pos, this->obj.y_pos), this->obj.size);
}
