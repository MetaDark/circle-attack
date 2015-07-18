#include "overlay.h"

void overlay_init(Overlay *this, Layer *parent) {
  GRect bounds = layer_get_bounds(parent);
  this->text_layer = text_layer_create((GRect) {
      .origin = { 0, bounds.size.h / 2 - 10 },
      .size = { bounds.size.w, 20 }
  });

  text_layer_set_text_alignment(this->text_layer, GTextAlignmentCenter);
  text_layer_set_text_color(this->text_layer, OVERLAY_TEXT_COLOR);
  text_layer_set_background_color(this->text_layer, OVERLAY_BACKGROUND_COLOR);
  layer_add_child(parent, text_layer_get_layer(this->text_layer));
}

void overlay_deinit(Overlay *this) {
  text_layer_destroy(this->text_layer);
}

void overlay_open(Overlay *this, const char * text) {
  text_layer_set_text(this->text_layer, text);
  layer_set_hidden(text_layer_get_layer(this->text_layer), false);
}

void overlay_close(Overlay *this) {
  layer_set_hidden(text_layer_get_layer(this->text_layer), true);
}
