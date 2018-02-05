#pragma once
#include "InputHandler.h"

#include <Log.h>
#include <Vector2.h>
#include <Matrix3.h>

typedef struct NVGcontext NVGcontext;

namespace tc
{

struct NGCanvasPrivateData;
class NGView;
class NGBaseNode;

class NGCanvas
{
public:
    NGCanvas();
    ~NGCanvas();

    void Init();
    void Shutdown();

    void Paint(NGView* view);

private:
    friend NGBaseNode;

    void RegisterNode(NGBaseNode* node);
    void UnregisterNode(NGBaseNode* node);

    NGCanvasPrivateData* Pd;
};

class NGView : public IInputHandler
{
public:
    void Init(NVGcontext* vg)
    {
        Vg = vg;
    }

    void Shutdown()
    {
    }

    NVGcontext* GetNVGContext() const
    {
        return Vg;
    }

    bool MouseMoved(const FMouseMotionEvent& evt) override
    {
        if (bIsDragging)
        {
            auto currMousePos = IntVector2(evt.x, evt.y);
            auto delta = currMousePos - LastMousePosition;
            ViewSpaceMove(-delta.x_, -delta.y_);
            LastMousePosition = currMousePos;
            return true;
        }
        return false;
    }

    bool MousePressed(const FMouseButtonEvent& evt) override
    {
        bIsDragging = true;
        LastMousePosition.x_ = evt.x;
        LastMousePosition.y_ = evt.y;
        return true;
    }

    bool MouseReleased(const FMouseButtonEvent& evt) override
    {
        bIsDragging = false;
        return true;
    }

    bool MouseWheelRolled(const FMouseWheelEvent& evt) override
    {
        if (evt.WheelY >= 1)
        {
            ViewSpaceMove(evt.X - PositionOnScreen.x_, evt.Y - PositionOnScreen.y_);
            ScaleFactor += 0.1f;
            ViewSpaceMove(PositionOnScreen.x_ - evt.X, PositionOnScreen.y_ - evt.Y);
        }
        else if (evt.WheelY <= -1)
        {
            ViewSpaceMove(evt.X - PositionOnScreen.x_, evt.Y - PositionOnScreen.y_);
            ScaleFactor -= 0.1f;
            ViewSpaceMove(PositionOnScreen.x_ - evt.X, PositionOnScreen.y_ - evt.Y);
        }
        return true;
    }

    void Move(float x, float y)
    {
        Position.x_ += x;
        Position.y_ += y;
    }

    void ViewSpaceMove(float x, float y)
    {
        Position.x_ += x / ScaleFactor;
        Position.y_ += y / ScaleFactor;
    }

    void Scale(float ratio)
    {
        ScaleFactor *= ratio;
    }

    const Vector2& GetPosition() const
    {
        return Position;
    }

    void SetPosition(const Vector2& value)
    {
        Position = value;
    }

    float GetScale() const
    {
        return ScaleFactor;
    }

    void SetScale(float value)
    {
        ScaleFactor = value;
    }

    Vector2 TransformVector(const Vector2& v)
    {
        return {ScaleFactor * (v.x_ - Position.x_), ScaleFactor * (v.y_ - Position.y_)};
    }

    const Vector2& GetPositionOnScreen() const
    {
        return PositionOnScreen;
    }

    void SetPositionOnScreen(const Vector2& value)
    {
        PositionOnScreen = value;
    }

    const Vector2& GetSizeOnScreen() const
    {
        return SizeOnScreen;
    }

    void SetSizeOnScreen(const Vector2& value)
    {
        SizeOnScreen = value;
    }

private:
    Vector2 Position;
    float ScaleFactor = 1.0f;
    Vector2 PositionOnScreen;
    Vector2 SizeOnScreen;
    bool bIsDragging = false;
    IntVector2 LastMousePosition;
    NVGcontext* Vg;
};

class NGBaseNode
{
public:
    explicit NGBaseNode(NGCanvas* Canvas);

    virtual ~NGBaseNode();

    virtual const char* GetName() const;

    float GetWidth() const;
    float GetHeight() const;

    const Vector2& GetPosition() const
    {
        return Position;
    }

    void SetPosition(const Vector2& value)
    {
        Position = value;
    }

    void SetPosition(float x, float y)
    {
        Position.x_ = x;
        Position.y_ = y;
    }

    void Paint(NGView* view);

private:
    NGCanvas* Canvas;
    Vector2 Position;
};

} /* namespace tc */
