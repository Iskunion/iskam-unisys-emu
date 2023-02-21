#include <basic.h>
#include <io.h>

#define TIMER_NR 2

static uint32_t base_time[TIMER_NR];
static uint32_t timeout[TIMER_NR];

void set_alarm(uint32_t _timeout, int timer_no) {
  base_time[timer_no] = io_read(AM_TIMER_UPTIME).us;
  timeout[timer_no] = _timeout;
  return ;
}

bool check_alarm(int timer_no) {
  return io_read(AM_TIMER_UPTIME).us - base_time[timer_no] > timeout[timer_no];
}
