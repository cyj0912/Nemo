#pragma once
#include <EnumClass.h>
#include <cstdint>

namespace tc
{

/*
  Key codes adapted from

  Simple DirectMedia Layer
  Copyright (C) 1997-2017 Sam Lantinga <slouken@libsdl.org>

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
*/

enum class EScanCode
{
    Unknown = 0,

    /**
     *  \name Usage page 0x07
     *
     *  These values are from usage page 0x07 (USB keyboard page).
     */
    /* @{ */

    A = 4,
    B = 5,
    C = 6,
    D = 7,
    E = 8,
    F = 9,
    G = 10,
    H = 11,
    I = 12,
    J = 13,
    K = 14,
    L = 15,
    M = 16,
    N = 17,
    O = 18,
    P = 19,
    Q = 20,
    R = 21,
    S = 22,
    T = 23,
    U = 24,
    V = 25,
    W = 26,
    X = 27,
    Y = 28,
    Z = 29,

    _1 = 30,
    _2 = 31,
    _3 = 32,
    _4 = 33,
    _5 = 34,
    _6 = 35,
    _7 = 36,
    _8 = 37,
    _9 = 38,
    _0 = 39,

    RETURN = 40,
    ESCAPE = 41,
    BACKSPACE = 42,
    TAB = 43,
    SPACE = 44,

    MINUS = 45,
    EQUALS = 46,
    LEFTBRACKET = 47,
    RIGHTBRACKET = 48,
    BACKSLASH = 49, /**< Located at the lower left of the return
                                  *   key on ISO keyboards and at the right end
                                  *   of the QWERTY row on ANSI keyboards.
                                  *   Produces REVERSE SOLIDUS (backslash) and
                                  *   VERTICAL LINE in a US layout, REVERSE
                                  *   SOLIDUS and VERTICAL LINE in a UK Mac
                                  *   layout, NUMBER SIGN and TILDE in a UK
                                  *   Windows layout, DOLLAR SIGN and POUND SIGN
                                  *   in a Swiss German layout, NUMBER SIGN and
                                  *   APOSTROPHE in a German layout, GRAVE
                                  *   ACCENT and POUND SIGN in a French Mac
                                  *   layout, and ASTERISK and MICRO SIGN in a
                                  *   French Windows layout.
                                  */
    NONUSHASH = 50, /**< ISO USB keyboards actually use this code
                                  *   instead of 49 for the same key, but all
                                  *   OSes I've seen treat the two codes
                                  *   identically. So, as an implementor, unless
                                  *   your keyboard generates both of those
                                  *   codes and your OS treats them differently,
                                  *   you should generate BACKSLASH
                                  *   instead of this code. As a user, you
                                  *   should not rely on this code because SDL
                                  *   will never generate it with most (all?)
                                  *   keyboards.
                                  */
    SEMICOLON = 51,
    APOSTROPHE = 52,
    GRAVE = 53, /**< Located in the top left corner (on both ANSI
                              *   and ISO keyboards). Produces GRAVE ACCENT and
                              *   TILDE in a US Windows layout and in US and UK
                              *   Mac layouts on ANSI keyboards, GRAVE ACCENT
                              *   and NOT SIGN in a UK Windows layout, SECTION
                              *   SIGN and PLUS-MINUS SIGN in US and UK Mac
                              *   layouts on ISO keyboards, SECTION SIGN and
                              *   DEGREE SIGN in a Swiss German layout (Mac:
                              *   only on ISO keyboards), CIRCUMFLEX ACCENT and
                              *   DEGREE SIGN in a German layout (Mac: only on
                              *   ISO keyboards), SUPERSCRIPT TWO and TILDE in a
                              *   French Windows layout, COMMERCIAL AT and
                              *   NUMBER SIGN in a French Mac layout on ISO
                              *   keyboards, and LESS-THAN SIGN and GREATER-THAN
                              *   SIGN in a Swiss German, German, or French Mac
                              *   layout on ANSI keyboards.
                              */
    COMMA = 54,
    PERIOD = 55,
    SLASH = 56,

    CAPSLOCK = 57,

