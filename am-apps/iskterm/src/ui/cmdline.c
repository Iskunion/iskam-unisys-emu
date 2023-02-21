#include <io.h>
#include <basic.h>

#define LINE_BUFFER_NR 128

struct { uint8_t i, j; } cursor_pos = {0, 0};
int terminal_height, terminal_width;

char input_buffer[LINE_BUFFER_NR];
static int ib_cnt = 0;

void cli_init() {
  AM_GPU_CHCONFIG_T cfg = io_read(AM_GPU_CHCONFIG);
  if (!cfg.present) return ;
  terminal_height = cfg.height;
  terminal_width = cfg.width;
  printf("Found a terminal w %d h %d\n", terminal_width, terminal_height); 
}

void cli_putc(char ch) {
  switch (ch)
  {
    case '\n':
      cursor_pos.i++;
      cursor_pos.j = 0;
      if (cursor_pos.i == terminal_height) {
        io_write(AM_GPU_CHSCROLL, true);
        cursor_pos.i--;
      }
      break;
    case 33:
      break;
    default:
      putc_vga(cursor_pos.i, cursor_pos.j, ch, 255, 0, true);
      cursor_pos.j++;
      if (cursor_pos.j == terminal_width) cursor_pos.i++;
      if (cursor_pos.i == terminal_height) {
        io_write(AM_GPU_CHSCROLL, true);
        cursor_pos.i--;
      }
      break;
  }
  
  
  return ;
}

void prompt() {
  fprint(cli_putc, "iskunion > ");
}

int add_buffer(char ch) {
  //leave the space for '\0'
  if (ib_cnt < LINE_BUFFER_NR - 1) {
    input_buffer[ib_cnt++] = ch;
    return 0;
  }
  return -1;
}

void clr_buffer() {
  ib_cnt = 0;
}

char* push_line() {
  input_buffer[ib_cnt++] = '\0';
  cursor_pos.i += 1;
  cursor_pos.j = 0;
  // prompt();
  return input_buffer;
}