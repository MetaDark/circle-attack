#ifndef OPPONENT_H
#define OPPONENT_H

#include "object.h"

typedef struct Opponent {
  Object obj;
  int dying;
#ifdef PBL_COLOR
  GColor8 color;
#endif
} Opponent;

void opponent_init(Opponent *this);
void opponent_respawn(Opponent *this);
void opponent_kill(Opponent *this);
void opponent_update(Opponent *this);
void opponent_draw(Opponent *this, Layer *layer, GContext *ctx);

#endif
