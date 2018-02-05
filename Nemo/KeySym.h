/*
 * The event structures are adapted from SDL2
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

#pragma once

#include <cstdint>
#include <type_traits>

namespace tc
{

enum class EScanCode
{
    Unknown = 0,

    /**
     *  \name Usage page 0x07
     *
     *  These values are from usage page 0x07 (USB keyboard page).
     */

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

    Return = 40,
    Escape = 41,
    Backspace = 42,
    Tab = 43,
    Space = 44,

    Minus = 45,
    Equals = 46,
    Leftbracket = 47,
    Rightbracket = 48,
    Backslash = 49, /**< Located at the lower left of the return
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
    Nonushash = 50, /**< ISO USB keyboards actually use this code
                                  *   instead of 49 for the same key, but all
                                  *   OSes I've seen treat the two codes
                                  *   identically. So, as an implementor, unless
                                  *   your keyboard generates both of those
                                  *   codes and your OS treats them differently,
                                  *   you should generate Backslash
                                  *   instead of this code. As a user, you
                                  *   should not rely on this code because SDL
                                  *   will never generate it with most (all?)
                                  *   keyboards.
                                  */
    Semicolon = 51,
    Apostrophe = 52,
    Grave = 53, /**< Located in the top left corner (on both ANSI
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
    Comma = 54,
    Period = 55,
    Slash = 56,

    Capslock = 57,

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

    Printscreen = 70,
    Scrolllock = 71,
    Pause = 72,
    Insert = 73, /**< insert on PC, help on some Mac keyboards (but
                                   does send code 73, not 117) */
    Home = 74,
    Pageup = 75,
    Delete = 76,
    End = 77,
    Pagedown = 78,
    Right = 79,
    Left = 80,
    Down = 81,
    Up = 82,

    Numlockclear = 83, /**< num lock on PC, clear on Mac keyboards
                                     */
    KpDivide = 84,
    KpMultiply = 85,
    KpMinus = 86,
    KpPlus = 87,
    KpEnter = 88,
    Kp_1 = 89,
    Kp_2 = 90,
    Kp_3 = 91,
    Kp_4 = 92,
    Kp_5 = 93,
    Kp_6 = 94,
    Kp_7 = 95,
    Kp_8 = 96,
    Kp_9 = 97,
    Kp_0 = 98,
    KpPeriod = 99,

    Nonusbackslash = 100, /**< This is the additional key that ISO
                                        *   keyboards have over ANSI ones,
                                        *   located between left shift and Y.
                                        *   Produces GRAVE ACCENT and TILDE in a
                                        *   US or UK Mac layout, REVERSE SOLIDUS
                                        *   (backslash) and VERTICAL LINE in a
                                        *   US or UK Windows layout, and
                                        *   LESS-THAN SIGN and GREATER-THAN SIGN
                                        *   in a Swiss German, German, or French
                                        *   layout. */
    Application = 101, /**< windows contextual menu, compose */
    Power = 102, /**< The USB document says this is a status flag,
                               *   not a physical key - but some Mac keyboards
                               *   do have a power key. */
    KpEquals = 103,
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
    Execute = 116,
    Help = 117,
    Menu = 118,
    Select = 119,
    Stop = 120,
    Again = 121,   /**< redo */
    Undo = 122,
    Cut = 123,
    Copy = 124,
    Paste = 125,
    Find = 126,
    Mute = 127,
    Volumeup = 128,
    Volumedown = 129,
    /* not sure whether there's a reason to enable these */
    /*     Lockingcapslock = 130,  */
    /*     Lockingnumlock = 131, */
    /*     Lockingscrolllock = 132, */
    KpComma = 133,
    KpEqualsas400 = 134,

    International1 = 135, /**< used on Asian keyboards, see
                                            footnotes in USB doc */
    International2 = 136,
    International3 = 137, /**< Yen */
    International4 = 138,
    International5 = 139,
    International6 = 140,
    International7 = 141,
    International8 = 142,
    International9 = 143,
    Lang1 = 144, /**< Hangul/English toggle */
    Lang2 = 145, /**< Hanja conversion */
    Lang3 = 146, /**< Katakana */
    Lang4 = 147, /**< Hiragana */
    Lang5 = 148, /**< Zenkaku/Hankaku */
    Lang6 = 149, /**< reserved */
    Lang7 = 150, /**< reserved */
    Lang8 = 151, /**< reserved */
    Lang9 = 152, /**< reserved */

    Alterase = 153, /**< Erase-Eaze */
    Sysreq = 154,
    Cancel = 155,
    Clear = 156,
    Prior = 157,
    Return2 = 158,
    Separator = 159,
    Out = 160,
    Oper = 161,
    Clearagain = 162,
    Crsel = 163,
    Exsel = 164,

    Kp_00 = 176,
    Kp_000 = 177,
    Thousandsseparator = 178,
    Decimalseparator = 179,
    Currencyunit = 180,
    Currencysubunit = 181,
    KpLeftparen = 182,
    KpRightparen = 183,
    KpLeftbrace = 184,
    KpRightbrace = 185,
    KpTab = 186,
    KpBackspace = 187,
    KpA = 188,
    KpB = 189,
    KpC = 190,
    KpD = 191,
    KpE = 192,
    KpF = 193,
    KpXor = 194,
    KpPower = 195,
    KpPercent = 196,
    KpLess = 197,
    KpGreater = 198,
    KpAmpersand = 199,
    KpDblampersand = 200,
    KpVerticalbar = 201,
    KpDblverticalbar = 202,
    KpColon = 203,
    KpHash = 204,
    KpSpace = 205,
    KpAt = 206,
    KpExclam = 207,
    KpMemstore = 208,
    KpMemrecall = 209,
    KpMemclear = 210,
    KpMemadd = 211,
    KpMemsubtract = 212,
    KpMemmultiply = 213,
    KpMemdivide = 214,
    KpPlusminus = 215,
    KpClear = 216,
    KpClearentry = 217,
    KpBinary = 218,
    KpOctal = 219,
    KpDecimal = 220,
    KpHexadecimal = 221,

    Lctrl = 224,
    Lshift = 225,
    Lalt = 226, /**< alt, option */
    Lgui = 227, /**< windows, command (apple), meta */
    Rctrl = 228,
    Rshift = 229,
    Ralt = 230, /**< alt gr, option */
    Rgui = 231, /**< windows, command (apple), meta */

    Mode = 257,    /**< I'm not sure if this is really not covered
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

    Audionext = 258,
    Audioprev = 259,
    Audiostop = 260,
    Audioplay = 261,
    Audiomute = 262,
    Mediaselect = 263,
    Www = 264,
    Mail = 265,
    Calculator = 266,
    Computer = 267,
    AcSearch = 268,
    AcHome = 269,
    AcBack = 270,
    AcForward = 271,
    AcStop = 272,
    AcRefresh = 273,
    AcBookmarks = 274,

    /* @} *//* Usage page 0x0C */

    /**
     *  \name Walther keys
     *
     *  These are values that Christian Walther added (for mac keyboard?).
     */
    /* @{ */

    Brightnessdown = 275,
    Brightnessup = 276,
    Displayswitch = 277, /**< display mirroring/dual display
                                           switch, video mode switch */
    Kbdillumtoggle = 278,
    Kbdillumdown = 279,
    Kbdillumup = 280,
    Eject = 281,
    Sleep = 282,

    App1 = 283,
    App2 = 284,

    /* @} *//* Walther keys */

    /**
     *  \name Usage page 0x0C (additional media keys)
     *
     *  These values are mapped from usage page 0x0C (USB consumer page).
     */
    /* @{ */

    Audiorewind = 285,
    Audiofastforward = 286,

    /* @} *//* Usage page 0x0C (additional media keys) */

    /* Add any other keys here. */

    NumScancodes = 512 /**< not a key, just marks the number of scancodes
                                 for array bounds */
};