    F1 = 58,
    F2 = 59,
    F3 = 60,
    F4 = 61,
    F5 = 62,
    F6 = 63,
    F7 = 64,
    F8 = 65,
    F9 = 66,
    F10 = 67,
    F11 = 68,
    F12 = 69,

    PRINTSCREEN = 70,
    SCROLLLOCK = 71,
    PAUSE = 72,
    INSERT = 73, /**< insert on PC, help on some Mac keyboards (but
                                   does send code 73, not 117) */
    HOME = 74,
    PAGEUP = 75,
    Delete = 76,
    END = 77,
    PAGEDOWN = 78,
    RIGHT = 79,
    LEFT = 80,
    DOWN = 81,
    UP = 82,

    NUMLOCKCLEAR = 83, /**< num lock on PC, clear on Mac keyboards
                                     */
    KP_DIVIDE = 84,
    KP_MULTIPLY = 85,
    KP_MINUS = 86,
    KP_PLUS = 87,
    KP_ENTER = 88,
    KP_1 = 89,
    KP_2 = 90,
    KP_3 = 91,
    KP_4 = 92,
    KP_5 = 93,
    KP_6 = 94,
    KP_7 = 95,
    KP_8 = 96,
    KP_9 = 97,
    KP_0 = 98,
    KP_PERIOD = 99,

    NONUSBACKSLASH = 100, /**< This is the additional key that ISO
                                        *   keyboards have over ANSI ones,
                                        *   located between left shift and Y.
                                        *   Produces GRAVE ACCENT and TILDE in a
                                        *   US or UK Mac layout, REVERSE SOLIDUS
                                        *   (backslash) and VERTICAL LINE in a
                                        *   US or UK Windows layout, and
                                        *   LESS-THAN SIGN and GREATER-THAN SIGN
                                        *   in a Swiss German, German, or French
                                        *   layout. */
    APPLICATION = 101, /**< windows contextual menu, compose */
    POWER = 102, /**< The USB document says this is a status flag,
                               *   not a physical key - but some Mac keyboards
                               *   do have a power key. */
    KP_EQUALS = 103,
    F13 = 104,
    F14 = 105,
    F15 = 106,
    F16 = 107,
    F17 = 108,
    F18 = 109,
    F19 = 110,
    F20 = 111,
    F21 = 112,
    F22 = 113,
    F23 = 114,
    F24 = 115,
    EXECUTE = 116,
    HELP = 117,
    MENU = 118,
    SELECT = 119,
    STOP = 120,
    AGAIN = 121,   /**< redo */
    UNDO = 122,
    CUT = 123,
    COPY = 124,
    PASTE = 125,
    FIND = 126,
    MUTE = 127,
    VOLUMEUP = 128,
    VOLUMEDOWN = 129,
/* not sure whether there's a reason to enable these */
/*     LOCKINGCAPSLOCK = 130,  */
/*     LOCKINGNUMLOCK = 131, */
/*     LOCKINGSCROLLLOCK = 132, */
    KP_COMMA = 133,
    KP_EQUALSAS400 = 134,

    INTERNATIONAL1 = 135, /**< used on Asian keyboards, see
                                            footnotes in USB doc */
    INTERNATIONAL2 = 136,
    INTERNATIONAL3 = 137, /**< Yen */
    INTERNATIONAL4 = 138,
    INTERNATIONAL5 = 139,
    INTERNATIONAL6 = 140,
    INTERNATIONAL7 = 141,
    INTERNATIONAL8 = 142,
    INTERNATIONAL9 = 143,
    LANG1 = 144, /**< Hangul/English toggle */
    LANG2 = 145, /**< Hanja conversion */
    LANG3 = 146, /**< Katakana */
    LANG4 = 147, /**< Hiragana */
    LANG5 = 148, /**< Zenkaku/Hankaku */
    LANG6 = 149, /**< reserved */
    LANG7 = 150, /**< reserved */
    LANG8 = 151, /**< reserved */
    LANG9 = 152, /**< reserved */

