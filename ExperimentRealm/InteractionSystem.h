#pragma once
#include "InputHandler.h"

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
              TGIHandler(nullptr), PTGIHandler(nullptr)
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

    bool MousePressed(const FMouseButtonEvent& evt) override;

    void CreateNodeTranslateGizmo(FNode* property);

    void CreatePointTranslateGizmo(Vector3* property);

    void RemoveGizmos();

private:
    FEntityManager* EntityManager;
    FEditorMaster* EditorMaster;

    FBaseEntity* SelectedEntity;
    FBaseEntity* Gizmo;
    FTranslateGizmoInputHandler* TGIHandler;
    FPointTranslateGizmoInputHandler* PTGIHandler;
};

} /* namespace tc */
