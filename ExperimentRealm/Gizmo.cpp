#include <CoreImage/CoreImage.h>
#include "Gizmo.h"

namespace tc
{

EAxis FTranslateGizmo::HitWorldSpaceRay(const Ray& worldRay)
{
    auto worldToLocal = GetTransformMatrix().Inverse();
    auto localRay = worldRay.Transformed(worldToLocal);

    BoundingBox xAabb = BoundingBox(Vector3(0.0f, -0.1f, -0.1f), Vector3(1.1f, 0.1f, 0.1f));
    BoundingBox yAabb = BoundingBox(Vector3(-0.1f, 0.0f -0.1f), Vector3(0.1f, 1.1f, 0.1f));
    BoundingBox zAabb = BoundingBox(Vector3(-0.1f, -0.1f, 0.0f), Vector3(0.1f, 0.1f, 1.1f));
    if (localRay.HitDistance(xAabb) < M_INFINITY)
        return EAxis::X;
    else if (localRay.HitDistance(yAabb) < M_INFINITY)
        return EAxis::Y;
    else if (localRay.HitDistance(zAabb) < M_INFINITY)
        return EAxis::Z;
    else
        return EAxis::None;
}

Ray FTranslateGizmo::GetAxisRayWorldSpace(EAxis axis)
{
    Vector3 dir;
    switch (axis)
    {
    case EAxis::X:
        dir = Vector3::UNIT_X;
        break;
    case EAxis::Y:
        dir = Vector3::UNIT_Y;
        break;
    case EAxis::Z:
        dir = Vector3::UNIT_Z;
        break;
    default:
        //Or probably throw an error or something
        return Ray();
    }
    Ray localRay = Ray(Vector3::ZERO, dir);
    return localRay.Transformed(GetTransformMatrix());
}

void FPointTranslateGizmo::MoveAlongAxis(EAxis axis, float offset)
{
    switch (axis)
    {
    case EAxis::X:
        GetTransformNode().Translate(offset, 0.0f, 0.0f);
        break;
    case EAxis::Y:
        GetTransformNode().Translate(0.0f, offset, 0.0f);
        break;
    case EAxis::Z:
        GetTransformNode().Translate(0.0f, 0.0f, offset);
        break;
    default:
        break;
    }
    *TrackedPoint = GetTransformNode().GetTranslation();
}

EAxis FPointTranslateGizmo::HitWorldSpaceRay(const Ray& worldRay)
{
    auto worldToLocal = GetTransformMatrix().Inverse();
    auto localRay = worldRay.Transformed(worldToLocal);

    BoundingBox xAabb = BoundingBox(Vector3(0.0f, -0.1f, -0.1f), Vector3(1.1f, 0.1f, 0.1f));
    BoundingBox yAabb = BoundingBox(Vector3(-0.1f, 0.0f -0.1f), Vector3(0.1f, 1.1f, 0.1f));
    BoundingBox zAabb = BoundingBox(Vector3(-0.1f, -0.1f, 0.0f), Vector3(0.1f, 0.1f, 1.1f));
    if (localRay.HitDistance(xAabb) < M_INFINITY)
        return EAxis::X;
    else if (localRay.HitDistance(yAabb) < M_INFINITY)
        return EAxis::Y;
    else if (localRay.HitDistance(zAabb) < M_INFINITY)
        return EAxis::Z;
    else
        return EAxis::None;
}

Ray FPointTranslateGizmo::GetAxisRayWorldSpace(EAxis axis)
{
    Vector3 dir;
    switch (axis)
    {
    case EAxis::X:
        dir = Vector3::UNIT_X;
        break;
    case EAxis::Y:
        dir = Vector3::UNIT_Y;
        break;
    case EAxis::Z:
        dir = Vector3::UNIT_Z;
        break;
    default:
        //Or probably throw an error or something
        return Ray();
    }
    Ray localRay = Ray(Vector3::ZERO, dir);
    return localRay.Transformed(GetTransformMatrix());
}


FTranslateGizmoInputHandler::FTranslateGizmoInputHandler(FTranslateGizmo* gizmo, FViewPort* vp)
        : Gizmo(gizmo), ViewPort(vp), DragAlongAxis(EAxis::None), bDraggingAlong(false)
{
}

bool FTranslateGizmoInputHandler::MouseMoved(const FMouseMotionEvent& evt)
{
    if (bDraggingAlong)
    {
        Gizmo->SetHighlightAxis(DragAlongAxis);
        auto worldRay = ViewPort->GetRayTo(evt.x, evt.y);

        Vector3 closestPoint = DragAlongRay.ClosestPoint(worldRay);
        Vector3 offset = closestPoint - LastClosestPoint;
        float offsetDist = offset.Length();
        float offsetDotRaydir = offset.DotProduct(DragAlongRay.Direction);
        float oneWithSign = offsetDotRaydir > 0.0f ? 1.0f : -1.0f;
        switch (DragAlongAxis)
        {
        case EAxis::X:
            Gizmo->GetTransformNode().Translate(offsetDist * oneWithSign, 0.0f, 0.0f);
            break;
        case EAxis::Y:
            Gizmo->GetTransformNode().Translate(0.0f, offsetDist * oneWithSign, 0.0f);
            break;
        case EAxis::Z:
            Gizmo->GetTransformNode().Translate(0.0f, 0.0f, offsetDist * oneWithSign);
            break;
        default:
            break;
        }
        LastClosestPoint = closestPoint;
        return true;
    }
    auto worldRay = ViewPort->GetRayTo(evt.x, evt.y);
    auto hit = Gizmo->HitWorldSpaceRay(worldRay);
    Gizmo->SetHighlightAxis(hit);
    return false;
}

bool FTranslateGizmoInputHandler::MousePressed(const FMouseButtonEvent& evt)
{
    if (evt.button == EMouseButton::Left)
    {
        auto ray = ViewPort->GetRayTo(evt.x, evt.y);
        auto hit = Gizmo->HitWorldSpaceRay(ray);
        if (hit != EAxis::None)
        {
            DragAlongRay = Gizmo->GetAxisRayWorldSpace(hit);
            DragAlongAxis = hit;
            bDraggingAlong = true;

            LastClosestPoint = DragAlongRay.ClosestPoint(ray);
            return true;
        }
    }
    return false;
}

bool FTranslateGizmoInputHandler::MouseReleased(const FMouseButtonEvent& evt)
{
    bDraggingAlong = false;
    return false;
}

FPointTranslateGizmoInputHandler::FPointTranslateGizmoInputHandler(FPointTranslateGizmo* gizmo, FViewPort* vp)
    : Gizmo(gizmo), ViewPort(vp), DragAlongAxis(EAxis::None), bDraggingAlong(false)
{
}

bool FPointTranslateGizmoInputHandler::MouseMoved(const FMouseMotionEvent& evt)
{
    if (bDraggingAlong)
    {
        Gizmo->SetHighlightAxis(DragAlongAxis);
        auto worldRay = ViewPort->GetRayTo(evt.x, evt.y);

        Vector3 closestPoint = DragAlongRay.ClosestPoint(worldRay);
        Vector3 offset = closestPoint - LastClosestPoint;
        float offsetDist = offset.Length();
        float offsetDotRaydir = offset.DotProduct(DragAlongRay.Direction);
        float oneWithSign = offsetDotRaydir > 0.0f ? 1.0f : -1.0f;
        Gizmo->MoveAlongAxis(DragAlongAxis, offsetDist * oneWithSign);
        LastClosestPoint = closestPoint;
        return true;
    }
    auto worldRay = ViewPort->GetRayTo(evt.x, evt.y);
    auto hit = Gizmo->HitWorldSpaceRay(worldRay);
    Gizmo->SetHighlightAxis(hit);
    return false;
}

bool FPointTranslateGizmoInputHandler::MousePressed(const FMouseButtonEvent& evt)
{
    if (evt.button == EMouseButton::Left)
    {
        auto ray = ViewPort->GetRayTo(evt.x, evt.y);
        auto hit = Gizmo->HitWorldSpaceRay(ray);
        if (hit != EAxis::None)
        {
            DragAlongRay = Gizmo->GetAxisRayWorldSpace(hit);
            DragAlongAxis = hit;
            bDraggingAlong = true;

            LastClosestPoint = DragAlongRay.ClosestPoint(ray);
            return true;
        }
    }
    return false;
}

bool FPointTranslateGizmoInputHandler::MouseReleased(const FMouseButtonEvent& evt)
{
    bDraggingAlong = false;
    return false;
}

} /* namespace tc */

