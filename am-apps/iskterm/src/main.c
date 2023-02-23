#include <am.h>
#include <klib-macros.h>
#include <klib.h>
#include <io.h>
#include <ui.h>
#include <shell.h>
#include <app.h>

int main(){
  // flush_vga();
  display_image();
  io_write(AM_GPU_CHSCROLL, false);
  // typing_game();
  // video_test();
  cli_init();
  flush_vga();
  shell_host();
  return 0;
}