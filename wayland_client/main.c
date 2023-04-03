#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>

#include <sys/mman.h>
#include <sys/syscall.h>

#include <wayland-client-protocol.h>
#include <wayland-client.h>
#include <wayland-cursor.h>

#include "draw_buf.h"
#include "xdg-shell.h"

#pragma region fields

char *g_window_title = "hgc-demo";

#pragma region global
struct wl_display *display;

struct wl_compositor *g_compositor;
struct wl_shm *g_shm;
struct wl_data_device_manager *g_data_device_mgr;
struct wl_seat *g_seat;
struct zxdg_shell_v6 *g_xdg_shell;
#pragma endregion

struct wl_surface *csr_sfc;
struct wl_cursor_theme *csr_theme;
struct wl_cursor *csr;
struct wl_cursor_image *csr_img;
struct wl_buffer *csr_buf;

struct wl_pointer *g_pointer;
struct wl_data_source *data_source;

struct wl_surface *g_sfc;
struct zxdg_surface_v6 *g_xdg_surface;
struct zxdg_toplevel_v6 *g_xdg_toplevel;

struct wl_shm_pool *g_pool;
struct wl_buffer *g_buf;
unsigned char *g_buf_data;

struct wl_surface *icon_sfc;
const int width = 720;
const int height = 420;
const int bpp = 4;
const int stride = width * bpp;
const int size = stride * height; // bytes

#pragma endregion

#pragma region register
void registry_global_handler(void *data, struct wl_registry *registry,
                             uint32_t name, const char *interface,
                             uint32_t version);

void registry_global_remove_handler(void *data, struct wl_registry *registry,
                                    uint32_t name);

const struct wl_registry_listener registry_listener = {
    .global = registry_global_handler,
    .global_remove = registry_global_remove_handler};

void registry_global_remove_handler(void *data, struct wl_registry *registry,
                                    uint32_t name) {
  printf("removed: %u\n", name);
}

void registry_global_handler(void *data, struct wl_registry *registry,
                             uint32_t name, const char *interface,
                             uint32_t version) {
  printf("[interface]: '%s', version: %u, name: %u\n", interface, version,
         name);
  if (strcmp(interface, "wl_compositor") == 0) {
    g_compositor =
        wl_registry_bind(registry, name, &wl_compositor_interface, 3);
    if (!g_compositor) {
      printf("Fatal Error: wl_compositor object unavailable\n");
      exit(1);
    }
  }

  if (strcmp(interface, "wl_shm") == 0) {
    g_shm = wl_registry_bind(registry, name, &wl_shm_interface, 1);
    if (!g_shm) {
      printf("Fatal Error: wl_shm object unavailable\n");
      exit(1);
    }
  }

  if (strcmp(interface, "wl_data_device_manager") == 0) {
    g_data_device_mgr =
        wl_registry_bind(registry, name, &wl_data_device_manager_interface, 1);
    if (!g_data_device_mgr) {
      printf("wl_data_device_manager object unavailable :(\n");
      exit(1);
    }
  }

  if (strcmp(interface, "wl_seat") == 0) {
    g_seat = wl_registry_bind(registry, name, &wl_seat_interface, 1);
    if (!g_seat) {
      printf("wl_seat object unavailable :(\n");
      exit(1);
    }
  }

  if (strcmp(interface, "zxdg_shell_v6") == 0) {
    g_xdg_shell = wl_registry_bind(registry, name, &zxdg_shell_v6_interface, 1);
    if (!g_xdg_shell) {
      printf("wl_shell object unavailable :(\n");
      exit(1);
    }
  }

  if (strcmp(interface, "org_kde_plasma_shell") == 0) {
    printf("org_kde_plasma_shell object is here.\n");
  }
}
#pragma endregion

#pragma region point

void pointer_enter_handler(void *data, struct wl_pointer *pointer,
                           uint32_t serial, struct wl_surface *surface,
                           wl_fixed_t x, wl_fixed_t y) {
  // Bind surface to its role -- cursor
  wl_pointer_set_cursor(pointer, serial, csr_sfc, csr_img->hotspot_x,
                        csr_img->hotspot_y);

  printf("enter:\t%d %d\n", wl_fixed_to_int(x), wl_fixed_to_int(y));
}

void pointer_leave_handler(void *data, struct wl_pointer *pointer,
                           uint32_t serial, struct wl_surface *surface) {
  printf("leave\n");
}

void pointer_motion_handler(void *data, struct wl_pointer *pointer,
                            uint32_t time, wl_fixed_t x, wl_fixed_t y) {
  printf("motion:\t%d %d\n", wl_fixed_to_int(x), wl_fixed_to_int(y));
}

void pointer_button_handler(void *data, struct wl_pointer *pointer,
                            uint32_t serial, uint32_t time, uint32_t button,
                            uint32_t state) {
  printf("button: 0x%x state: %d\n", button, state);
}

void pointer_axis_handler(void *data, struct wl_pointer *pointer, uint32_t time,
                          uint32_t axis, wl_fixed_t value) {
  printf("axis: %d %f\n", axis, wl_fixed_to_double(value));
}

const struct wl_pointer_listener pointer_listener = {
    .enter = pointer_enter_handler,
    .leave = pointer_leave_handler,
    .motion = pointer_motion_handler,
    .button = pointer_button_handler,
    .axis = pointer_axis_handler};

