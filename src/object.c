#include "object.h"

/* void object_init(Object *this, Layer *layer) { */
/*   this->layer = layer; */
/* } */

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

/* static int object_collides_rect(Object *object, GRect *rect) { */
/*   int collidesX = 0; */
/*   int collidesY = 0; */

/*   return collidesX && collidesY; */
/* } */
