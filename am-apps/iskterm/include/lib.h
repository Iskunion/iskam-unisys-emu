#ifndef _LIB_H_
#define _LIB_H_

#include <klib.h>

char *tokenize(char *args, char serp);
int getexpr(char *expr, bool *succ);

#endif