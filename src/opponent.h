#ifndef OPPONENT_H
#define OPPONENT_H

#include "object.h"

typedef struct Opponent {
  int dying;
  Object obj;
#ifdef PBL_COLOR
  GColor color;
#endif
} Opponent;

void opponent_respawn(Opponent *this);
void opponent_kill(Opponent *this);
void opponent_update(Opponent *this);
void opponent_draw(Opponent *this, GContext *ctx);

#endif
