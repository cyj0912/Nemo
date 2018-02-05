#pragma once

#include "InputEvent.h"

namespace tc
{

class IInputHandler
{
public:
    virtual ~IInputHandler()
    {
        if (PrevHandler)
            PrevHandler->NextHandler = NextHandler;
        if (NextHandler)
            NextHandler->PrevHandler = PrevHandler;
    }

    virtual bool KeyPressed(const FKeyboardEvent& evt) { return false; }
    virtual bool KeyReleased(const FKeyboardEvent& evt) { return false; }
    virtual bool MouseMoved(const FMouseMotionEvent &evt) { return false; }
    virtual bool MousePressed(const FMouseButtonEvent &evt) { return false; }
    virtual bool MouseReleased(const FMouseButtonEvent &evt) { return false; }
    virtual bool MouseWheelRolled(const FMouseWheelEvent &evt) { return false; }

    IInputHandler* PrevHandler = nullptr;
    IInputHandler* NextHandler = nullptr;
};

class FMetaInputHandler : public IInputHandler
{
public:
    bool KeyPressed(const FKeyboardEvent& evt) override;

    bool KeyReleased(const FKeyboardEvent& evt) override;

    bool MouseMoved(const FMouseMotionEvent& evt) override;

    bool MousePressed(const FMouseButtonEvent& evt) override;

    bool MouseReleased(const FMouseButtonEvent& evt) override;

    bool MouseWheelRolled(const FMouseWheelEvent& evt) override;

    void Insert(IInputHandler* handler);

    void InsertFront(IInputHandler* handler);
};

class FMulticastInputHandler : public IInputHandler
{
public:
    bool KeyPressed(const FKeyboardEvent& evt) override;

    bool KeyReleased(const FKeyboardEvent& evt) override;

    bool MouseMoved(const FMouseMotionEvent& evt) override;

    bool MousePressed(const FMouseButtonEvent& evt) override;

    bool MouseReleased(const FMouseButtonEvent& evt) override;

    bool MouseWheelRolled(const FMouseWheelEvent& evt) override;

    void Insert(IInputHandler* handler);

    void InsertFront(IInputHandler* handler);
};

} /* namespace tc */
