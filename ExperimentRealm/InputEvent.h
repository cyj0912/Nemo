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

#include "KeySym.h"

namespace tc
{

struct FKeyboardEvent
{
    uint32_t type;        /**< ::SDL_KEYDOWN or ::SDL_KEYUP */
    uint32_t timestamp;   /**< In milliseconds, populated using SDL_GetTicks() */
    uint32_t windowID;    /**< The window with keyboard focus, if any */
    uint8_t state;        /**< ::SDL_PRESSED or ::SDL_RELEASED */
    uint8_t repeat;       /**< Non-zero if this is a key repeat */
    uint8_t padding2;
    uint8_t padding3;
    FKeySym keysym;  /**< The key that was pressed or released */
};

struct FMouseMotionEvent
{
    uint32_t type;
    uint32_t timestamp;
    uint32_t windowID;
    uint32_t which;
    uint32_t state;
    int32_t x;
    int32_t y;
    int32_t xrel;
    int32_t yrel;
};

struct FMouseButtonEvent
{
    uint32_t type;
    uint32_t timestamp;
    uint32_t windowID;
    uint32_t which;
    uint8_t button;
    uint8_t state;
    uint8_t clicks;
    uint8_t padding1;
    int32_t x;
    int32_t y;
};

struct FMouseWheelEvent
{
    uint32_t type;
    uint32_t timestamp;
    uint32_t windowID;
    uint32_t which;
    int32_t x;
    int32_t y;
    uint32_t direction;
};

} /* namespace tc */
