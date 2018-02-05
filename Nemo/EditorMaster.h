#pragma once
#include "RenderComponent.h"
#include "InputHandler.h"

namespace tc
{

class FTranslateGizmo;
class FTranslateGizmoInputHandler;
class FPointTranslateGizmo;
class FPointTranslateGizmoInputHandler;
class FBaseEntity;
class FRayVisualizer;
class FPrimitiveRenderer;
class FEntityManager;
class FInteractionSystem;

class FEditorMaster : public IInputHandler
{
public:
    FEditorMaster();

    ~FEditorMaster() override;

    bool KeyPressed(const FKeyboardEvent& evt) override;

    bool KeyReleased(const FKeyboardEvent& evt) override;

    bool MousePressed(const FMouseButtonEvent& evt) override;

    bool MouseMoved(const FMouseMotionEvent& evt) override
    {
        MainInputHandler->MouseMoved(evt);
        return false;
    }

    bool MouseReleased(const FMouseButtonEvent& evt) override
    {
        MainInputHandler->MouseReleased(evt);
        return IInputHandler::MouseReleased(evt);
    }

    bool MouseWheelRolled(const FMouseWheelEvent& evt) override
    {
        MainInputHandler->MouseWheelRolled(evt);
        return IInputHandler::MouseWheelRolled(evt);
    }

    void ToggleWireframe();

    bool IsWireframe() const;

    void ImGuiUpdate();

    void RenderInit();

    void Render();

    void RenderDestroy();

    bool IsQuitting() const;

    void SetQuitting(bool v);

    FViewPort* GetViewPort() const;

    FPrimitiveRenderer* GetPrimitiveRenderer() const
    {
        return PrimitiveRenderer;
    }

    void InsertRenderAndInit(IRenderComponent* comp);

    void RegisterEntity(FBaseEntity* entity, bool isSubEntity = false);

private:
    bool bWireframe;
    FDummyRenderComponent RenderComponentListHead;

    bool bToQuit;

    FRayVisualizer* RayVisualizer;

    FPrimitiveRenderer* PrimitiveRenderer;

    FEntityManager* EntityManager;

    FInteractionSystem* InteractionSystem;

    FMulticastInputHandler* MainInputHandler;
};

extern FEditorMaster* GEditorMaster;

} /* namespace tc */
