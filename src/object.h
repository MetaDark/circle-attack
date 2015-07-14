#ifndef OBJECT_H
#define OBJECT_H

#include "pebble.h"

typedef struct Object {
  Layer *layer;
  int x_pos;
  int y_pos;
  int x_vel;
  int y_vel;
  int size;
} Object;

/* void object_init(Object *this, Layer *layer); */
void object_update(Object *this);
int object_collides(Object *this, Object *object);
/* int object_collides_rect(Object *object, GRect *rect); */

#endif
