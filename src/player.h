#ifndef PLAYER_H
#define PLAYER_H

#include <pebble.h>
#include "object.h"

#ifdef PBL_COLOR
#define PLAYER_COLOR GColorPictonBlue
#else
#define PLAYER_COLOR GColorWhite
#endif

typedef struct Player {
  Object obj;
  int health;
} Player;

#include "player.h"

void player_init(Player *this);
void player_update(Player *this);
void player_draw(Player *this, Layer * layer, GContext *ctx);
void player_health_update(Player *this, int delta);
void player_health_draw(Player *this, Layer *layer, GContext *ctx);

#endif
