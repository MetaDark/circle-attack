#ifndef PLAYER_H
#define PLAYER_H

#include "object.h"

typedef struct Player {
  Object obj;
  int health;
} Player;

#include "player.h"

void player_init(Player *this, Layer *layer);
void player_update(Player *this);
void player_draw(Player *this, GContext *ctx);
void player_health_update(Player *this, int delta);
void player_health_draw(Player *this, GContext *ctx);

#endif
