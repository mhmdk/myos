#ifndef _KEYBOARD_KEYCODES_H_
#define _KEYBOARD_KEYCODES_H_

// keycode is 8 bits: (3 bits row, 5 bits column)

#define ROW_0 0
#define ROW_1 (1<<5)
#define ROW_2 (2<<5)
#define ROW_3 (3<<5)
#define ROW_4 (4<<5)
#define ROW_5 (5<<5)
#define ROW_6 (6<<5)
#define ROW_7 (7<<5)

#define COL_0 0
#define COL_1 1
#define COL_2 2
#define COL_3 3
#define COL_4 4
#define COL_5 5
#define COL_6 6
#define COL_7 7
#define COL_8 8
#define COL_9 9
#define COL_10 10
#define COL_11 11
#define COL_12 12
#define COL_13 13
#define COL_14 14
#define COL_15 15
#define COL_16 16
#define COL_17 17
#define COL_18 18
#define COL_19 19
#define COL_20 20
#define COL_21 21
#define COL_22 22
#define COL_23 23

#define KEY_RESERVED 0xFF

#define KEY_ESCAPE (ROW_0 + COL_0)
#define KEY_F1 (ROW_0 + COL_1)
#define KEY_F2 (ROW_0+ COL_2)
#define KEY_F3 (ROW_0+ COL_3)
#define KEY_F4 (ROW_0+ COL_4)
#define KEY_F5 (ROW_0+ COL_5)
#define KEY_F6 (ROW_0+ COL_6)
#define KEY_F7 (ROW_0+ COL_7)
#define KEY_F8 (ROW_0+ COL_8)
#define KEY_F9 (ROW_0+ COL_9)
#define KEY_F10 (ROW_0+ COL_10)
#define KEY_F11 (ROW_0+ COL_11)
#define KEY_F12 (ROW_0+ COL_12)
#define KEY_PRINTSCREEN (ROW_0+ COL_13)
#define KEY_SCROLL_LOCK (ROW_0+ COL_14)
#define KEY_PAUSE (ROW_0+ COL_15)

#define KEY_BACKTICK (ROW_1 + COL_0)
#define KEY_1 (ROW_1+ COL_1)
#define KEY_2  (ROW_1+ COL_2)
#define KEY_3 (ROW_1+ COL_3)
#define KEY_4 (ROW_1+ COL_4)
#define KEY_5 (ROW_1+ COL_5)
#define KEY_6 (ROW_1+ COL_6)
#define KEY_7 (ROW_1+ COL_7)
#define KEY_8 (ROW_1+ COL_8)
#define KEY_9 (ROW_1+ COL_9)
#define KEY_0 (ROW_1+ COL_10)
#define KEY_DASH (ROW_1+ COL_11)
#define KEY_EQUALS (ROW_1+ COL_12)
#define KEY_BACKSPACE (ROW_1+ COL_13)
#define KEY_INSERT (ROW_1+ COL_14)
#define KEY_HOME (ROW_1+ COL_15)
#define KEY_PAGEUP (ROW_1+ COL_16)
#define KEY_NUMLOCK (ROW_1+ COL_17)
#define KEY_KEYPAD_SLASH (ROW_1+ COL_18)
#define KEY_KEYPAD_ASTERISK (ROW_1+ COL_19)
#define KEY_KEYPAD_DASH (ROW_1+ COL_20)

#define KEY_TAB ROW_2 +COL_0
#define KEY_Q ROW_2 +COL_1
#define KEY_W ROW_2 +COL_2
#define KEY_E ROW_2 +COL_3
#define KEY_R ROW_2 +COL_4
#define KEY_T ROW_2 +COL_5
#define KEY_Y ROW_2 +COL_6
#define KEY_U ROW_2 +COL_7
#define KEY_I ROW_2 +COL_8
#define KEY_O ROW_2 +COL_9
#define KEY_P ROW_2 +COL_10
#define KEY_OPEN_BRACKET ROW_2 +COL_11
#define KEY_CLOSE_BRACKET ROW_2 +COL_12
#define KEY_BACKSLASH ROW_2 +COL_13
#define KEY_DELETE ROW_2 +COL_14
#define KEY_END ROW_2 +COL_15
#define KEY_PAGEDOWN ROW_2 +COL_16
#define KEY_KEYPAD_7 ROW_2 +COL_17
#define KEY_KEYPAD_8 ROW_2 +COL_18
#define KEY_KEYPAD_9 ROW_2 +COL_19
#define KEY_KEYPAD_PLUS ROW_2 +COL_20

#define KEY_CAPSLOCK ROW_3+COL_0
#define KEY_A ROW_3+COL_1
#define KEY_S ROW_3+COL_2
#define KEY_D ROW_3+COL_3
#define KEY_F ROW_3+COL_4
#define KEY_G ROW_3+COL_5
#define KEY_H ROW_3+COL_6
#define KEY_J ROW_3+COL_7
#define KEY_K ROW_3+COL_8
#define KEY_L ROW_3+COL_9
#define KEY_SEMICOLON ROW_3+COL_10
#define KEY_SINGLE_QUOTE ROW_3+COL_11
#define KEY_ENTER ROW_3+COL_12
//SKIP 4  EMPTY COLUMNS
#define KEY_KEYPAD_4 ROW_3+COL_17
#define KEY_KEYPAD_5 ROW_3+COL_18
#define KEY_KEYPAD_6 ROW_3+COL_19
//LAST COLUMN IS KEYPAD PLUS, BUT BELONGS TO THE ROW ABOVE

#define KEY_LEFT_SHIFT ROW_4+COL_0
#define KEY_Z ROW_4+COL_1
#define KEY_X ROW_4+COL_2
#define KEY_C ROW_4+COL_3
#define KEY_V ROW_4+COL_4
#define KEY_B ROW_4+COL_5
#define KEY_N ROW_4+COL_6
#define KEY_M ROW_4+COL_7
#define KEY_COMMA ROW_4+COL_8
#define KEY_DOT ROW_4+COL_9
#define KEY_SLASH ROW_4+COL_10
#define KEY_RIGHT_SHIFT ROW_4+COL_11
//SKIP 3 COLUMNS (2 SHIFTS SPAN )
#define KEY_UP_ARROW ROW_4+COL_15
//SKIP 1 COLUMN
#define KEY_KEYPAD_1 ROW_4+COL_17
#define KEY_KEYPAD_2 ROW_4+COL_18
#define KEY_KEYPAD_3 ROW_4+COL_19
#define KEY_KEYPAD_ENTER ROW_4+COL_20

#define KEY_LEFT_CONTROL ROW_5+COL_0
#define KEY_LEFT_SUPER ROW_5+COL_1
#define KEY_LEFT_ALT ROW_5+COL_2
#define KEY_SPACE ROW_5+COL_3
#define KEY_RIGHT_ALT ROW_5+COL_4
#define KEY_RIGHT_SUPER ROW_5+COL_5
#define KEY_RIGHT_CONTROL ROW_5+COL_6
//SKIP  COLUMNS, TO ALIGN LEFT ARROW WITH WITH DELETE
#define KEY_LEFT_ARROW ROW_5+COL_14
#define KEY_DOWN_ARROW ROW_5+COL_15
#define KEY_RIGHT_ARROW ROW_5+COL_16
#define KEY_KEYPAD_0 ROW_5+COL_17
//SKIP 1COUMN, KEYPAD 0 SPANS 2 COLUMNS
#define KEY_KEYPAD_DOT ROW_5+COL_19
//LAST KEY IS ENTER , BUT BELONS TOROW ABOVE

#endif
