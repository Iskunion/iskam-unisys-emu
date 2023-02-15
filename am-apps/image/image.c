#include <am.h>
#include <klib-macros.h>
#include <stdio.h>

extern uint8_t wallpaper;

int main(){
  
  char *image_data = &wallpaper;
  
  io_write(AM_GPU_DRAW256, 0, 0, image_data, 320, 240, 0);
  io_write(AM_GPU_DRAW256, 0, 0, NULL, 0, 0, 1);

  while(1) io_write(AM_GPU_DRAW256, 0, 0, NULL, 0, 0, 1);;

  return 0;
}