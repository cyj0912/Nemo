#pragma once

namespace tc
{

class FTranslateGizmo;
class FTranslateGizmoInputHandler;
class FPointTranslateGizmo;
class FPointTranslateGizmoInputHandler;
class FBaseEntity;
class FRayVisualizer;

class FEditorMaster : public IInputHandler
{
public:
    FEditorMaster();

    ~FEditorMaster() override;

    bool KeyPressed(const FKeyboardEvent& evt) override;

    bool KeyReleased(const FKeyboardEvent& evt) override;

    bool MousePressed(const FMouseButtonEvent& evt) override;

    void CreateGizmoFor(FBaseEntity* entity);

    void RemoveGizmos();

    void ToggleWireframe();

    bool IsWireframe() const;

    void ImGuiUpdate();

    void Render();

    void RenderDestroy();

    bool IsQuitting() const;

    void SetQuitting(bool v);

    FViewPort* GetViewPort() const;

private:
    bool bWireframe;
    vector<FBaseEntity*> EntityVector;
    FDummyRenderComponent RenderComponentListHead;

    FBaseEntity* SelectedEntity;

    FTranslateGizmo* TranslateGizmo;
    FTranslateGizmoInputHandler* TranslateGizmoInputHandler;
    FPointTranslateGizmo* PointTranslateGizmo;
    FPointTranslateGizmoInputHandler* PointTranslateGizmoInputHandler;

    bool bToQuit;

    FRayVisualizer* RayVisualizer;
};

} /* namespace tc */
