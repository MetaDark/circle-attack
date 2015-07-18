#ifndef OVERLAY_H
#define OVERLAY_H

#include <pebble.h>

#ifdef PBL_COLOR
#define OVERLAY_TEXT_COLOR GColorWhite
#define OVERLAY_BACKGROUND_COLOR GColorDarkCandyAppleRed
#else
#define OVERLAY_TEXT_COLOR GColorBlack
#define OVERLAY_BACKGROUND_COLOR GColorWhite
#endif

typedef struct {
  TextLayer *text_layer;
} Overlay;

void overlay_init(Overlay *this, Layer *parent);
void overlay_deinit(Overlay *this);
void overlay_open(Overlay *this, const char * text);
void overlay_close(Overlay *this);

#endif
