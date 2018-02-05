#pragma once
#include "InputHandler.h"

#include <UsingStl.h>

namespace tc
{

class FNode;
class Vector3;
class FEntityManager;
class IRayIntersectComponent;
class FInteractionSystem;
class FEditorMaster;
class FBaseEntity;
class FTranslateGizmoInputHandler;
class FPointTranslateGizmoInputHandler;

class IInteractionComponent
{
public:
    virtual void CreateGizmo(FInteractionSystem* system) = 0;
    virtual IRayIntersectComponent* GetRayIntersectComponent() = 0;
};

class FInteractionSystem : public IInputHandler
{
public:
    explicit FInteractionSystem(FEntityManager* em)
            : EntityManager(em), EditorMaster(nullptr), SelectedEntity(nullptr), Gizmo(nullptr),
              TGIHandler(nullptr), PTGIHandler(nullptr), bAllowMultiSelect(false)
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

    bool MousePressed(const FMouseButtonEvent& evt) override;

    void CreateNodeTranslateGizmo(FNode* property);

    void CreatePointTranslateGizmo(Vector3* property);

    void RemoveGizmos();

    void ImGuiUpdate();

    FBaseEntity* GetSelectedEntity() const
    {
        return SelectedEntity;
    }

    const set<FBaseEntity*>& GetSelectedEntities() const
    {
        return SelectedEntities;
    }

protected:
    bool IsEntityInSelection(FBaseEntity* entity)
    {
        return entity == SelectedEntity || SelectedEntities.find(entity) != SelectedEntities.end();
    }

private:
    FEntityManager* EntityManager;
    FEditorMaster* EditorMaster;

    FBaseEntity* SelectedEntity;
    set<FBaseEntity*> SelectedEntities;
    FBaseEntity* Gizmo;
    FTranslateGizmoInputHandler* TGIHandler;
    FPointTranslateGizmoInputHandler* PTGIHandler;

    bool bAllowMultiSelect;
};

} /* namespace tc */