    ALTERASE = 153, /**< Erase-Eaze */
    SYSREQ = 154,
    CANCEL = 155,
    CLEAR = 156,
    PRIOR = 157,
    RETURN2 = 158,
    SEPARATOR = 159,
    Out = 160,
    OPER = 161,
    CLEARAGAIN = 162,
    CRSEL = 163,
    EXSEL = 164,

    KP_00 = 176,
    KP_000 = 177,
    THOUSANDSSEPARATOR = 178,
    DECIMALSEPARATOR = 179,
    CURRENCYUNIT = 180,
    CURRENCYSUBUNIT = 181,
    KP_LEFTPAREN = 182,
    KP_RIGHTPAREN = 183,
    KP_LEFTBRACE = 184,
    KP_RIGHTBRACE = 185,
    KP_TAB = 186,
    KP_BACKSPACE = 187,
    KP_A = 188,
    KP_B = 189,
    KP_C = 190,
    KP_D = 191,
    KP_E = 192,
    KP_F = 193,
    KP_XOR = 194,
    KP_POWER = 195,
    KP_PERCENT = 196,
    KP_LESS = 197,
    KP_GREATER = 198,
    KP_AMPERSAND = 199,
    KP_DBLAMPERSAND = 200,
    KP_VERTICALBAR = 201,
    KP_DBLVERTICALBAR = 202,
    KP_COLON = 203,
    KP_HASH = 204,
    KP_SPACE = 205,
    KP_AT = 206,
    KP_EXCLAM = 207,
    KP_MEMSTORE = 208,
    KP_MEMRECALL = 209,
    KP_MEMCLEAR = 210,
    KP_MEMADD = 211,
    KP_MEMSUBTRACT = 212,
    KP_MEMMULTIPLY = 213,
    KP_MEMDIVIDE = 214,
    KP_PLUSMINUS = 215,
    KP_CLEAR = 216,
    KP_CLEARENTRY = 217,
    KP_BINARY = 218,
    KP_OCTAL = 219,
    KP_DECIMAL = 220,
    KP_HEXADECIMAL = 221,

    LCTRL = 224,
    LSHIFT = 225,
    LALT = 226, /**< alt, option */
    LGUI = 227, /**< windows, command (apple), meta */
    RCTRL = 228,
    RSHIFT = 229,
    RALT = 230, /**< alt gr, option */
    RGUI = 231, /**< windows, command (apple), meta */

    MODE = 257,    /**< I'm not sure if this is really not covered
                                 *   by any of the above, but since there's a
                                 *   special KMOD_MODE for it I'm adding it here
                                 */

    /* @} *//* Usage page 0x07 */

    /**
     *  \name Usage page 0x0C
     *
     *  These values are mapped from usage page 0x0C (USB consumer page).
     */
    /* @{ */

    AUDIONEXT = 258,
    AUDIOPREV = 259,
    AUDIOSTOP = 260,
    AUDIOPLAY = 261,
    AUDIOMUTE = 262,
    MEDIASELECT = 263,
    WWW = 264,
    MAIL = 265,
    CALCULATOR = 266,
    COMPUTER = 267,
    AC_SEARCH = 268,
    AC_HOME = 269,
    AC_BACK = 270,
    AC_FORWARD = 271,
    AC_STOP = 272,
    AC_REFRESH = 273,
    AC_BOOKMARKS = 274,

    /* @} *//* Usage page 0x0C */

    /**
     *  \name Walther keys
     *
     *  These are values that Christian Walther added (for mac keyboard?).
     */
    /* @{ */

    BRIGHTNESSDOWN = 275,
    BRIGHTNESSUP = 276,
    DISPLAYSWITCH = 277, /**< display mirroring/dual display
                                           switch, video mode switch */
    KBDILLUMTOGGLE = 278,
    KBDILLUMDOWN = 279,
    KBDILLUMUP = 280,
    EJECT = 281,
    SLEEP = 282,

    APP1 = 283,
    APP2 = 284,

    /* @} *//* Walther keys */

    /**
     *  \name Usage page 0x0C (additional media keys)
     *
     *  These values are mapped from usage page 0x0C (USB consumer page).
     */
    /* @{ */

