#include <klib.h>
#include <am.h>
#include <klib-macros.h>
int main(){
  int a=0;
  int b=5;
  int c=a+b;
  while (a < 1000000) {
    a++;
    if (a%100000 == 0)
    printf("now time is %d\n", io_read(AM_TIMER_UPTIME).us);
  }
  
  printf("sgsgsg\n");
  return 0;
}