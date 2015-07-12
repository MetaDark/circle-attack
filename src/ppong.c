#include <pebble.h>
#include <stdio.h>

#include "color-polyfill.h"

#define ARRAYSIZE(x) (sizeof(x)/sizeof(*x))

static Window *window;
static Layer *game_layer;

enum {
  GAME_ACTIVE,
  GAME_OVER
} game_state = GAME_ACTIVE;

typedef struct Object {
  int x_pos;
  int y_pos;
  int x_vel;
  int y_vel;
  int size;
} Object;

static void object_update(Object *object) {
  object->y_pos += object->y_vel;
  object->x_pos += object->x_vel;
}

static int object_collides(Object *object1, Object *object2) {
  int collidesX =
    (object1->x_pos - object1->size) <= (object2->x_pos + object2->size) &&
    (object1->x_pos + object1->size) >= (object2->x_pos - object2->size);

  int collidesY =
    (object1->y_pos - object1->size) <= (object2->y_pos + object2->size) &&
    (object1->y_pos + object1->size) >= (object2->y_pos - object2->size);

  return collidesX && collidesY;
}

static int object_collides_rect(Object *object, GRect *rect) {
  int collidesX = 0;
  int collidesY = 0;

  return collidesX && collidesY;
}

typedef struct Player {
  int health;
  int points;
  Object obj;
} Player;

// Make player singleton for simplicity
static Player player = {
  .health = 100,
  .obj = {
    .x_pos = 0,
    .y_pos = 84,
    .x_vel = 0,
    .y_vel = 0,
    .size = 20
  }
};

static void player_draw(GContext *ctx) {
  // Draw the three circles
  graphics_context_set_fill_color(ctx, GColorPictonBlue);
  graphics_fill_circle(ctx, GPoint(player.obj.x_pos, player.obj.y_pos), player.obj.size);

  graphics_context_set_fill_color(ctx, GColorBlack);
  graphics_fill_circle(ctx, GPoint(player.obj.x_pos, player.obj.y_pos), player.obj.size / 2);

  graphics_context_set_fill_color(ctx, GColorWhite);
  graphics_fill_circle(ctx, GPoint(player.obj.x_pos, player.obj.y_pos), player.obj.size / 4);

  // Draw the cannon
  graphics_context_set_fill_color(ctx, GColorPictonBlue);
  graphics_fill_rect(ctx, (GRect) {
      .origin = { player.obj.x_pos + player.obj.size, player.obj.y_pos - 5 },
      .size = { 5, 10 }
  }, 0, GCornerNone);
}

static void player_health_update(int delta) {
  player.health += delta;

  if (player.health <= 0) {
    player.health = 50;
  }
}

static void player_health_draw(GContext *ctx) {
  graphics_context_set_fill_color(ctx, GColorWhite);
  graphics_fill_rect(ctx, (GRect) {
      .origin = { 4, 4 },
      .size = { (player.health * 144 / 100) - 8, 12}
  }, 0, GCornerNone);

  if (player.health > 66) {
    graphics_context_set_fill_color(ctx, GColorIslamicGreen);
  } else if (player.health > 33) {
    graphics_context_set_fill_color(ctx, GColorChromeYellow);
  } else {
    graphics_context_set_fill_color(ctx, GColorDarkCandyAppleRed);
  }

  graphics_fill_rect(ctx, (GRect) {
      .origin = { 5, 5 },
      .size = { (player.health * 144 / 100) - 10, 10 }
  }, 0, GCornerNone);
}

typedef struct Points {
  int points;
  char text[15];
  TextLayer *text_layer;
} Points;

static Points points;
static void points_update(int delta) {
  player.points += delta;
  snprintf(points.text, ARRAYSIZE(points.text), "Points: %i", player.points);
}

typedef struct Bullet {
  Object obj;
} Bullet;

// Make bullet singleton for simplicity
static Bullet bullet = {
  .obj = {
    .x_pos = -10,
    .y_pos = 0,
    .x_vel = 0,
    .y_vel = 0,
    .size = 5
  }
};

static void bullet_respawn(int x_pos, int y_pos, int x_vel, int y_vel) {
  bullet.obj.x_pos = x_pos;
  bullet.obj.y_pos = y_pos;
  bullet.obj.x_vel = x_vel;
  bullet.obj.y_vel = y_vel;
}

static void bullet_hide() {
  bullet_respawn(-bullet.obj.size * 2, -bullet.obj.size * 2, 0, 0);
}

static void bullet_update() {
  object_update(&bullet.obj);

  // Stop and hide the bullet when it goes off the screen
  if (bullet.obj.x_pos - bullet.obj.size > 144) {
    bullet_hide();
  }
}

static void bullet_draw(GContext *ctx) {
  graphics_context_set_fill_color(ctx, GColorWhite);
  graphics_fill_circle(ctx, GPoint(bullet.obj.x_pos, bullet.obj.y_pos), bullet.obj.size);
}

typedef struct Opponent {
  int dying;
  Object obj;
 #ifdef PBL_COLOR
  GColor color;
 #endif
} Opponent;

// Make opponent singleton for simplicity
static Opponent opponent = {
  .dying = 0,
  .obj = {
    .x_pos = -10,
    .y_pos = 0,
    .x_vel = 0,
    .y_vel = 0,
    .size = 10
  }
};