    AUDIOREWIND = 285,
    AUDIOFASTFORWARD = 286,

    /* @} *//* Usage page 0x0C (additional media keys) */

    /* Add any other keys here. */

    NUM_SCANCODES = 512 /**< not a key, just marks the number of scancodes
                                 for array bounds */
};

#define SCANCODE_MASK (1<<30)
#define SCANCODE_TO_KEYCODE(X) ((int)X | SCANCODE_MASK)

enum class EKeyCode
{
    Unknown = 0,

    Return = '\r',
    Escape = '\033',
    Backspace = '\b',
    Tab = '\t',
    Space = ' ',
    Exclaim = '!',
    Quotedbl = '"',
    Hash = '#',
    Percent = '%',
    Dollar = '$',
    Ampersand = '&',
    Quote = '\'',
    Leftparen = '(',
    Rightparen = ')',
    Asterisk = '*',
    Plus = '+',
    Comma = ',',
    Minus = '-',
    Period = '.',
    Slash = '/',
    _0 = '0',
    _1 = '1',
    _2 = '2',
    _3 = '3',
    _4 = '4',
    _5 = '5',
    _6 = '6',
    _7 = '7',
    _8 = '8',
    _9 = '9',
    Colon = ':',
    Semicolon = ';',
    Less = '<',
    Equals = '=',
    Greater = '>',
    Question = '?',
    At = '@',
    /*
           Skip uppercase letters
         */
    Leftbracket = '[',
    Backslash = '\\',
    Rightbracket = ']',
    Caret = '^',
    Underscore = '_',
    Backquote = '`',
    A = 'a',
    B = 'b',
    C = 'c',
    D = 'd',
    E = 'e',
    F = 'f',
    G = 'g',
    H = 'h',
    I = 'i',
    J = 'j',
    K = 'k',
    L = 'l',
    M = 'm',
    N = 'n',
    O = 'o',
    P = 'p',
    Q = 'q',
    R = 'r',
    S = 's',
    T = 't',
    U = 'u',
    V = 'v',
    W = 'w',
    X = 'x',
    Y = 'y',
    Z = 'z',

    Capslock = SCANCODE_TO_KEYCODE(EScanCode::CAPSLOCK),

    F1 = SCANCODE_TO_KEYCODE(EScanCode::F1),
    F2 = SCANCODE_TO_KEYCODE(EScanCode::F2),
    F3 = SCANCODE_TO_KEYCODE(EScanCode::F3),
    F4 = SCANCODE_TO_KEYCODE(EScanCode::F4),
    F5 = SCANCODE_TO_KEYCODE(EScanCode::F5),
    F6 = SCANCODE_TO_KEYCODE(EScanCode::F6),
    F7 = SCANCODE_TO_KEYCODE(EScanCode::F7),
    F8 = SCANCODE_TO_KEYCODE(EScanCode::F8),
    F9 = SCANCODE_TO_KEYCODE(EScanCode::F9),
    F10 = SCANCODE_TO_KEYCODE(EScanCode::F10),
    F11 = SCANCODE_TO_KEYCODE(EScanCode::F11),
    F12 = SCANCODE_TO_KEYCODE(EScanCode::F12),

    Printscreen = SCANCODE_TO_KEYCODE(EScanCode::PRINTSCREEN),
    Scrolllock = SCANCODE_TO_KEYCODE(EScanCode::SCROLLLOCK),
    Pause = SCANCODE_TO_KEYCODE(EScanCode::PAUSE),
    Insert = SCANCODE_TO_KEYCODE(EScanCode::INSERT),
    Home = SCANCODE_TO_KEYCODE(EScanCode::HOME),
    Pageup = SCANCODE_TO_KEYCODE(EScanCode::PAGEUP),
    Delete = '\177',
    End = SCANCODE_TO_KEYCODE(EScanCode::END),
    Pagedown = SCANCODE_TO_KEYCODE(EScanCode::PAGEDOWN),
    Right = SCANCODE_TO_KEYCODE(EScanCode::RIGHT),
    Left = SCANCODE_TO_KEYCODE(EScanCode::LEFT),
    Down = SCANCODE_TO_KEYCODE(EScanCode::DOWN),
    Up = SCANCODE_TO_KEYCODE(EScanCode::UP),

