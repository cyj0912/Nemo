#pragma once
#include "InputHandler.h"

#include <Matrix3x4.h>
#include <UsingStl.h>

namespace tc
{

class FNode;
class FEntityManager;
class IRayIntersectComponent;
class FInteractionSystem;
class FEditorMaster;
class FBaseEntity;
class FTranslateGizmo;
class FTranslateGizmoInputHandler;
class FPointTranslateGizmoInputHandler;

enum EGizmoFlags
{
    GF_NONE = 0x0,
    GF_TRANSLATE = 0x1,
    GF_ROTATE = 0x2,
    GF_SCALE = 0x4,
};

class IInteractionComponent
{
public:
    virtual EGizmoFlags GetGizmoFlags()
    {
        return GF_NONE;
    }

    virtual Matrix3x4 QueryPreferredGizmoTransform()
    {
        return {};
    }

    virtual void SetGizmoTransformStart(const FNode& node) {}
    virtual void UpdateFromGizmoTransform(const FNode& node) {}
    virtual IRayIntersectComponent* GetRayIntersectComponent() = 0;
};

class FInteractionSystem : public IInputHandler
{
public:
    explicit FInteractionSystem(FEntityManager* em)
            : EntityManager(em), EditorMaster(nullptr), MainSelection(nullptr),
              TranslateGizmoInputHandler(nullptr), bAllowMultiSelect(false),
              TranslateGizmoBasis(nullptr), TranslateGizmo(nullptr)
    {
    }

    FEditorMaster* GetEditorMaster() const
    {
        return EditorMaster;
    }

    void SetEditorMaster(FEditorMaster* v)
    {
        EditorMaster = v;
    }

    bool KeyPressed(const FKeyboardEvent& evt) override;

    bool KeyReleased(const FKeyboardEvent& evt) override;

    bool MouseMoved(const FMouseMotionEvent& evt) override;

    bool MousePressed(const FMouseButtonEvent& evt) override;

    bool MouseReleased(const FMouseButtonEvent& evt) override;

    void CreateGizmoFromSelections();

    void UpdateGizmo();

    void RemoveGizmo();

    void ImGuiUpdate();

    FBaseEntity* GetMainSelection() const
    {
        return MainSelection;
    }

    const vector<FBaseEntity*>& GetSelectedEntities() const
    {
        return SelectedEntities;
    }

protected:
    bool IsEntityInSelection(FBaseEntity* entity)
    {
        return std::find(SelectedEntities.begin(), SelectedEntities.end(), entity) != SelectedEntities.end();
    }

private:
    FEntityManager* EntityManager;
    FEditorMaster* EditorMaster;

    FBaseEntity* MainSelection;
    vector<FBaseEntity*> SelectedEntities;

    bool bAllowMultiSelect;

    FNode* TranslateGizmoBasis;
    FTranslateGizmo* TranslateGizmo;
    FTranslateGizmoInputHandler* TranslateGizmoInputHandler;
};

} /* namespace tc */
