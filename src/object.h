#ifndef OBJECT_H
#define OBJECT_H

#include "pebble.h"

typedef struct Object {
  int x_pos;
  int y_pos;
  int x_vel;
  int y_vel;
  int size;
} Object;

void object_update(Object *this);
int object_collides(Object *this, Object *object);
int object_collides_rect(Object *this, GRect rect);
void object_lock_rect(Object *this, GRect rect);

#endif
