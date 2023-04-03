
typedef struct pixel_ {
  // little-endian ARGB
  unsigned char blue;
  unsigned char green;
  unsigned char red;
  unsigned char alpha;
} z_pixel;

void draw_square(int x, int y, z_pixel *px);

void draw_stripes_pattern(int x, int y, z_pixel *px);

void draw_cross(int x, int y, int height, z_pixel *px);

void draw_buffer(unsigned char *data, int width, int height,
                        int stride);