    Numlockclear = SCANCODE_TO_KEYCODE(EScanCode::NUMLOCKCLEAR),
    KpDivide = SCANCODE_TO_KEYCODE(EScanCode::KP_DIVIDE),
    KpMultiply = SCANCODE_TO_KEYCODE(EScanCode::KP_MULTIPLY),
    KpMinus = SCANCODE_TO_KEYCODE(EScanCode::KP_MINUS),
    KpPlus = SCANCODE_TO_KEYCODE(EScanCode::KP_PLUS),
    KpEnter = SCANCODE_TO_KEYCODE(EScanCode::KP_ENTER),
    Kp_1 = SCANCODE_TO_KEYCODE(EScanCode::KP_1),
    Kp_2 = SCANCODE_TO_KEYCODE(EScanCode::KP_2),
    Kp_3 = SCANCODE_TO_KEYCODE(EScanCode::KP_3),
    Kp_4 = SCANCODE_TO_KEYCODE(EScanCode::KP_4),
    Kp_5 = SCANCODE_TO_KEYCODE(EScanCode::KP_5),
    Kp_6 = SCANCODE_TO_KEYCODE(EScanCode::KP_6),
    Kp_7 = SCANCODE_TO_KEYCODE(EScanCode::KP_7),
    Kp_8 = SCANCODE_TO_KEYCODE(EScanCode::KP_8),
    Kp_9 = SCANCODE_TO_KEYCODE(EScanCode::KP_9),
    Kp_0 = SCANCODE_TO_KEYCODE(EScanCode::KP_0),
    KpPeriod = SCANCODE_TO_KEYCODE(EScanCode::KP_PERIOD),

    Application = SCANCODE_TO_KEYCODE(EScanCode::APPLICATION),
    Power = SCANCODE_TO_KEYCODE(EScanCode::POWER),
    KpEquals = SCANCODE_TO_KEYCODE(EScanCode::KP_EQUALS),
    F13 = SCANCODE_TO_KEYCODE(EScanCode::F13),
    F14 = SCANCODE_TO_KEYCODE(EScanCode::F14),
    F15 = SCANCODE_TO_KEYCODE(EScanCode::F15),
    F16 = SCANCODE_TO_KEYCODE(EScanCode::F16),
    F17 = SCANCODE_TO_KEYCODE(EScanCode::F17),
    F18 = SCANCODE_TO_KEYCODE(EScanCode::F18),
    F19 = SCANCODE_TO_KEYCODE(EScanCode::F19),
    F20 = SCANCODE_TO_KEYCODE(EScanCode::F20),
    F21 = SCANCODE_TO_KEYCODE(EScanCode::F21),
    F22 = SCANCODE_TO_KEYCODE(EScanCode::F22),
    F23 = SCANCODE_TO_KEYCODE(EScanCode::F23),
    F24 = SCANCODE_TO_KEYCODE(EScanCode::F24),
    Execute = SCANCODE_TO_KEYCODE(EScanCode::EXECUTE),
    Help = SCANCODE_TO_KEYCODE(EScanCode::HELP),
    Menu = SCANCODE_TO_KEYCODE(EScanCode::MENU),
    Select = SCANCODE_TO_KEYCODE(EScanCode::SELECT),
    Stop = SCANCODE_TO_KEYCODE(EScanCode::STOP),
    Again = SCANCODE_TO_KEYCODE(EScanCode::AGAIN),
    Undo = SCANCODE_TO_KEYCODE(EScanCode::UNDO),
    Cut = SCANCODE_TO_KEYCODE(EScanCode::CUT),
    Copy = SCANCODE_TO_KEYCODE(EScanCode::COPY),
    Paste = SCANCODE_TO_KEYCODE(EScanCode::PASTE),
    Find = SCANCODE_TO_KEYCODE(EScanCode::FIND),
    Mute = SCANCODE_TO_KEYCODE(EScanCode::MUTE),
    Volumeup = SCANCODE_TO_KEYCODE(EScanCode::VOLUMEUP),
    Volumedown = SCANCODE_TO_KEYCODE(EScanCode::VOLUMEDOWN),
    KpComma = SCANCODE_TO_KEYCODE(EScanCode::KP_COMMA),
    KpEqualsas400 =
    SCANCODE_TO_KEYCODE(EScanCode::KP_EQUALSAS400),

