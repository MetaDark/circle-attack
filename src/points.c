#include "points.h"
#include "utils.h"

void points_init(Points *this, Layer *parent) {
  GRect bounds = layer_get_bounds(parent);

  this->text_layer = text_layer_create((GRect) {
      .origin = { 0, bounds.size.h - 20 },
      .size = { bounds.size.w, 20 }
  });

  text_layer_set_text(this->text_layer, this->text);
  text_layer_set_text_alignment(this->text_layer, GTextAlignmentCenter);
  text_layer_set_text_color(this->text_layer, POINTS_TEXT_COLOR);
  text_layer_set_background_color(this->text_layer, POINTS_BACKGROUND_COLOR);

  layer_add_child(parent, text_layer_get_layer(this->text_layer));
}

void points_deinit(Points *this) {
  text_layer_destroy(this->text_layer);
}

void points_update(Points *this, int delta) {
  this->points += delta;
  snprintf(this->text, ARRAYSIZE(this->text), "Points: %i", this->points);
}
