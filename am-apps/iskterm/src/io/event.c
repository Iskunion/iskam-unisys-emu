#include <basic.h>
#include <io.h>

static char ascii_upper[128] = {
  [AM_KEY_0] =')',
  [AM_KEY_1] ='!',
  [AM_KEY_2] ='@',
  [AM_KEY_3] ='#',
  [AM_KEY_4] ='$',
  [AM_KEY_5] ='%',
  [AM_KEY_6] ='^',
  [AM_KEY_7] ='&',
  [AM_KEY_8] ='*',
  [AM_KEY_9] ='(',
  [AM_KEY_APOSTROPHE] ='~',
  [AM_KEY_A] ='A',
  [AM_KEY_B] ='B',
  [AM_KEY_COMMA] ='<',
  [AM_KEY_C] ='C',
  [AM_KEY_D] ='D',
  [AM_KEY_EQUALS] ='+',
  [AM_KEY_E] ='E',
  [AM_KEY_F] ='F',
  [AM_KEY_GRAVE] ='\"',
  [AM_KEY_BACKSLASH] ='|',
  [AM_KEY_G] ='G',
  [AM_KEY_H] ='H',
  [AM_KEY_I] ='I',
  [AM_KEY_J] ='J',
  [AM_KEY_K] ='K',
  [AM_KEY_LEFTBRACKET] ='{',
  [AM_KEY_L] ='L',
  [AM_KEY_MINUS] ='_',
  [AM_KEY_M] ='M',
  [AM_KEY_N] ='N',
  [AM_KEY_O] ='O',
  [AM_KEY_PERIOD] ='>',
  [AM_KEY_P] ='P',
  [AM_KEY_Q] ='Q',
  [AM_KEY_RIGHTBRACKET] ='}',
  [AM_KEY_R] ='R',
  [AM_KEY_SEMICOLON] =':',
  [AM_KEY_SLASH] ='?',
  [AM_KEY_SPACE] =' ',
  [AM_KEY_S] ='S',
  [AM_KEY_T] ='T',
  [AM_KEY_U] ='U',
  [AM_KEY_V] ='V',
  [AM_KEY_W] ='W',
  [AM_KEY_X] ='X',
  [AM_KEY_Y] ='Y',
  [AM_KEY_Z] ='Z'
};

static char ascii_lower[128] = {
  [AM_KEY_0] ='0',
  [AM_KEY_1] ='1',
  [AM_KEY_2] ='2',
  [AM_KEY_3] ='3',
  [AM_KEY_4] ='4',
  [AM_KEY_5] ='5',
  [AM_KEY_6] ='6',
  [AM_KEY_7] ='7',
  [AM_KEY_8] ='8',
  [AM_KEY_9] ='9',
  [AM_KEY_APOSTROPHE] ='`',
  [AM_KEY_A] ='a',
  [AM_KEY_B] ='b',
  [AM_KEY_COMMA] =',',
  [AM_KEY_C] ='c',
  [AM_KEY_D] ='d',
  [AM_KEY_EQUALS] ='=',
  [AM_KEY_E] ='e',
  [AM_KEY_F] ='f',
  [AM_KEY_GRAVE] ='\'',
  [AM_KEY_BACKSLASH] ='\\',
  [AM_KEY_G] ='g',
  [AM_KEY_H] ='h',
  [AM_KEY_I] ='i',
  [AM_KEY_J] ='j',
  [AM_KEY_K] ='k',
  [AM_KEY_LEFTBRACKET] ='[]',
  [AM_KEY_L] ='l',
  [AM_KEY_MINUS] ='-',
  [AM_KEY_M] ='m',
  [AM_KEY_N] ='n',
  [AM_KEY_O] ='o',
  [AM_KEY_PERIOD] ='.',
  [AM_KEY_P] ='p',
  [AM_KEY_Q] ='q',
  [AM_KEY_RIGHTBRACKET] =']',
  [AM_KEY_R] ='r',
  [AM_KEY_SEMICOLON] =':',
  [AM_KEY_SLASH] ='/',
  [AM_KEY_SPACE] =' ',
  [AM_KEY_S] ='s',
  [AM_KEY_T] ='t',
  [AM_KEY_U] ='u',
  [AM_KEY_V] ='v',
  [AM_KEY_W] ='w',
  [AM_KEY_X] ='x',
  [AM_KEY_Y] ='y',
  [AM_KEY_Z] ='z'
};

static struct {
  bool ctrl;
  bool shift;
  bool capslock;
  bool capslock_pressed;
} kbd_ctl_status;

ISKEvent wait_event() {
  while (true) {
    AM_INPUT_KEYBRD_T kbd = io_read(AM_INPUT_KEYBRD);
    switch (kbd.keycode)
    {
      case AM_KEY_LSHIFT:
      case AM_KEY_RSHIFT:
        if (kbd.keydown)
          kbd_ctl_status.shift = true;
        else kbd_ctl_status.shift = false;
        break;
      case AM_KEY_LCTRL:
        if (kbd.keydown)
          kbd_ctl_status.ctrl = true;
        else kbd_ctl_status.ctrl = false;
        break;
      case AM_KEY_CAPSLOCK:
        if (kbd.keydown) {
          if (!kbd_ctl_status.capslock_pressed) {
            kbd_ctl_status.capslock_pressed = true;
            if (kbd_ctl_status.capslock) 
              kbd_ctl_status.capslock = false;
            else kbd_ctl_status.capslock = true;
          }
        }
        else kbd_ctl_status.capslock_pressed = false;
      case AM_KEY_TAB:
        if (!kbd.keydown) break;  
        break;
      case AM_KEY_ESCAPE:
        if (!kbd.keydown) break;
        return (ISKEvent) {.event_type = ISKEVENT_QUIT, .event_val.code = 0};
      case AM_KEY_BACKSPACE:
        if (!kbd.keydown) break;
        break;
      case AM_KEY_RETURN:
        if (!kbd.keydown) break;
        return (ISKEvent) {.event_type = ISKEVENT_RETURN, .event_val.code = 0};
      default:
        if (!kbd.keydown) break;
        if ((kbd_ctl_status.capslock && kbd_ctl_status.shift) || (!kbd_ctl_status.capslock && !kbd_ctl_status.shift))
          return (ISKEvent) {.event_type = ISKEVENT_KEYBOARD, .event_val.ascii = ascii_lower[kbd.keycode]};
        else return (ISKEvent) {.event_type = ISKEVENT_KEYBOARD, .event_val.ascii = ascii_upper[kbd.keycode]};
        break;
    }
  }
}