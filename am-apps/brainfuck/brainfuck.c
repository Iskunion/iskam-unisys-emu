#include <am.h>
#include <klib.h>
#include <klib-macros.h>

#define NR_CELLS 256
#define NR_CODE  4096

//256B TRM
uint8_t machine[NR_CELLS];
uint32_t addr = 0;

//4KB CODE
char code[NR_CODE];

int main(){
  char *pc = code, ch;
  
  //download code via uart
  while (ch = io_read(AM_UART_RX).data) *pc++ = ch;
  *pc = '\0';

  for (pc = code; *pc; pc++) {
    switch (*pc)
    {
      case '+':
        printf("Add at 0x%02x\n", addr);
        ++machine[addr];
        break;
      case '-':
        printf("Sub at 0x%02x\n", addr);
        --machine[addr];
        break;
      case '<':
        addr = !addr ? NR_CELLS - 1 : addr - 1;
        printf("Move Left to 0x%02x\n", addr);
        break;
      case '>':
        addr = (addr == NR_CELLS - 1) ? 0 : addr + 1; 
        printf("Move Right to 0x%02x\n", addr);
        break;
      case '.':
        putch((char) machine[addr]);
        printf("putch: %c\n", (char) machine[addr]);
        break;
      case ',':
        ch = io_read(AM_INPUT_KEYBRD).keycode;
        machine[addr] = ch;
        printf("getch: %c\n", ch);
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
          } while (score);
          printf("skip loop to 0x%03x\n", (int) (pc - code));
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
          printf("continue loop to 0x%03x\n", (int) (pc - code));
          pc++;
        }
        break;
      case '\0':
        halt(0);
      default:
        printf("Ignore.\n");
        break;
    }
  }
}