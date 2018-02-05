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

bool FInteractionSystem::MouseMoved(const FMouseMotionEvent& evt)
{
    if (TranslateGizmoInputHandler && TranslateGizmoInputHandler->MouseMoved(evt))
    {
        UpdateGizmo();
        return true;
    }
    return false;
}

bool FInteractionSystem::MousePressed(const FMouseButtonEvent& evt)
{
    if (TranslateGizmoInputHandler && TranslateGizmoInputHandler->MousePressed(evt))
        return true;

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
            //LOGDEBUG("%s %g\n", dynamic_cast<FBaseEntity*>(interactComp)->GetTypeNameInString(), distance);
            if(distance < minHitDistance)
            {
                hitEntity = dynamic_cast<FBaseEntity*>(interactComp);
                hitEntityIaComp = interactComp;
                minHitDistance = distance;
            }
        }

        if (IsEntityInSelection(hitEntity))
        {
            //Already selected, do nothing
            return false;
        }

        if (hitEntity)
        {
            UpdateGizmo();
            RemoveGizmo();
            if (bAllowMultiSelect)
            {
                SelectedEntities.push_back(hitEntity);
                if (!MainSelection)
                    MainSelection = hitEntity;
            }
            else
            {
                SelectedEntities.clear();
                SelectedEntities.push_back(hitEntity);
                MainSelection = hitEntity;
            }
            CreateGizmoFromSelections();
            return true;
        }
        else
        {
            UpdateGizmo();
            RemoveGizmo();
            MainSelection = nullptr;
            SelectedEntities.clear();
            //Should return true or false?
        }
    }
    return false;
}

bool FInteractionSystem::MouseReleased(const FMouseButtonEvent& evt)
{
    return TranslateGizmoInputHandler && TranslateGizmoInputHandler->MouseReleased(evt);
}

void FInteractionSystem::CreateGizmoFromSelections()
{
    Vector3 basisTranslation;
    int numBases = 0;
    for (auto* pEntity : SelectedEntities)
    {
        auto* interaction = dynamic_cast<IInteractionComponent*>(pEntity);
        if (interaction->GetGizmoFlags() & GF_TRANSLATE)
        {
            basisTranslation += interaction->QueryPreferredGizmoTransform().Translation();
            numBases++;
        }
    }
    if (numBases == 0)
        return;

    basisTranslation /= (float)numBases;
    TranslateGizmoBasis = new FNode();
    TranslateGizmoBasis->SetTranslation(basisTranslation);
    TranslateGizmo = new FTranslateGizmo(TranslateGizmoBasis);
    EditorMaster->InsertRenderAndInit(TranslateGizmo);
    TranslateGizmoInputHandler = new FTranslateGizmoInputHandler(TranslateGizmo, EditorMaster->GetViewPort());

    for (auto* pEntity : SelectedEntities)
    {
        auto* interaction = dynamic_cast<IInteractionComponent*>(pEntity);
        interaction->SetGizmoTransformStart(*TranslateGizmoBasis);
    }
}

void FInteractionSystem::UpdateGizmo()
{
    if (TranslateGizmoBasis)
    {
        for (auto* pEntity : SelectedEntities)
        {
            auto* interaction = dynamic_cast<IInteractionComponent*>(pEntity);
            interaction->UpdateFromGizmoTransform(*TranslateGizmoBasis);
        }
    }
}

void FInteractionSystem::RemoveGizmo()
{
    delete TranslateGizmoInputHandler;
    TranslateGizmoInputHandler = nullptr;
    if (TranslateGizmo)
    {
        TranslateGizmo->RenderDestroy();
        delete TranslateGizmo;
        TranslateGizmo = nullptr;
    }
    delete TranslateGizmoBasis;
    TranslateGizmoBasis = nullptr;
}

void FInteractionSystem::ImGuiUpdate()
{
    ImGui::Begin("Interaction");
    ImGui::Text("Allow Multiselect: %s", bAllowMultiSelect ? "Yes" : "No");
    ImGui::Text("Main Selection: %s", MainSelection ? MainSelection->GetTypeNameInString() : "None");
    ImGui::Text("Selections:");
    for (auto iter = SelectedEntities.begin(); iter != SelectedEntities.end(); iter++)
        ImGui::Text("%s", (*iter)->GetTypeNameInString());
    if (MainSelection)
        MainSelection->ImGuiUpdate(this);
    ImGui::End();
}

} /* namespace tc */
