#ifndef POINTS_H
#define POINTS_H

#include <pebble.h>

#ifdef PBL_COLOR
#define POINTS_TEXT_COLOR GColorWhite
#define POINTS_BACKGROUND_COLOR GColorDarkGray
#else
#define POINTS_TEXT_COLOR GColorBlack
#define POINTS_BACKGROUND_COLOR GColorWhite
#endif

typedef struct Points {
  int points;
  char text[15];
  TextLayer *text_layer;
} Points;

void points_init(Points *this, Layer *parent);
void points_deinit(Points *this);
void points_update(Points *this, int delta);

#endif
