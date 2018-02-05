#pragma once

#include "InputEvent.h"

#if TC_OS == TC_OS_WINDOWS_NT
#include <SDL2/SDL_events.h>
#else
#include <SDL.h>
#endif

namespace tc
{

FKeyboardEvent ConvertKeyboardEvent(const SDL_KeyboardEvent& e)
{
    FKeyboardEvent result;
    result.type = e.type;
    result.timestamp = e.timestamp;
    result.windowID = e.windowID;
    result.state = e.state;
    result.repeat = e.repeat;
    result.keysym.scancode = (EScanCode)e.keysym.scancode;
    result.keysym.sym = (EKeyCode)e.keysym.sym;
    result.keysym.mod = e.keysym.mod;
    return result;
}

FMouseMotionEvent ConvertMouseMotionEvent(const SDL_MouseMotionEvent& e)
{
    FMouseMotionEvent result;
    static_assert(sizeof(FMouseMotionEvent) == sizeof(SDL_MouseMotionEvent),
		"FMouseMotionEvent is not the same size as SDL_MouseMotionEvent");
    memcpy(&result, &e, sizeof(result));
    return result;
}

FMouseButtonEvent ConvertMouseButtonEvent(const SDL_MouseButtonEvent& e)
{
    FMouseButtonEvent result;
    static_assert(sizeof(FMouseButtonEvent) == sizeof(SDL_MouseButtonEvent),
		"FMouseButtonEvent is not the same size as SDL_MouseButtonEvent");
    memcpy(&result, &e, sizeof(result));
    return result;
}

FMouseWheelEvent ConvertMouseWheelEvent(const SDL_MouseWheelEvent& e)
{
    FMouseWheelEvent result;
    result.type = e.type;
    result.timestamp = e.timestamp;
    result.windowID = e.windowID;
    result.which = e.which;
    result.WheelX = e.x;
    result.WheelY = e.y;
    result.direction = e.direction;
    return result;
}

} /* namespace tc */
