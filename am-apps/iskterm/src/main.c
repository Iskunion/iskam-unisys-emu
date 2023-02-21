#include <am.h>
#include <klib-macros.h>
#include <klib.h>
#include <io.h>
#include <ui.h>
#include <shell.h>

int main(){
  // flush_vga();
  display_image();
  cli_init();
  flush_vga();
  fprint(cli_putc, "Hello iskam %s", __TIME__);
  shell_host();
  return 0;
}