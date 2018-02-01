#include "InteractionSystem.h"
#include "EntityManager.h"
#include "EditorMaster.h"
#include "EntityLibrary.h"
#include "Gizmo.h"

#include <Ray.h>

#include "imgui/imgui.h"

namespace tc
{

bool FInteractionSystem::KeyPressed(const FKeyboardEvent& evt)
{
    if (evt.keysym.sym == EKeyCode::Lshift || evt.keysym.sym == EKeyCode::Rshift)
        bAllowMultiSelect = true;
    return false;
}

bool FInteractionSystem::KeyReleased(const FKeyboardEvent& evt)
{
    if (evt.keysym.sym == EKeyCode::Lshift || evt.keysym.sym == EKeyCode::Rshift)
        bAllowMultiSelect = false;
    return false;
}

bool FInteractionSystem::MousePressed(const FMouseButtonEvent& evt)
{
    if (evt.button == EMouseButton::Left)
    {
        auto worldRay = EditorMaster->GetViewPort()->GetRayTo(evt.x, evt.y);

        //The closer entity wins if contested
        FBaseEntity* hitEntity = nullptr;
        IInteractionComponent* hitEntityIaComp = nullptr;
        float minHitDistance = M_INFINITY;
        for (auto* interactComp : EntityManager->GetComponents<IInteractionComponent>())
        {
            IRayIntersectComponent* rayTestComp = interactComp->GetRayIntersectComponent();
            if (!rayTestComp)
                continue;
            float distance = rayTestComp->RayHitDistance(worldRay);
            if(distance < minHitDistance)
            {
                hitEntity = dynamic_cast<FBaseEntity*>(interactComp);
                hitEntityIaComp = interactComp;
                minHitDistance = distance;
            }
        }

        FBaseEntity* originalSelection = SelectedEntity;

        if (hitEntity)
        {
            if (bAllowMultiSelect)
            {
                if (!IsEntityInSelection(hitEntity))
                {
                    if (SelectedEntity)
                        SelectedEntities.insert(hitEntity);
                    else
                        SelectedEntity = hitEntity;
                }
            }
            else
            {
                SelectedEntity = hitEntity;
                SelectedEntities.clear();
            }
        }
        else
        {
            SelectedEntity = nullptr;
            SelectedEntities.clear();
        }

        //TODO
        //handle gizmo creation/destruction
        //RemoveGizmos();
        //hitEntityIaComp->CreateGizmo(this);
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

void FInteractionSystem::ImGuiUpdate()
{
    ImGui::Begin("Interaction");
    ImGui::Text("Allow Multiselect: %s", bAllowMultiSelect ? "Yes" : "No");
    ImGui::Text("Selection: %s", SelectedEntity ? SelectedEntity->GetTypeNameInString() : "None");
    for (auto iter = SelectedEntities.begin(); iter != SelectedEntities.end(); iter++)
        ImGui::Text("%s", (*iter)->GetTypeNameInString());
    if (SelectedEntity)
        SelectedEntity->ImGuiUpdate(this);
    ImGui::End();
}

} /* namespace tc */