static void opponent_respawn() {
  opponent.dying = 0;
  opponent.obj.x_pos = 168;
  opponent.obj.y_pos = rand() % 144 - 20;
  opponent.obj.x_vel = -(rand() % 4 + 1);
  opponent.obj.y_vel = 0;
  opponent.obj.size = rand() % 25 + 5;
 #ifdef PBL_COLOR
  opponent.color = GColorFromRGB(rand() % 223 + 32, rand() % 223 + 32, rand() % 223 + 32);
 #endif
}

static void opponent_kill() {
/*   opponent.dying = 1; */
/* } */

/* static void opponent_update(GContext *ctx) { */
/*   if (opponent.dying) { */
/*     opponent.obj.size -= 1; */
/*     if (opponent.obj.size <= 0) { */
/*       opponent_respawn(); */
/*     } */
/*   } else { */
/*     object_update(&opponent.obj); */
/*   } */
/* } */

/* static void opponent_draw(GContext *ctx) { */
/*  #ifdef PBL_COLOR */
/*   graphics_context_set_fill_color(ctx, opponent.color); */
 #else
  graphics_context_set_fill_color(ctx, GColorWhite);
 #endif

  graphics_fill_circle(ctx, GPoint(opponent.obj.x_pos, opponent.obj.y_pos), opponent.obj.size);

  graphics_context_set_fill_color(ctx, GColorBlack);
  graphics_fill_circle(ctx, GPoint(opponent.obj.x_pos, opponent.obj.y_pos), opponent.obj.size / 2);

  graphics_context_set_fill_color(ctx, GColorWhite);
  graphics_fill_circle(ctx, GPoint(opponent.obj.x_pos, opponent.obj.y_pos), opponent.obj.size / 4);
}

static void background_update() {

}

static void background_draw(GContext *ctx) {

}

static void start_select_handler(ClickRecognizerRef recognizer, void *ctx) {
  bullet_respawn(player.obj.x_pos + player.obj.size, player.obj.y_pos, 10, 0);
}

static void start_up_handler(ClickRecognizerRef recognizer, void *ctx) {
  player.obj.y_vel = -5;
}

static void end_up_handler(ClickRecognizerRef recognizer, void *ctx) {
  player.obj.y_vel = 0;
}

static void start_down_handler(ClickRecognizerRef recognizer, void *ctx) {
  player.obj.y_vel = 5;
}

static void end_down_handler(ClickRecognizerRef recognizer, void *ctx) {
  player.obj.y_vel = 0;
}

static void game_layer_update_callback(Layer *layer, GContext *ctx) {
  if (game_state == GAME_ACTIVE) {
    background_update();
    background_draw(ctx);

    player_update();
    player_draw(ctx);

    opponent_update();
    opponent_draw(ctx);

    bullet_update();
    bullet_draw(ctx);

    if (object_collides(&bullet.obj, &opponent.obj)) {
      points_update(1);
      bullet_hide();
      opponent_kill();
    }

    // Opponent passes screen boundary
    if (opponent.obj.x_pos + opponent.obj.size < 0) {
      points_update(-1);
      opponent_respawn();
      vibes_short_pulse();
    }

    if (object_collides(&player.obj, &opponent.obj)) {
      player_health_update(-opponent.obj.size);
      opponent_kill();
      vibes_short_pulse();
    }

    player_health_draw(ctx);
  } else {

  }
}

static void game_layer_gameover() {

}

static void click_config_provider(void *ctx) {
  window_raw_click_subscribe(BUTTON_ID_SELECT, start_select_handler, NULL, NULL);
  window_raw_click_subscribe(BUTTON_ID_UP, start_up_handler, end_up_handler, NULL);
  window_raw_click_subscribe(BUTTON_ID_DOWN, start_down_handler, end_down_handler, NULL);
}

static void timer_callback(void *data) {
  layer_mark_dirty(game_layer);
  app_timer_register(1000 / 30, timer_callback, NULL);
}

static void main_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);

  GRect bounds = layer_get_bounds(window_layer);
  GRect frame = layer_get_frame(window_layer);

 /* #ifdef PBL_PLATFORM_APLITE */
 /*  window_set_fullscreen(window, true); */
 /* #endif */

  game_layer = layer_create((GRect) {
      .origin = { 0, 0 },
      .size = { bounds.size.w, bounds.size.h }
  });
  layer_set_update_proc(game_layer, game_layer_update_callback);
  layer_add_child(window_layer, game_layer);

  points.points = 0;
  points.text_layer = text_layer_create((GRect) {
      .origin = { 0, bounds.size.h - 20 },
      .size = { bounds.size.w, 20 }
  });
  points_update(0);
  text_layer_set_text(points.text_layer, points.text);
  text_layer_set_text_alignment(points.text_layer, GTextAlignmentCenter);
  layer_add_child(game_layer, text_layer_get_layer(points.text_layer));

  opponent_respawn();
  timer_callback(NULL);
}

static void main_window_unload(Window *window) {
  text_layer_destroy(points.text_layer);
  layer_destroy(game_layer);
}

static void init(void) {
  window = window_create();
  window_set_background_color(window, GColorBlack);
  window_set_click_config_provider(window, click_config_provider);
  window_set_window_handlers(window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload,
  });

  window_stack_push(window, true);
}

static void deinit(void) {
  window_destroy(window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
