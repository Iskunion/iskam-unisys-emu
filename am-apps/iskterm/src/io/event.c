#include <basic.h>
#include <io.h>

static struct {
  bool ctrl     : 1;
  bool shift    : 1;
  bool capslock : 1;
} kbd_ctl_status;

ISKEvent wait_event() {
  return (ISKEvent) {.event_type = ISKEVENT_NIL, .event_val = 0};
}