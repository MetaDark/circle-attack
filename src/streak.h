#ifndef STREAK_H
#define STREAK_H

#include <pebble.h>
#include "object.h"

#ifdef PBL_COLOR
#define STREAK_COLOR GColorDarkGray
#else
#define STREAK_COLOR GColorWhite
#endif

typedef struct Streak {
  Object obj;
} Streak;

void streak_init(Streak *this);
void streak_respawn(Streak *this);
void streak_update(Streak *this);
void streak_draw(Streak *this, Layer *layer, GContext *ctx);

#endif
