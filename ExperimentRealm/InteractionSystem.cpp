#include "InteractionSystem.h"
#include "EntityManager.h"
#include "EditorMaster.h"
#include "EntityLibrary.h"
#include "Gizmo.h"

#include <Ray.h>

namespace tc
{

bool FInteractionSystem::MousePressed(const FMouseButtonEvent& evt)
{
    if (evt.button == EMouseButton::Left)
    {
        auto worldRay = EditorMaster->GetViewPort()->GetRayTo(evt.x, evt.y);

        //The closer entity wins if contested
        FBaseEntity* NewSelectedEntity = nullptr;
        IInteractionComponent* NewSelectedEntityIComp = nullptr;
        float minHitDistance = M_INFINITY;
        for (auto* interactComp : EntityManager->GetComponents<IInteractionComponent>())
        {
            IRayIntersectComponent* rayTestComp = interactComp->GetRayIntersectComponent();
            float distance = rayTestComp->RayHitDistance(worldRay);
            if(distance < minHitDistance)
            {
                NewSelectedEntity = dynamic_cast<FBaseEntity*>(interactComp);
                NewSelectedEntityIComp = interactComp;
                minHitDistance = distance;
            }
        }

        if (NewSelectedEntity)
        {
            if (NewSelectedEntity != SelectedEntity)
            {
                RemoveGizmos();
                NewSelectedEntityIComp->CreateGizmo(this);
                SelectedEntity = NewSelectedEntity;
                return true;
            }

            //Selected the original selected, nothing happens
            return false;
        }

        //When the new selection is nothing
        if (NewSelectedEntity != SelectedEntity)
        {
            RemoveGizmos();
            SelectedEntity = NewSelectedEntity;
        }
    }
    return false;
}

void FInteractionSystem::CreateNodeTranslateGizmo(FNode* property)
{
    auto gizmo = new FTranslateGizmo(property);
    EditorMaster->InsertRenderAndInit(gizmo);
    TGIHandler = new FTranslateGizmoInputHandler(gizmo, EditorMaster->GetViewPort());
    Gizmo = gizmo;
}

void FInteractionSystem::CreatePointTranslateGizmo(Vector3* property)
{
    auto gizmo = new FPointTranslateGizmo(property);
    EditorMaster->InsertRenderAndInit(gizmo);
    PTGIHandler = new FPointTranslateGizmoInputHandler(gizmo, EditorMaster->GetViewPort());
    Gizmo = gizmo;
}

void FInteractionSystem::RemoveGizmos()
{
    auto* renderComp = dynamic_cast<IRenderComponent*>(Gizmo);
    if (renderComp)
        renderComp->RenderDestroy();
    delete Gizmo;
    delete TGIHandler;
    delete PTGIHandler;
}

} /* namespace tc */
