#include <pebble.h>
#include <stdio.h>

#include "background.h"
#include "overlay.h"
#include "points.h"

#include "player.h"
#include "opponent.h"
#include "bullet.h"

#define BACKGROUND_COLOR GColorBlack

enum {
  GAME_ACTIVE,
  GAME_PAUSED,
  GAME_OVER
} game_state = GAME_ACTIVE;

static Window *window;
static Layer *render_layer;

static Background background;
static Overlay overlay;
static Points points;

static Player player;
static Opponent opponent;
static Bullet bullet;

static void timer_callback(void *data) {
  layer_mark_dirty(render_layer);

  switch (game_state) {
  case GAME_ACTIVE:
  case GAME_OVER:
    app_timer_register(1000 / 30, timer_callback, NULL);
  case GAME_PAUSED:
    break;
  }
}

static void game_init() {
  game_state = GAME_ACTIVE;

  overlay_close(&overlay);

  points.points = 0;
  points_update(&points, 0);

  player_init(&player);
  opponent_init(&opponent);
  bullet_init(&bullet);

  timer_callback(NULL);
}

static void game_over() {
  game_state = GAME_OVER;
  overlay_open(&overlay, "GAME OVER");
}

static void game_pause() {
  game_state = GAME_PAUSED;
  overlay_open(&overlay, "PAUSED");
}

static void game_unpause() {
  game_state = GAME_ACTIVE;
  overlay_close(&overlay);
  timer_callback(NULL);
}

static void game_update(GRect bounds) {
  // Update positions of objects
  player_update(&player);
  opponent_update(&opponent);
  bullet_update(&bullet);

  // Keep the player within the screen
  object_lock_rect(&player.obj, bounds);

  // Opponent passes through left side of screen
  if (opponent.obj.x_pos + opponent.obj.size < 0) {
    points_update(&points, -1);
    opponent_respawn(&opponent);
    vibes_short_pulse();
  }

  // Opponent hits player
  if (object_collides(&player.obj, &opponent.obj)) {
    player_health_update(&player, -opponent.obj.size);
    opponent_kill(&opponent);
    vibes_short_pulse();
  }

  // Bullet hits opponent
  if (object_collides(&bullet.obj, &opponent.obj)) {
    points_update(&points, 1);
    bullet_hide(&bullet);
    opponent_kill(&opponent);
  }

  // Bullet goes off the screen
  if (!object_collides_rect(&bullet.obj, bounds)) {
    bullet_hide(&bullet);
  }

  // Player looses all health
  if (player.health <= 0) {
    game_over();
  }
}

static void game_draw(Layer *layer, GContext *ctx) {
  // Draw objects
  player_draw(&player, layer, ctx);
  opponent_draw(&opponent, layer, ctx);
  bullet_draw(&bullet, layer, ctx);

  // Draw static UI
  player_health_draw(&player, layer, ctx);
}

static void render_layer_update_callback(Layer *layer, GContext *ctx) {
  background_update(&background);
  background_draw(&background, layer, ctx);

  GRect bounds;
  switch (game_state) {
  case GAME_ACTIVE:
    bounds = layer_get_bounds(layer);
    game_update(bounds);
  case GAME_PAUSED:
    game_draw(layer, ctx);
    break;
  case GAME_OVER:
    break;
  }
}

static void click_back_handler(ClickRecognizerRef recognizer, void *ctx) {
  switch (game_state) {
  case GAME_ACTIVE:
    game_pause();
    break;
  case GAME_PAUSED:
  case GAME_OVER:
    window_stack_pop(true);
    break;
  }
}

static void start_select_handler(ClickRecognizerRef recognizer, void *ctx) {
  switch (game_state) {
  case GAME_ACTIVE:
    bullet_respawn(&bullet, player.obj.x_pos + player.obj.size, player.obj.y_pos, 10, 0);
    break;
  case GAME_PAUSED:
    game_unpause();
    break;
  case GAME_OVER:
    game_init();
    break;
  }
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

static void click_config_provider(void *ctx) {
  window_single_click_subscribe(BUTTON_ID_BACK, click_back_handler);
  window_raw_click_subscribe(BUTTON_ID_SELECT, start_select_handler, NULL, NULL);
  window_raw_click_subscribe(BUTTON_ID_UP, start_up_handler, end_up_handler, NULL);
  window_raw_click_subscribe(BUTTON_ID_DOWN, start_down_handler, end_down_handler, NULL);
}

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect window_bounds = layer_get_bounds(window_layer);

  // Initialize the render layer
  render_layer = layer_create((GRect) {
      .origin = { 0, 0 },
      .size = { window_bounds.size.w, window_bounds.size.h - 20 }
  });
  layer_set_update_proc(render_layer, render_layer_update_callback);
  layer_add_child(window_layer, render_layer);

  background_init(&background);
  points_init(&points, window_layer);
  overlay_init(&overlay, render_layer);

  game_init(window);
}

static void window_unload(Window *window) {
  points_deinit(&points);
  overlay_deinit(&overlay);
  layer_destroy(render_layer);
}

static void init(void) {
  window = window_create();

  window_set_background_color(window, BACKGROUND_COLOR);
  window_set_click_config_provider(window, click_config_provider);
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });

#ifdef PBL_PLATFORM_APLITE
  window_set_fullscreen(window, true);
#endif

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
