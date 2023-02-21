#include <basic.h>
#include <io.h>

#define WP_TIMEOUT 5000000

extern char wallpaper[];
extern int wallpaper_width, wallpaper_height;

void display_image() {
  int x = (io_read(AM_GPU_CONFIG).width - wallpaper_width) >> 1;
  int y = (io_read(AM_GPU_CONFIG).height - wallpaper_height) >> 1;
  io_write(AM_GPU_DRAW256, 0, 0, wallpaper, wallpaper_width, wallpaper_height, 0);
  printf("displaying image with size %d*%d at (%d, %d)\n", wallpaper_width, wallpaper_height, x, y);

  set_alarm(WP_TIMEOUT, 1);
  while(!check_alarm(1)) sync_vga();

  printf("end display image\n");
}