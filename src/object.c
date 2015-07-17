#include "object.h"

void object_init(Object *this, Layer *layer) {
  this->layer = layer;
}

void object_update(Object *this) {
  this->y_pos += this->y_vel;
  this->x_pos += this->x_vel;
}

int object_collides(Object *this, Object *object) {
  int collidesX =
    (this->x_pos - this->size) <= (object->x_pos + object->size) &&
    (this->x_pos + this->size) >= (object->x_pos - object->size);

  int collidesY =
    (this->y_pos - this->size) <= (object->y_pos + object->size) &&
    (this->y_pos + this->size) >= (object->y_pos - object->size);

  return collidesX && collidesY;
}

int object_collides_rect(Object *this, GRect rect) {
  int collidesX =
    (this->x_pos - this->size) <= (rect.origin.x + rect.size.w) &&
    (this->x_pos + this->size) >= (rect.origin.x);

  int collidesY =
    (this->y_pos - this->size) <= (rect.origin.y + rect.size.h) &&
    (this->y_pos + this->size) >= (rect.origin.y);

  return collidesX && collidesY;
}

void object_retain_rect(Object *this, GRect rect) {
  if (this->x_pos < 0) {
    this->x_pos = 0;
  } else if (this->x_pos > rect.size.w) {
    this->x_pos = rect.size.w;
  }

  if (this->y_pos < 0) {
    this->y_pos = 0;
  } else if (this->y_pos > rect.size.h) {
    this->y_pos = rect.size.h;
  }
}
