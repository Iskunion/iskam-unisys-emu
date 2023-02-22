#include <lib.h>
#include <klib.h>

char *tokenize(char *args, char serp) {
  static char *now = NULL;
  if (args != NULL) now = args;
  if (now == NULL)
    return now;
  for (; *now == serp; now++) ;
  char *nxt = now, *res = now;
  for (; *nxt != serp && *nxt != '\0'; nxt++) ;
  if (*nxt != '\0') {
    *nxt++ = '\0';
    for (; *nxt == serp; nxt++) ;
  }
  now = nxt;
  return *res == '\0' ? NULL : res;
}