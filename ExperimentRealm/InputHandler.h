#pragma once
#include <SDL_events.h>

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

    virtual bool KeyPressed(const SDL_KeyboardEvent& evt) { return false; }
    virtual bool KeyReleased(const SDL_KeyboardEvent& evt) { return false; }
    virtual bool MouseMoved(const SDL_MouseMotionEvent &evt) { return false; }
    virtual bool MousePressed(const SDL_MouseButtonEvent &evt) { return false; }
    virtual bool MouseReleased(const SDL_MouseButtonEvent &evt) { return false; }
    virtual bool MouseWheelRolled(const SDL_MouseWheelEvent &evt) { return false; }

    IInputHandler* PrevHandler = nullptr;
    IInputHandler* NextHandler = nullptr;
};

class FMetaInputHandler : public IInputHandler
{
public:
    bool KeyPressed(const SDL_KeyboardEvent& evt) override;

    bool KeyReleased(const SDL_KeyboardEvent& evt) override;

    bool MouseMoved(const SDL_MouseMotionEvent& evt) override;

    bool MousePressed(const SDL_MouseButtonEvent& evt) override;

    bool MouseReleased(const SDL_MouseButtonEvent& evt) override;

    bool MouseWheelRolled(const SDL_MouseWheelEvent& evt) override;

    void Insert(IInputHandler* handler);

    void InsertFront(IInputHandler* handler);
};

} /* namespace tc */