    Alterase = SCANCODE_TO_KEYCODE(EScanCode::ALTERASE),
    Sysreq = SCANCODE_TO_KEYCODE(EScanCode::SYSREQ),
    Cancel = SCANCODE_TO_KEYCODE(EScanCode::CANCEL),
    Clear = SCANCODE_TO_KEYCODE(EScanCode::CLEAR),
    Prior = SCANCODE_TO_KEYCODE(EScanCode::PRIOR),
    Return2 = SCANCODE_TO_KEYCODE(EScanCode::RETURN2),
    Separator = SCANCODE_TO_KEYCODE(EScanCode::SEPARATOR),
    Out = SCANCODE_TO_KEYCODE(EScanCode::Out),
    Oper = SCANCODE_TO_KEYCODE(EScanCode::OPER),
    Clearagain = SCANCODE_TO_KEYCODE(EScanCode::CLEARAGAIN),
    Crsel = SCANCODE_TO_KEYCODE(EScanCode::CRSEL),
    Exsel = SCANCODE_TO_KEYCODE(EScanCode::EXSEL),

    Kp_00 = SCANCODE_TO_KEYCODE(EScanCode::KP_00),
    Kp_000 = SCANCODE_TO_KEYCODE(EScanCode::KP_000),
    Thousandsseparator =
    SCANCODE_TO_KEYCODE(EScanCode::THOUSANDSSEPARATOR),
    Decimalseparator =
    SCANCODE_TO_KEYCODE(EScanCode::DECIMALSEPARATOR),
    Currencyunit = SCANCODE_TO_KEYCODE(EScanCode::CURRENCYUNIT),
    Currencysubunit =
    SCANCODE_TO_KEYCODE(EScanCode::CURRENCYSUBUNIT),
    KpLeftparen = SCANCODE_TO_KEYCODE(EScanCode::KP_LEFTPAREN),
    KpRightparen = SCANCODE_TO_KEYCODE(EScanCode::KP_RIGHTPAREN),
    KpLeftbrace = SCANCODE_TO_KEYCODE(EScanCode::KP_LEFTBRACE),
    KpRightbrace = SCANCODE_TO_KEYCODE(EScanCode::KP_RIGHTBRACE),
    KpTab = SCANCODE_TO_KEYCODE(EScanCode::KP_TAB),
    KpBackspace = SCANCODE_TO_KEYCODE(EScanCode::KP_BACKSPACE),
    KpA = SCANCODE_TO_KEYCODE(EScanCode::KP_A),
    KpB = SCANCODE_TO_KEYCODE(EScanCode::KP_B),
    KpC = SCANCODE_TO_KEYCODE(EScanCode::KP_C),
    KpD = SCANCODE_TO_KEYCODE(EScanCode::KP_D),
    KpE = SCANCODE_TO_KEYCODE(EScanCode::KP_E),
    KpF = SCANCODE_TO_KEYCODE(EScanCode::KP_F),
    KpXor = SCANCODE_TO_KEYCODE(EScanCode::KP_XOR),
    KpPower = SCANCODE_TO_KEYCODE(EScanCode::KP_POWER),
    KpPercent = SCANCODE_TO_KEYCODE(EScanCode::KP_PERCENT),
    KpLess = SCANCODE_TO_KEYCODE(EScanCode::KP_LESS),
    KpGreater = SCANCODE_TO_KEYCODE(EScanCode::KP_GREATER),
    KpAmpersand = SCANCODE_TO_KEYCODE(EScanCode::KP_AMPERSAND),
    KpDblampersand =
    SCANCODE_TO_KEYCODE(EScanCode::KP_DBLAMPERSAND),
    KpVerticalbar =
    SCANCODE_TO_KEYCODE(EScanCode::KP_VERTICALBAR),
    KpDblverticalbar =
    SCANCODE_TO_KEYCODE(EScanCode::KP_DBLVERTICALBAR),
    KpColon = SCANCODE_TO_KEYCODE(EScanCode::KP_COLON),
    KpHash = SCANCODE_TO_KEYCODE(EScanCode::KP_HASH),
    KpSpace = SCANCODE_TO_KEYCODE(EScanCode::KP_SPACE),
    KpAt = SCANCODE_TO_KEYCODE(EScanCode::KP_AT),
    KpExclam = SCANCODE_TO_KEYCODE(EScanCode::KP_EXCLAM),
    KpMemstore = SCANCODE_TO_KEYCODE(EScanCode::KP_MEMSTORE),
    KpMemrecall = SCANCODE_TO_KEYCODE(EScanCode::KP_MEMRECALL),
    KpMemclear = SCANCODE_TO_KEYCODE(EScanCode::KP_MEMCLEAR),
    KpMemadd = SCANCODE_TO_KEYCODE(EScanCode::KP_MEMADD),
    KpMemsubtract =
    SCANCODE_TO_KEYCODE(EScanCode::KP_MEMSUBTRACT),
    KpMemmultiply =
    SCANCODE_TO_KEYCODE(EScanCode::KP_MEMMULTIPLY),
    KpMemdivide = SCANCODE_TO_KEYCODE(EScanCode::KP_MEMDIVIDE),
    KpPlusminus = SCANCODE_TO_KEYCODE(EScanCode::KP_PLUSMINUS),
    KpClear = SCANCODE_TO_KEYCODE(EScanCode::KP_CLEAR),
    KpClearentry = SCANCODE_TO_KEYCODE(EScanCode::KP_CLEARENTRY),
    KpBinary = SCANCODE_TO_KEYCODE(EScanCode::KP_BINARY),
    KpOctal = SCANCODE_TO_KEYCODE(EScanCode::KP_OCTAL),
    KpDecimal = SCANCODE_TO_KEYCODE(EScanCode::KP_DECIMAL),
    KpHexadecimal =
    SCANCODE_TO_KEYCODE(EScanCode::KP_HEXADECIMAL),