void register_pointer() {
  // this is only going to work if your computer
  // has a pointing device
  g_pointer = wl_seat_get_pointer(g_seat);
  wl_pointer_add_listener(g_pointer, &pointer_listener, NULL);
}
#pragma endregion

#pragma region surface

void xdg_toplevel_configure_handler(void *data,
                                    struct zxdg_toplevel_v6 *xdg_toplevel,
                                    int32_t width, int32_t height,
                                    struct wl_array *states) {
  printf("configure: %dx%d\n", width, height);
}

void xdg_toplevel_close_handler(void *data,
                                struct zxdg_toplevel_v6 *xdg_toplevel) {
  printf("close\n");
}

const struct zxdg_toplevel_v6_listener xdg_toplevel_listener = {
    .configure = xdg_toplevel_configure_handler,
    .close = xdg_toplevel_close_handler};

void xdg_surface_configure_handler(void *data,
                                   struct zxdg_surface_v6 *xdg_surface,
                                   uint32_t serial) {
  zxdg_surface_v6_ack_configure(xdg_surface, serial);
}

const struct zxdg_surface_v6_listener xdg_surface_listener = {
    .configure = xdg_surface_configure_handler};

void xdg_shell_ping_handler(void *data, struct zxdg_shell_v6 *xdg_shell,
                            uint32_t serial) {
  zxdg_shell_v6_pong(xdg_shell, serial);
  printf("ping-pong\n");
}

const struct zxdg_shell_v6_listener xdg_shell_listener = {
    .ping = xdg_shell_ping_handler};

void config_surface() {
  g_sfc = wl_compositor_create_surface(g_compositor);
  // Bind the surface to its role -- shell surface
  // struct wl_shell_surface* shell_surface =
  // wl_shell_get_shell_surface(shell, surface);
  // wl_shell_surface_set_toplevel(shell_surface);
  g_xdg_surface = zxdg_shell_v6_get_xdg_surface(g_xdg_shell, g_sfc);
  zxdg_surface_v6_add_listener(g_xdg_surface, &xdg_surface_listener, NULL);

  g_xdg_toplevel = zxdg_surface_v6_get_toplevel(g_xdg_surface);
  zxdg_toplevel_v6_add_listener(g_xdg_toplevel, &xdg_toplevel_listener, NULL);

  zxdg_toplevel_v6_set_title(g_xdg_toplevel, g_window_title);

  // signal that the surface is ready to be configured
  wl_surface_commit(g_sfc);
}

void render_surface() {
  // open an anonymous file and write some zero bytes to it
  int fd = syscall(SYS_memfd_create, "buffer", 0);
  ftruncate(fd, size);

  // turn it into a shared memory pool
  g_pool = wl_shm_create_pool(g_shm, fd, size);
  // allocate the buffer in that pool
  g_buf = wl_shm_pool_create_buffer(g_pool, 0, width, height, stride,
                                    WL_SHM_FORMAT_ARGB8888);

  // map file to memory
  g_buf_data = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  draw_buffer(g_buf_data, width, height, stride);

  wl_surface_attach(g_sfc, g_buf, 0, 0);
  wl_surface_commit(g_sfc);
}

#pragma endregion

#pragma region cursor
void config_cursor() {
  csr_theme = wl_cursor_theme_load(NULL, 24, g_shm);
  csr = wl_cursor_theme_get_cursor(csr_theme, "left_ptr");
  csr_img = csr->images[0];
  csr_buf = wl_cursor_image_get_buffer(csr_img);
  csr_sfc = wl_compositor_create_surface(g_compositor);
  wl_surface_attach(csr_sfc, csr_buf, 0, 0);
  wl_surface_commit(csr_sfc);
}

#pragma endregion

#pragma region data
const char *mime_type;

static void data_source_target(void *data, struct wl_data_source *source,
                               const char *mime_type) {
  printf("[data source] data_source_target\n");
}

static void data_source_send(void *data, struct wl_data_source *source,
                             const char *mime_type, int32_t fd) {
  printf("[data source] data_source_send\n");
  close(fd);
}

static void data_source_cancelled(void *data, struct wl_data_source *source) {
  printf("[data source] data_source_cancelled\n");
}

const struct wl_data_source_listener data_source_listener = {
    data_source_target, data_source_send, data_source_cancelled};
#pragma endregion

int main(int argc, char *argv[]) {

  // Step1:connect
  display = wl_display_connect(NULL);
  if (!display) {
    printf("Error! connecting :(\n");
    return 1;
  }
  printf("Connected!\n");

  // Step2:register
  struct wl_registry *registry = wl_display_get_registry(display);
  wl_registry_add_listener(registry, &registry_listener, NULL);

  // Step3
  printf("[interface] Waiting...\n");
  wl_display_roundtrip(display);
  printf("[interface] Got them all!\n");

  printf("\n"); // ---------------- 分割线 ----------------

  register_pointer();

  config_surface();

  render_surface();

  config_cursor();

  icon_sfc = wl_compositor_create_surface(g_compositor);

  data_source = wl_data_device_manager_create_data_source(g_data_device_mgr);
  if (!data_source) {
    printf("Fatal Error: wl_data_source object unavailable\n");
    return 1;
  }
  wl_data_source_add_listener(data_source, &data_source_listener, NULL);

  while (1) {
    wl_display_dispatch(display);
  }

  return 0;
}