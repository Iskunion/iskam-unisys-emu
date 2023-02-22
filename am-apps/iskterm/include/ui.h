#ifndef __UI_H
#define __UI_H

#include <stdint.h>

//image
extern int wallpaper_height;
extern int wallpaper_width;
extern char wallpaper[];

void display_image();

//cmdline
extern char input_buffer[];
int add_buffer(char ch);
int del_buffer();
void prompt();
void clr_buffer();
void cli_init();
void cli_putc(char ch);
char* push_line();

extern struct { uint8_t i, j; } cursor_pos;

#endif