#include "InputHandler.h"

namespace tc
{

bool FMetaInputHandler::KeyPressed(const FKeyboardEvent& evt)
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

bool FMetaInputHandler::KeyReleased(const FKeyboardEvent& evt)
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

bool FMetaInputHandler::MouseMoved(const FMouseMotionEvent& evt)
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

bool FMetaInputHandler::MousePressed(const FMouseButtonEvent& evt)
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

bool FMetaInputHandler::MouseReleased(const FMouseButtonEvent& evt)
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

bool FMetaInputHandler::MouseWheelRolled(const FMouseWheelEvent& evt)
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
    if (!this->NextHandler)
    {
        Insert(handler);
    }
    else
    {
        this->NextHandler->PrevHandler = handler;
        handler->NextHandler = this->NextHandler;

        this->NextHandler = handler;
        handler->PrevHandler = this;
    }
}

bool FMulticastInputHandler::KeyPressed(const FKeyboardEvent& evt)
{
    bool handled = false;
    IInputHandler* next = NextHandler;
    while (next)
    {
        handled = next->KeyPressed(evt);
        next = next->NextHandler;
    }
    return handled;
}

bool FMulticastInputHandler::KeyReleased(const FKeyboardEvent& evt)
{
    bool handled = false;
    IInputHandler* next = NextHandler;
    while (next)
    {
        handled = next->KeyReleased(evt);
        next = next->NextHandler;
    }
    return handled;
}

bool FMulticastInputHandler::MouseMoved(const FMouseMotionEvent& evt)
{
    bool handled = false;
    IInputHandler* next = NextHandler;
    while (next)
    {
        handled = next->MouseMoved(evt);
        next = next->NextHandler;
    }
    return handled;
}

bool FMulticastInputHandler::MousePressed(const FMouseButtonEvent& evt)
{
    bool handled = false;
    IInputHandler* next = NextHandler;
    while (next)
    {
        handled = next->MousePressed(evt);
        next = next->NextHandler;
    }
    return handled;
}

bool FMulticastInputHandler::MouseReleased(const FMouseButtonEvent& evt)
{
    bool handled = false;
    IInputHandler* next = NextHandler;
    while (next)
    {
        handled = next->MouseReleased(evt);
        next = next->NextHandler;
    }
    return handled;
}

bool FMulticastInputHandler::MouseWheelRolled(const FMouseWheelEvent& evt)
{
    bool handled = false;
    IInputHandler* next = NextHandler;
    while (next)
    {
        handled = next->MouseWheelRolled(evt);
        next = next->NextHandler;
    }
    return handled;
}

void FMulticastInputHandler::Insert(IInputHandler* handler)
{
    IInputHandler* next = this;
    while (next->NextHandler)
        next = next->NextHandler;
    next->NextHandler = handler;
    handler->PrevHandler = next;
}

void FMulticastInputHandler::InsertFront(IInputHandler* handler)
{
    if (!this->NextHandler)
    {
        Insert(handler);
    }
    else
    {
        this->NextHandler->PrevHandler = handler;
        handler->NextHandler = this->NextHandler;

        this->NextHandler = handler;
        handler->PrevHandler = this;
    }
}

} /* namespace tc */
