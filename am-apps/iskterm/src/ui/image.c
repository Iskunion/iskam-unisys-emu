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

  sync_vga();

  set_alarm(WP_TIMEOUT, 1);
  while(true) {
    AM_INPUT_KEYBRD_T key_event = io_read(AM_INPUT_KEYBRD);
    if(key_event.keycode == AM_KEY_RETURN && key_event.keydown)
      break;
    sync_vga();
  } 

  printf("end display image\n");
}