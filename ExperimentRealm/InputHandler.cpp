#include "InputHandler.h"

namespace tc
{


bool FMetaInputHandler::KeyPressed(const SDL_KeyboardEvent& evt)
{
    bool handled = false;
    IInputHandler* next = NextHandler;
    while (next && !handled)
    {
        handled = next->KeyPressed(evt);
        next = next->NextHandler;
    }
    return handled;
}

bool FMetaInputHandler::KeyReleased(const SDL_KeyboardEvent& evt)
{
    bool handled = false;
    IInputHandler* next = NextHandler;
    while (next && !handled)
    {
        handled = next->KeyReleased(evt);
        next = next->NextHandler;
    }
    return handled;
}

bool FMetaInputHandler::MouseMoved(const SDL_MouseMotionEvent& evt)
{
    bool handled = false;
    IInputHandler* next = NextHandler;
    while (next && !handled)
    {
        handled = next->MouseMoved(evt);
        next = next->NextHandler;
    }
    return handled;
}

bool FMetaInputHandler::MousePressed(const SDL_MouseButtonEvent& evt)
{
    bool handled = false;
    IInputHandler* next = NextHandler;
    while (next && !handled)
    {
        handled = next->MousePressed(evt);
        next = next->NextHandler;
    }
    return handled;
}

bool FMetaInputHandler::MouseReleased(const SDL_MouseButtonEvent& evt)
{
    bool handled = false;
    IInputHandler* next = NextHandler;
    while (next && !handled)
    {
        handled = next->MouseReleased(evt);
        next = next->NextHandler;
    }
    return handled;
}

bool FMetaInputHandler::MouseWheelRolled(const SDL_MouseWheelEvent& evt)
{
    bool handled = false;
    IInputHandler* next = NextHandler;
    while (next && !handled)
    {
        handled = next->MouseWheelRolled(evt);
        next = next->NextHandler;
    }
    return handled;
}

void FMetaInputHandler::Insert(IInputHandler* handler)
{
    IInputHandler* next = this;
    while (next->NextHandler)
        next = next->NextHandler;
    next->NextHandler = handler;
    handler->PrevHandler = next;
}

void FMetaInputHandler::InsertFront(IInputHandler* handler)
{
    this->NextHandler->PrevHandler = handler;
    handler->NextHandler = this->NextHandler;

    this->NextHandler = handler;
    handler->PrevHandler = this;

}

} /* namespace tc */
