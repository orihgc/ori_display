#include "draw_buf.h"

void draw_square(int x, int y, z_pixel *px) {
  px->alpha = 255;
  px->red = 70;
  px->green = 130;
  px->blue = 180;
}

void draw_stripes_pattern(int x, int y, z_pixel *px) {
  // draw a stripes pattern
  if ((x + y) % 30 < 10) {
    // transparent
    px->alpha = 0;
  } else if ((x + y) % 30 < 20) {
    // yellow
    px->alpha = 255;
    px->red = 255;
    px->green = 255;
    px->blue = 0;
  } else {
    // semitransparent red
    px->alpha = 128;
    px->red = 255;
    px->green = 0;
    px->blue = 0;
  }
}

void draw_cross(int x, int y, int height, z_pixel *px) {
  if ((80 < x && x < 120) || (80 < y && y < 120)) {
    // gradient from blue at the top to white at the bottom
    px->alpha = 255;
    px->red = (double)y / height * 255;
    px->green = px->red;
    px->blue = 255;
  } else {
    // transparent
    px->alpha = 0;
  }
}

void draw_buffer(unsigned char *data, int width, int height,
                        int stride) {
  for (int x = 0; x < width; x++) {
    for (int y = 0; y < height; y++) {
      z_pixel *px = (z_pixel *)(data + y * stride + x * 4);
      draw_square(x, y, px);
    }
  }
}