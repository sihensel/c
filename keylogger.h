#ifndef H_KEYCODES
#define H_KEYCODES

#include <linux/input-event-codes.h>

/*
   Linux input event codes:
   https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git/tree/include/uapi/linux/input-event-codes.h
*/

/**
  * Translate a keyboard character from input_event.code
  * These keycodes are not exhaustive and only cover the US layout
  *
  * @param keycode Keycode from linux/input-event-codes.h
  * @returns The string representation of that keycode
*/
char *translate_keycode(unsigned int keycode) {

    switch (keycode) {
        case KEY_A: return "a"; break;
        case KEY_B: return "b"; break;
        case KEY_C: return "c"; break;
        case KEY_D: return "d"; break;
        case KEY_E: return "e"; break;
        case KEY_F: return "f"; break;
        case KEY_G: return "g"; break;
        case KEY_H: return "h"; break;
        case KEY_I: return "i"; break;
        case KEY_J: return "j"; break;
        case KEY_K: return "k"; break;
        case KEY_L: return "l"; break;
        case KEY_M: return "m"; break;
        case KEY_N: return "n"; break;
        case KEY_O: return "o"; break;
        case KEY_P: return "p"; break;
        case KEY_Q: return "q"; break;
        case KEY_R: return "r"; break;
        case KEY_S: return "s"; break;
        case KEY_T: return "t"; break;
        case KEY_U: return "u"; break;
        case KEY_V: return "v"; break;
        case KEY_W: return "w"; break;
        case KEY_X: return "x"; break;
        case KEY_Y: return "y"; break;
        case KEY_Z: return "z"; break;

        case KEY_1: return "1"; break;
        case KEY_2: return "2"; break;
        case KEY_3: return "3"; break;
        case KEY_4: return "4"; break;
        case KEY_5: return "5"; break;
        case KEY_6: return "6"; break;
        case KEY_7: return "7"; break;
        case KEY_8: return "8"; break;
        case KEY_9: return "9"; break;
        case KEY_0: return "0"; break;

        case KEY_F1:  return "[F1]"; break;
        case KEY_F2:  return "[F2]"; break;
        case KEY_F3:  return "[F3]"; break;
        case KEY_F4:  return "[F4]"; break;
        case KEY_F5:  return "[F5]"; break;
        case KEY_F6:  return "[F6]"; break;
        case KEY_F7:  return "[F7]"; break;
        case KEY_F8:  return "[F8]"; break;
        case KEY_F9:  return "[F9]"; break;
        case KEY_F10: return "[F10]"; break;
        case KEY_F11: return "[F11]"; break;
        case KEY_F12: return "[F12]"; break;

        case KEY_MINUS:      return "[-_]"; break;
        case KEY_EQUAL:      return "[=+]"; break;
        case KEY_BACKSPACE:  return "[BACKSPACE]"; break;
        case KEY_TAB:        return "[TAB]"; break;
        case KEY_LEFTBRACE:  return "[[{]"; break;
        case KEY_RIGHTBRACE: return "[]}]"; break;
        case KEY_ENTER:      return "[ENTER]"; break;
        case KEY_LEFTCTRL:   return "[CTRL]"; break;
        case KEY_RIGHTCTRL:  return "[CTRL]"; break;
        case KEY_SEMICOLON:  return "[;:]"; break;
        case KEY_APOSTROPHE: return "['\"]"; break;
        case KEY_GRAVE:      return "[`~]"; break;
        case KEY_LEFTSHIFT:  return "[SHIFT]"; break;
        case KEY_RIGHTSHIFT: return "[SHIFT]"; break;
        case KEY_COMMA:      return "[,<]"; break;
        case KEY_DOT:        return "[.>]"; break;
        case KEY_SLASH:      return "[/?]"; break;
        case KEY_BACKSLASH:  return "[\\|]"; break;
        case KEY_SPACE:      return "[SPACE]"; break;
        case KEY_CAPSLOCK:   return "[CAPS]"; break;
        case KEY_LEFTMETA:   return "[SUPER]"; break;
        case KEY_RIGHTMETA:  return "[SUPER]"; break;
        case KEY_RIGHTALT:   return "[ALT]"; break;
        case KEY_LEFTALT:    return "[ALT]"; break;
        case KEY_COMPOSE:    return "[COMPOSE]"; break;
        case KEY_INSERT:     return "[INSERT]"; break;
        case KEY_DELETE:     return "[DELETE]"; break;
        case KEY_HOME:       return "[HOME]"; break;
        case KEY_END:        return "[END]"; break;
        case KEY_PAGEUP:     return "[PAGEUP]"; break;
        case KEY_PAGEDOWN:   return "[PAGEDOWN]"; break;
        case KEY_UP:         return "[UP]"; break;
        case KEY_DOWN:       return "[DOWN]"; break;
        case KEY_LEFT:       return "[LEFT]"; break;
        case KEY_RIGHT:      return "[RIGHT]"; break;
        case KEY_SYSRQ:      return "[PRTSC]"; break;
        case KEY_SCROLLLOCK: return "[SCROLLLOCK]"; break;
        case KEY_PAUSE:      return "[PAUSE]"; break;
        case KEY_ESC:        return "[ESC]"; break;

        default: return "[UNKNOWN]"; break;
    }
}
#endif
