#ifndef BACKGROUND_H
#define BACKGROUND_H

#include "streak.h"

typedef struct Background {
  Streak streaks[10];
} Background;

void background_init(Background *this);
void background_update(Background *this);
void background_draw(Background *this, Layer *layer, GContext *ctx);

#endif
