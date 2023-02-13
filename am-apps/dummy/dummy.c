#include <am.h>
#include <klib-macros.h>
const char str[] = "Exist with code";
int main(){
  char str_tmp[50];
  char *stt = str_tmp;
  for (char *st = str; *st; st++) *stt++ = *st;
  *stt++ = '\0';
  putstr(str_tmp);
  return 0;
}