    Lctrl = SCANCODE_TO_KEYCODE(EScanCode::LCTRL),
    Lshift = SCANCODE_TO_KEYCODE(EScanCode::LSHIFT),
    Lalt = SCANCODE_TO_KEYCODE(EScanCode::LALT),
    Lgui = SCANCODE_TO_KEYCODE(EScanCode::LGUI),
    Rctrl = SCANCODE_TO_KEYCODE(EScanCode::RCTRL),
    Rshift = SCANCODE_TO_KEYCODE(EScanCode::RSHIFT),
    Ralt = SCANCODE_TO_KEYCODE(EScanCode::RALT),
    Rgui = SCANCODE_TO_KEYCODE(EScanCode::RGUI),

    Mode = SCANCODE_TO_KEYCODE(EScanCode::MODE),

    Audionext = SCANCODE_TO_KEYCODE(EScanCode::AUDIONEXT),
    Audioprev = SCANCODE_TO_KEYCODE(EScanCode::AUDIOPREV),
    Audiostop = SCANCODE_TO_KEYCODE(EScanCode::AUDIOSTOP),
    Audioplay = SCANCODE_TO_KEYCODE(EScanCode::AUDIOPLAY),
    Audiomute = SCANCODE_TO_KEYCODE(EScanCode::AUDIOMUTE),
    Mediaselect = SCANCODE_TO_KEYCODE(EScanCode::MEDIASELECT),
    Www = SCANCODE_TO_KEYCODE(EScanCode::WWW),
    Mail = SCANCODE_TO_KEYCODE(EScanCode::MAIL),
    Calculator = SCANCODE_TO_KEYCODE(EScanCode::CALCULATOR),
    Computer = SCANCODE_TO_KEYCODE(EScanCode::COMPUTER),
    AcSearch = SCANCODE_TO_KEYCODE(EScanCode::AC_SEARCH),
    AcHome = SCANCODE_TO_KEYCODE(EScanCode::AC_HOME),
    AcBack = SCANCODE_TO_KEYCODE(EScanCode::AC_BACK),
    AcForward = SCANCODE_TO_KEYCODE(EScanCode::AC_FORWARD),
    AcStop = SCANCODE_TO_KEYCODE(EScanCode::AC_STOP),
    AcRefresh = SCANCODE_TO_KEYCODE(EScanCode::AC_REFRESH),
    AcBookmarks = SCANCODE_TO_KEYCODE(EScanCode::AC_BOOKMARKS),

