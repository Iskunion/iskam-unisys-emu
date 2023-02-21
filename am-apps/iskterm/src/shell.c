#include <basic.h>
#include <io.h>
#include <ui.h>

enum {
  GENERAL_ERROR = -1
};

static void welcome() {
  fprint(cli_putc, "Hello iskam %s\n", __TIME__);
  fprint(cli_putc, "Powered by ISKUNION\n");
}
static int goodbye() {
  halt(0);
  return GENERAL_ERROR;
}

void shell_mainloop() {
  while (true) {
    ISKEvent e = wait_event();
    switch (e.event_type)
    {
      case ISKEVENT_KEYBOARD: {
        switch (e.event_val.ascii)
        {
          case '\n':

            break;
          
          default:
            
            break;
        }
      }
        break;
      case ISKEVENT_QUIT:
        goodbye();
        break;
      default:
        break;
    }
  }
  return ;
}

typedef int (*handler_t) (char *);

void shell_host() {
  welcome();
  shell_mainloop();
  return ;
}