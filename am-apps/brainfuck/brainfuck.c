#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define NR_CELLS 256
#define NR_CODE  4096

void halt(int code) {
  exit(code);
}

//256B TRM
uint8_t machine[NR_CELLS];
uint32_t addr = 0;

//4KB CODE
char code[NR_CODE];

int main(){
// setvbuf(stdin, NULL, _IONBF, 1024);

  char *pc = code, ch;
  freopen("brainfuck.log", "w", stderr);

  FILE *codefile = fopen("brainfuck.txt", "r");
  //download code
  uint32_t size = fread(code, sizeof (char), NR_CODE, codefile);
  if (size >= NR_CODE - 1) {
    printf("too large code!");
    halt(-1);
  }
  code[size] = '\0';

  for (pc = code; *pc; pc++) {
    switch (*pc)
    {
      case '+':
        fprintf(stderr, "Add at 0x%02x\n", addr);
        ++machine[addr];
        break;
      case '-':
        fprintf(stderr, "Sub at 0x%02x\n", addr);
        --machine[addr];
        break;
      case '<':
        addr = !addr ? NR_CELLS - 1 : addr - 1;
        fprintf(stderr, "Move Left to 0x%02x\n", addr);
        break;
      case '>':
        addr = (addr == NR_CELLS - 1) ? 0 : addr + 1; 
        fprintf(stderr, "Move Right to 0x%02x\n", addr);
        break;
      case '.':
        putchar((char) machine[addr]);
        fprintf(stderr, "putch: %c\n", (char) machine[addr]);
        break;
      case ',':
        ch = getchar();
        machine[addr] = ch;
        fprintf(stderr, "getch: %c\n", ch);
        break;
      case '[':
        if (!machine[addr]) {
          uint32_t score = 0;
          do {
            if (!*pc) {
              printf("invalid jump\n");
              halt(-1);
            }
            if (*pc == '[') score++;
            if (*pc == ']') score--;
            pc++;
            // fprintf(stderr,"s");
          } while (score);
          fprintf(stderr,"skip loop to 0x%03x\n", (int) (pc - code));
          pc--;
        }
        break;
      case ']':
        if (machine[addr]) {
          uint32_t score = 0;
          do {
            if (pc + 1 == code) {
              printf("invalid jump\n");
              halt(-1);
            }
            if (*pc == ']') score++;
            if (*pc == '[') score--;
            pc--;
          } while (score);
          fprintf(stderr, "continue loop to 0x%03x\n", (int) (pc - code));
          pc++;
        }
        break;
      case '\0':
        halt(0);
      default:
        fprintf(stderr, "Ignore.\n");
        break;
    }
  }
}