typedef std::underlying_type<EScanCode>::type TScanCode;
#define ScanCodeToKeyCode(X) (static_cast<TScanCode>(X) | (1<<30))

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

    Capslock = ScanCodeToKeyCode(EScanCode::Capslock),

    F1 = ScanCodeToKeyCode(EScanCode::F1),
    F2 = ScanCodeToKeyCode(EScanCode::F2),
    F3 = ScanCodeToKeyCode(EScanCode::F3),
    F4 = ScanCodeToKeyCode(EScanCode::F4),
    F5 = ScanCodeToKeyCode(EScanCode::F5),
    F6 = ScanCodeToKeyCode(EScanCode::F6),
    F7 = ScanCodeToKeyCode(EScanCode::F7),
    F8 = ScanCodeToKeyCode(EScanCode::F8),
    F9 = ScanCodeToKeyCode(EScanCode::F9),
    F10 = ScanCodeToKeyCode(EScanCode::F10),
    F11 = ScanCodeToKeyCode(EScanCode::F11),
    F12 = ScanCodeToKeyCode(EScanCode::F12),

    Printscreen = ScanCodeToKeyCode(EScanCode::Printscreen),
    Scrolllock = ScanCodeToKeyCode(EScanCode::Scrolllock),
    Pause = ScanCodeToKeyCode(EScanCode::Pause),
    Insert = ScanCodeToKeyCode(EScanCode::Insert),
    Home = ScanCodeToKeyCode(EScanCode::Home),
    Pageup = ScanCodeToKeyCode(EScanCode::Pageup),
    Delete = '\177',
    End = ScanCodeToKeyCode(EScanCode::End),
    Pagedown = ScanCodeToKeyCode(EScanCode::Pagedown),
    Right = ScanCodeToKeyCode(EScanCode::Right),
    Left = ScanCodeToKeyCode(EScanCode::Left),
    Down = ScanCodeToKeyCode(EScanCode::Down),
    Up = ScanCodeToKeyCode(EScanCode::Up),

    Numlockclear = ScanCodeToKeyCode(EScanCode::Numlockclear),
    KpDivide = ScanCodeToKeyCode(EScanCode::KpDivide),
    KpMultiply = ScanCodeToKeyCode(EScanCode::KpMultiply),
    KpMinus = ScanCodeToKeyCode(EScanCode::KpMinus),
    KpPlus = ScanCodeToKeyCode(EScanCode::KpPlus),
    KpEnter = ScanCodeToKeyCode(EScanCode::KpEnter),
    Kp_1 = ScanCodeToKeyCode(EScanCode::Kp_1),
    Kp_2 = ScanCodeToKeyCode(EScanCode::Kp_2),
    Kp_3 = ScanCodeToKeyCode(EScanCode::Kp_3),
    Kp_4 = ScanCodeToKeyCode(EScanCode::Kp_4),
    Kp_5 = ScanCodeToKeyCode(EScanCode::Kp_5),
    Kp_6 = ScanCodeToKeyCode(EScanCode::Kp_6),
    Kp_7 = ScanCodeToKeyCode(EScanCode::Kp_7),
    Kp_8 = ScanCodeToKeyCode(EScanCode::Kp_8),
    Kp_9 = ScanCodeToKeyCode(EScanCode::Kp_9),
    Kp_0 = ScanCodeToKeyCode(EScanCode::Kp_0),
    KpPeriod = ScanCodeToKeyCode(EScanCode::KpPeriod),

    Application = ScanCodeToKeyCode(EScanCode::Application),
    Power = ScanCodeToKeyCode(EScanCode::Power),
    KpEquals = ScanCodeToKeyCode(EScanCode::KpEquals),
    F13 = ScanCodeToKeyCode(EScanCode::F13),
    F14 = ScanCodeToKeyCode(EScanCode::F14),
    F15 = ScanCodeToKeyCode(EScanCode::F15),
    F16 = ScanCodeToKeyCode(EScanCode::F16),
    F17 = ScanCodeToKeyCode(EScanCode::F17),
    F18 = ScanCodeToKeyCode(EScanCode::F18),
    F19 = ScanCodeToKeyCode(EScanCode::F19),
    F20 = ScanCodeToKeyCode(EScanCode::F20),
    F21 = ScanCodeToKeyCode(EScanCode::F21),
    F22 = ScanCodeToKeyCode(EScanCode::F22),
    F23 = ScanCodeToKeyCode(EScanCode::F23),
    F24 = ScanCodeToKeyCode(EScanCode::F24),
    Execute = ScanCodeToKeyCode(EScanCode::Execute),
    Help = ScanCodeToKeyCode(EScanCode::Help),
    Menu = ScanCodeToKeyCode(EScanCode::Menu),
    Select = ScanCodeToKeyCode(EScanCode::Select),
    Stop = ScanCodeToKeyCode(EScanCode::Stop),
    Again = ScanCodeToKeyCode(EScanCode::Again),
    Undo = ScanCodeToKeyCode(EScanCode::Undo),
    Cut = ScanCodeToKeyCode(EScanCode::Cut),
    Copy = ScanCodeToKeyCode(EScanCode::Copy),
    Paste = ScanCodeToKeyCode(EScanCode::Paste),
    Find = ScanCodeToKeyCode(EScanCode::Find),
    Mute = ScanCodeToKeyCode(EScanCode::Mute),
    Volumeup = ScanCodeToKeyCode(EScanCode::Volumeup),
    Volumedown = ScanCodeToKeyCode(EScanCode::Volumedown),
    KpComma = ScanCodeToKeyCode(EScanCode::KpComma),
    KpEqualsas400 = ScanCodeToKeyCode(EScanCode::KpEqualsas400),

    Alterase = ScanCodeToKeyCode(EScanCode::Alterase),
    Sysreq = ScanCodeToKeyCode(EScanCode::Sysreq),
    Cancel = ScanCodeToKeyCode(EScanCode::Cancel),
    Clear = ScanCodeToKeyCode(EScanCode::Clear),
    Prior = ScanCodeToKeyCode(EScanCode::Prior),
    Return2 = ScanCodeToKeyCode(EScanCode::Return2),
    Separator = ScanCodeToKeyCode(EScanCode::Separator),
    Out = ScanCodeToKeyCode(EScanCode::Out),
    Oper = ScanCodeToKeyCode(EScanCode::Oper),
    Clearagain = ScanCodeToKeyCode(EScanCode::Clearagain),
    Crsel = ScanCodeToKeyCode(EScanCode::Crsel),
    Exsel = ScanCodeToKeyCode(EScanCode::Exsel),

    Kp_00 = ScanCodeToKeyCode(EScanCode::Kp_00),
    Kp_000 = ScanCodeToKeyCode(EScanCode::Kp_000),
    Thousandsseparator = ScanCodeToKeyCode(EScanCode::Thousandsseparator),
    Decimalseparator = ScanCodeToKeyCode(EScanCode::Decimalseparator),
    Currencyunit = ScanCodeToKeyCode(EScanCode::Currencyunit),
    Currencysubunit = ScanCodeToKeyCode(EScanCode::Currencysubunit),
    KpLeftparen = ScanCodeToKeyCode(EScanCode::KpLeftparen),
    KpRightparen = ScanCodeToKeyCode(EScanCode::KpRightparen),
    KpLeftbrace = ScanCodeToKeyCode(EScanCode::KpLeftbrace),
    KpRightbrace = ScanCodeToKeyCode(EScanCode::KpRightbrace),
    KpTab = ScanCodeToKeyCode(EScanCode::KpTab),
    KpBackspace = ScanCodeToKeyCode(EScanCode::KpBackspace),
    KpA = ScanCodeToKeyCode(EScanCode::KpA),
    KpB = ScanCodeToKeyCode(EScanCode::KpB),
    KpC = ScanCodeToKeyCode(EScanCode::KpC),
    KpD = ScanCodeToKeyCode(EScanCode::KpD),
    KpE = ScanCodeToKeyCode(EScanCode::KpE),
    KpF = ScanCodeToKeyCode(EScanCode::KpF),
    KpXor = ScanCodeToKeyCode(EScanCode::KpXor),
    KpPower = ScanCodeToKeyCode(EScanCode::KpPower),
    KpPercent = ScanCodeToKeyCode(EScanCode::KpPercent),
    KpLess = ScanCodeToKeyCode(EScanCode::KpLess),
    KpGreater = ScanCodeToKeyCode(EScanCode::KpGreater),
    KpAmpersand = ScanCodeToKeyCode(EScanCode::KpAmpersand),
    KpDblampersand = ScanCodeToKeyCode(EScanCode::KpDblampersand),
    KpVerticalbar = ScanCodeToKeyCode(EScanCode::KpVerticalbar),
    KpDblverticalbar = ScanCodeToKeyCode(EScanCode::KpDblverticalbar),
    KpColon = ScanCodeToKeyCode(EScanCode::KpColon),
    KpHash = ScanCodeToKeyCode(EScanCode::KpHash),
    KpSpace = ScanCodeToKeyCode(EScanCode::KpSpace),
    KpAt = ScanCodeToKeyCode(EScanCode::KpAt),
    KpExclam = ScanCodeToKeyCode(EScanCode::KpExclam),
    KpMemstore = ScanCodeToKeyCode(EScanCode::KpMemstore),
    KpMemrecall = ScanCodeToKeyCode(EScanCode::KpMemrecall),
    KpMemclear = ScanCodeToKeyCode(EScanCode::KpMemclear),
    KpMemadd = ScanCodeToKeyCode(EScanCode::KpMemadd),
    KpMemsubtract = ScanCodeToKeyCode(EScanCode::KpMemsubtract),
    KpMemmultiply = ScanCodeToKeyCode(EScanCode::KpMemmultiply),
    KpMemdivide = ScanCodeToKeyCode(EScanCode::KpMemdivide),
    KpPlusminus = ScanCodeToKeyCode(EScanCode::KpPlusminus),
    KpClear = ScanCodeToKeyCode(EScanCode::KpClear),
    KpClearentry = ScanCodeToKeyCode(EScanCode::KpClearentry),
    KpBinary = ScanCodeToKeyCode(EScanCode::KpBinary),
    KpOctal = ScanCodeToKeyCode(EScanCode::KpOctal),
    KpDecimal = ScanCodeToKeyCode(EScanCode::KpDecimal),
    KpHexadecimal = ScanCodeToKeyCode(EScanCode::KpHexadecimal),

    Lctrl = ScanCodeToKeyCode(EScanCode::Lctrl),
    Lshift = ScanCodeToKeyCode(EScanCode::Lshift),
    Lalt = ScanCodeToKeyCode(EScanCode::Lalt),
    Lgui = ScanCodeToKeyCode(EScanCode::Lgui),
    Rctrl = ScanCodeToKeyCode(EScanCode::Rctrl),
    Rshift = ScanCodeToKeyCode(EScanCode::Rshift),
    Ralt = ScanCodeToKeyCode(EScanCode::Ralt),
    Rgui = ScanCodeToKeyCode(EScanCode::Rgui),

    Mode = ScanCodeToKeyCode(EScanCode::Mode),

    Audionext = ScanCodeToKeyCode(EScanCode::Audionext),
    Audioprev = ScanCodeToKeyCode(EScanCode::Audioprev),
    Audiostop = ScanCodeToKeyCode(EScanCode::Audiostop),
    Audioplay = ScanCodeToKeyCode(EScanCode::Audioplay),
    Audiomute = ScanCodeToKeyCode(EScanCode::Audiomute),
    Mediaselect = ScanCodeToKeyCode(EScanCode::Mediaselect),
    Www = ScanCodeToKeyCode(EScanCode::Www),
    Mail = ScanCodeToKeyCode(EScanCode::Mail),
    Calculator = ScanCodeToKeyCode(EScanCode::Calculator),
    Computer = ScanCodeToKeyCode(EScanCode::Computer),
    AcSearch = ScanCodeToKeyCode(EScanCode::AcSearch),
    AcHome = ScanCodeToKeyCode(EScanCode::AcHome),
    AcBack = ScanCodeToKeyCode(EScanCode::AcBack),
    AcForward = ScanCodeToKeyCode(EScanCode::AcForward),
    AcStop = ScanCodeToKeyCode(EScanCode::AcStop),
    AcRefresh = ScanCodeToKeyCode(EScanCode::AcRefresh),
    AcBookmarks = ScanCodeToKeyCode(EScanCode::AcBookmarks),

    Brightnessdown = ScanCodeToKeyCode(EScanCode::Brightnessdown),
    Brightnessup = ScanCodeToKeyCode(EScanCode::Brightnessup),
    Displayswitch = ScanCodeToKeyCode(EScanCode::Displayswitch),
    Kbdillumtoggle = ScanCodeToKeyCode(EScanCode::Kbdillumtoggle),
    Kbdillumdown = ScanCodeToKeyCode(EScanCode::Kbdillumdown),
    Kbdillumup = ScanCodeToKeyCode(EScanCode::Kbdillumup),
    Eject = ScanCodeToKeyCode(EScanCode::Eject),
    Sleep = ScanCodeToKeyCode(EScanCode::Sleep),
    App1 = ScanCodeToKeyCode(EScanCode::App1),
    App2 = ScanCodeToKeyCode(EScanCode::App2),

    Audiorewind = ScanCodeToKeyCode(EScanCode::Audiorewind),
    Audiofastforward = ScanCodeToKeyCode(EScanCode::Audiofastforward)
};

struct FKeySym
{
    EScanCode scancode;
    EKeyCode sym;
    uint16_t mod;
    //uint32_t unused;
};

enum EKeymod
{
    KMOD_NONE = 0x0000,
    KMOD_LSHIFT = 0x0001,
    KMOD_RSHIFT = 0x0002,
    KMOD_LCTRL = 0x0040,
    KMOD_RCTRL = 0x0080,
    KMOD_LALT = 0x0100,
    KMOD_RALT = 0x0200,
    KMOD_LGUI = 0x0400,
    KMOD_RGUI = 0x0800,
    KMOD_NUM = 0x1000,
    KMOD_CAPS = 0x2000,
    KMOD_MODE = 0x4000,
    KMOD_RESERVED = 0x8000
};

#define KMOD_CTRL   (KMOD_LCTRL|KMOD_RCTRL)
#define KMOD_SHIFT  (KMOD_LSHIFT|KMOD_RSHIFT)
#define KMOD_ALT    (KMOD_LALT|KMOD_RALT)
#define KMOD_GUI    (KMOD_LGUI|KMOD_RGUI)

} /* namespace tc */