    Brightnessdown =
    SCANCODE_TO_KEYCODE(EScanCode::BRIGHTNESSDOWN),
    Brightnessup = SCANCODE_TO_KEYCODE(EScanCode::BRIGHTNESSUP),
    Displayswitch = SCANCODE_TO_KEYCODE(EScanCode::DISPLAYSWITCH),
    Kbdillumtoggle =
    SCANCODE_TO_KEYCODE(EScanCode::KBDILLUMTOGGLE),
    Kbdillumdown = SCANCODE_TO_KEYCODE(EScanCode::KBDILLUMDOWN),
    Kbdillumup = SCANCODE_TO_KEYCODE(EScanCode::KBDILLUMUP),
    Eject = SCANCODE_TO_KEYCODE(EScanCode::EJECT),
    Sleep = SCANCODE_TO_KEYCODE(EScanCode::SLEEP),
    App1 = SCANCODE_TO_KEYCODE(EScanCode::APP1),
    App2 = SCANCODE_TO_KEYCODE(EScanCode::APP2),

    Audiorewind = SCANCODE_TO_KEYCODE(EScanCode::AUDIOREWIND),
    Audiofastforward = SCANCODE_TO_KEYCODE(EScanCode::AUDIOFASTFORWARD)
};

enum class EKeyMod
{
    None = 0x0000,
    LShift = 0x0001,
    RShift = 0x0002,
    LCtrl = 0x0040,
    RCtrl = 0x0080,
    LAlt = 0x0100,
    RAlt = 0x0200,
    LGui = 0x0400,
    RGui = 0x0800,
    Num = 0x1000,
    Caps = 0x2000,
    Mode = 0x4000,
    Reserved = 0x8000,

    Ctrl = LCtrl | RCtrl,
    Shift = LShift | RShift,
    Alt = LAlt | RAlt,
    Gui = LGui | RGui
};

enum class EInputEventType : uint32_t
{
    MouseMotion,
    MouseButton,
    MouseWheel,
    Keybodard
};

enum class EKeyState : uint8_t
{
    Pressed,
    Released
};

struct FMouseMotionEvent
{
    EInputEventType Type;
    uint32_t TimeStamp;

    uint32_t WindowID;
    uint32_t Which;
    int32_t x;
    int32_t y;
    int32_t xRel;
    int32_t yRel;
};

struct FMouseButtonEvent
{
    EInputEventType Type;
    uint32_t TimeStamp;

    uint32_t WindowID;
    uint32_t Which;
    uint8_t ButtonIndex;
    EKeyState State;
    uint8_t Clicks;
    uint8_t Padding1;
    int32_t x;
    int32_t y;
};

struct FMouseWheelEvent
{
    EInputEventType Type;
    uint32_t TimeStamp;

    uint32_t WindowID;
    uint32_t Which;
    int32_t x;
    int32_t y;
    uint32_t Direction;
};

struct FKeyboardInputEvent
{
    EInputEventType Type;
    uint32_t TimeStamp;

    EKeyState State;
    EKeyMod KeyMod;
    EKeyCode KeyCode;
};

typedef union FInputEvent
{
    EInputEventType Type;
    FMouseMotionEvent MouseMotion;
    FMouseButtonEvent MouseButton;
    FMouseWheelEvent MouseWheel;
    FKeyboardInputEvent Keyboard;
} FInputEvent;

}
