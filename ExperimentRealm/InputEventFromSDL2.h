#pragma once

#include "InputEvent.h"

#include <SDL_events.h>

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
    static_assert(sizeof(FMouseMotionEvent) == sizeof(SDL_MouseMotionEvent));
    memcpy(&result, &e, sizeof(result));
    return result;
}

FMouseButtonEvent ConvertMouseButtonEvent(const SDL_MouseButtonEvent& e)
{
    FMouseButtonEvent result;
    static_assert(sizeof(FMouseButtonEvent) == sizeof(SDL_MouseButtonEvent));
    memcpy(&result, &e, sizeof(result));
    return result;
}

FMouseWheelEvent ConvertMouseWheelEvent(const SDL_MouseWheelEvent& e)
{
    FMouseWheelEvent result;
    static_assert(sizeof(FMouseWheelEvent) == sizeof(SDL_MouseWheelEvent));
    memcpy(&result, &e, sizeof(result));
    return result;
}

} /* namespace tc */
