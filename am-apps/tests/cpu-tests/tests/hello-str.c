#include "trap.h"
#include <am.h>
char buf[128];

const char s = 'w';

int main() {
	printf("%s", "Hello world!\n");
	
	// ioe_write(AM_MEDIA_PRINT, (void *) &s);
  // putstr(buf);
	//check(strcmp(buf, "Hello world!\n") == 0);

	printf("\033[35m;%d + %d = %d\n", 1, 1, 2);
//  putstr(buf);
	//check(strcmp(buf, "1 + 1 = 2\n") == 0);

	printf("%d + %d = %d\n", 2, 10, 12);
  //putstr(buf);
	//check(strcmp(buf, "2 + 10 = 12\n") == 0);
	return 0;
}
