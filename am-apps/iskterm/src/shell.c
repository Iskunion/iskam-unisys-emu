#include <basic.h>
#include <io.h>
#include <ui.h>
#include <lib.h>
#include <app.h>

enum {
  GENERAL_ERROR = -1
};

typedef int (*handler_t) (char *);

static int cmd_help(char *args);
static int cmd_time(char *args);
static int cmd_fib(char *args);
static int cmd_disp(char *args);
static int cmd_video(char *args) {
  video_test();
  io_write(AM_GPU_CHSCROLL, false);
  return 0;
}
static int cmd_typing(char *args) {
  typing_game();
  io_write(AM_GPU_CHSCROLL, false);
  return 0;
}
static int cmd_clear(char *args) {
  clear_vga();
  cursor_pos.i = 0;
  cursor_pos.j = 0;
  return 0;
}
static int cmd_hello(char *args) {
  if (args == NULL) {
    fprint(cli_putc, "Hello World!\n");
    return 0;
  }
  fprint(cli_putc, "Hello, %s\n", tokenize(NULL, ' '));
  return 0;
}
static int cmd_expr(char *args) {
  if (args == NULL) {
    fprint(cli_putc, "Missing operand\n");
    return GENERAL_ERROR;
  }
  bool succ = true;
  int res = getexpr(args, &succ);
  fprint(cli_putc, "Ans: %d\n", res);
  return 0;
}
static int cmd_dry(char *args) {
  uint32_t ans = dry();
  fprint(cli_putc, "Mark: %d\n", ans);
  return 0;
}

static struct {
  const char *name;
  const char *usage;
  const char *description;
  handler_t handler;
} cmd_table [] = {
  { "help", "help", "Show command infos", cmd_help },
  { "time", "time", "Show system time", cmd_time },
  { "fib", "fib [N]", "Calculate fib_n", cmd_fib },
  { "display", "display", "Display boot picture", cmd_disp },
  { "video", "video", "View a video", cmd_video },
  { "typing", "typing", "Start typing game", cmd_typing },
  { "clear", "clear", "Clear the screen", cmd_clear },
  { "expr", "expr [expr]", "Calculate the expression", cmd_expr },
  { "hello", "hello [something]", "say hello to somthing you like", cmd_hello },
  { "dry", "dry", "run drystone benchmark", cmd_dry },
  /* TODO: Add more commands */
};

#define NR_CMD (sizeof(cmd_table) / sizeof(cmd_table[0]))

static int cmd_help(char *args) {
  char *arg = tokenize(NULL, ' ');
  int i;

  if (arg == NULL) {
    /* no argument given */
    for (i = 0; i < NR_CMD; i ++) {
      fprint(cli_putc, "%s", cmd_table[i].name);
      fprint(cli_putc, "%c", (i + 1) % 4u ? ' ' : '\n');
    }
    if (i % 4u) fprint(cli_putc, "%c", '\n');
    fprint(cli_putc, "Use help [subcmd] for more\n");
  }
  else {
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(arg, cmd_table[i].name) == 0) {
        fprint(cli_putc, "%s-Usage: %s\n %s.\n", cmd_table[i].name ,cmd_table[i].usage, cmd_table[i].description);
        return 0;
      }
    }
    fprint(cli_putc, "Unknown command '%s'\n", arg);
  }
  return 0;
}

static int cmd_time(char *args) {
  fprint(cli_putc, "now sys time: %d.%03d s\n", io_read(AM_TIMER_UPTIME).us / 1000000, io_read(AM_TIMER_UPTIME).us / 1000 % 1000);
  return 0;
}

static int cmd_fib(char *args) {
  char *arg = tokenize(NULL, ' ');
  if (arg == NULL) {
    fprint(cli_putc, "Missing operand!\n");
    return -1;
  }
  else {
    bool succ = true;
    int n;
    if (strlen(args) > 2) {
      fprint(cli_putc, "Too long!\n");
      return -1;
    }
    if (*(args + 1)) n = (*args - '0') * 10 + (*(args + 1) - '0');
    else n = *args - '0';
    if (n > 45) {
      fprint(cli_putc, "Too large!\n");
      return -1;
    }
    if (!n) {
      fprint(cli_putc, "Invalid operand!\n");
      return -1;
    }
    int l1 = 0, l2 = 1;
    while (n--) {
      int t = l2;
      l2 = l2 + l1;
      l1 = t;
    }
    fprint(cli_putc, "Ans: %d\n", l1);
  }
  return 0;
}

static int cmd_disp(char *args) {
  display_image();
  io_write(AM_GPU_CHSCROLL, false);
}

static void welcome() {
  fprint(cli_putc, "Hello iskam %d ms since boot\n", io_read(AM_TIMER_UPTIME).us / 1000);
  fprint(cli_putc, "Powered by ISKUNION\n");
  prompt();
}
static int goodbye() {
  printf("Terminating...\n");
  halt(0);
  return GENERAL_ERROR;
}

void shell_handler(char *str) {
  int i;

  char *str_end = str + strlen(str);

  char *cmd = tokenize(str, ' ');
  if (cmd == NULL) return ;

  char *args = cmd + strlen(cmd) + 1;
  if (args >= str_end) {
    args = NULL;
  }

  for (i = 0; i < NR_CMD; i ++) {
    if (strcmp(cmd, cmd_table[i].name) == 0) {
      if (cmd_table[i].handler(args) < 0) { return; }
      break;
    }
  }

  if (i == NR_CMD) { fprint(cli_putc, "Unknown command '%s'\n", cmd); }
}

void shell_mainloop() {
  while (true) {
    ISKEvent e = wait_event();
    switch (e.event_type)
    {
      case ISKEVENT_KEYBOARD: {
        add_buffer(e.event_val.ascii);
        
        // switch (e.event_val.ascii)
        // {
        //   case '\n':

        //     break;
        //   default:
            
        //     break;
        // }
      }
        break;
      case ISKEVENT_BACKSPACE:
        del_buffer();
        break;
      case ISKEVENT_RETURN:
        printf("line: %d breaks at %d\n", cursor_pos.i, cursor_pos.j);
        shell_handler(push_line());
        clr_buffer();
        prompt();
        break;
      case ISKEVENT_QUIT:
        goodbye();
        break;
      default:
        break;
    }
    sync_vga();
  }
  return ;
}

void shell_host() {
  welcome();
  shell_mainloop();
  return ;
}