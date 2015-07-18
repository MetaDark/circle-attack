#ifndef BULLET_H
#define BULLET_H

#include "object.h"

typedef struct Bullet {
  Object obj;
} Bullet;

void bullet_init(Bullet *this);
void bullet_respawn(Bullet *this, int x_pos, int y_pos, int x_vel, int y_vel);
void bullet_hide(Bullet *this);
void bullet_update(Bullet *this);
void bullet_draw(Bullet *this, Layer *layer, GContext *ctx);

#endif
