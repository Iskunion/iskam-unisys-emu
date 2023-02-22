#ifndef __IO_H
#define __IO_H

#include <stdbool.h>
#include <stdint.h>

//timer
void set_alarm(uint32_t _timeout, int timer_no);
bool check_alarm(int timer_no);

//event
typedef struct ISKEvent {
  uint8_t event_type;
  union {
    uint8_t ascii; //KEYBOARD, UART_RX
    uint8_t code;
  }event_val;
} ISKEvent;

enum {
  ISKEVENT_KEYBOARD,
  ISKEVENT_UART_RX,
  ISKEVENT_QUIT,
  ISKEVENT_RETURN,
  ISKEVENT_BACKSPACE,
  ISKEVENT_NIL
};

ISKEvent wait_event();

#endif