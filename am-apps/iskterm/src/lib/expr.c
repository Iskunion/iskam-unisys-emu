#include <klib.h>

const char optr[7] = { '+','-','*','/','%','<','>' };
const int priority[7] = { 1,1,5,5,5,0,0 };
inline int findOptr(char s) {
	for (int i = 0; i < 7; i++)
		if (optr[i] == s) return i;
	return -1;
}
static int end = 0;
static int work(char *expr, int start, int level, bool *succ) {
	uint32_t ans = 0;
	bool flag = false;
	for (;; start = end + 1) {
		if (!*succ) return 0xffffffff;
		if (expr[start] <= '9' && expr[start] >= '0') {
			ans = expr[start] - '0', end = start;
		}
		if (~findOptr(expr[start]) || expr[start] == '~') {
			if (flag) {
				int now = findOptr(expr[start]);
				if (priority[now] <= level) { end = start - 1; break; }
				if (expr[start] == '+') ans = ans + work(expr, start + 1, priority[now], succ);
				if (expr[start] == '-') ans = ans - work(expr, start + 1, priority[now], succ);
				if (expr[start] == '*') ans = ans * work(expr, start + 1, priority[now], succ);
				if (expr[start] == '/') {
					int tmp = work(expr, start + 1, priority[now], succ);
					if (!tmp) {
						*succ = false;
						return 0xffffffff;
					}
					ans = ans / tmp;
				}
				if (expr[start] == '%') {
					int tmp = work(expr, start + 1, priority[now], succ);
					if (!tmp) {
						*succ = false;
						return 0xffffffff;
					}
					ans = ans % tmp;
				}
				if (expr[start] == '<') ans = ans < work(expr, start + 1, priority[now], succ);
				if (expr[start] == '>') ans = ans > work(expr, start + 1, priority[now], succ);
			}
			else {
				if (expr[start] == '+') ans = +work(expr, start + 1, 8, succ);
				if (expr[start] == '-') ans = -work(expr, start + 1, 8, succ);
				if (expr[start] == '~') ans = ~work(expr, start + 1, 8, succ);
			}
		}
		if (expr[start] == '(') { ans = work(expr, start + 1, -1, succ); ++end; }
		if (expr[start] == ')') {
			end = start - 1; break;
		}
		if (expr[start] == '\0') {
			end = start - 1; break;
		}
		flag = true;
	}
	return ans;
}

int getexpr(char *expr, bool *succ) {
  end = 0;
  return work(expr, 0, -1, succ);
}