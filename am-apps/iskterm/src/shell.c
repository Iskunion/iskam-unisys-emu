#include <basic.h>
#include <io.h>
#include <ui.h>

enum {
  GENERAL_ERROR = -1
};

static void welcome() {
  fprint(cli_putc, "Hello iskam %s\n", __TIME__);
  fprint(cli_putc, "Powered by ISKUNION\n");
  prompt();
}
static int goodbye() {
  printf("Terminating...\n");
  halt(0);
  return GENERAL_ERROR;
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
      case ISKEVENT_RETURN:
        printf("line: %d breaks at %d\n", cursor_pos.i, cursor_pos.j);
        push_line();
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

typedef int (*handler_t) (char *);

void shell_host() {
  welcome();
  shell_mainloop();
  return ;
}