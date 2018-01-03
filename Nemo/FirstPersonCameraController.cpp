#include "FirstPersonCameraController.h"
#include <SceneNode.h>

namespace tc
{

FFirstPersonCameraController::FFirstPersonCameraController() : AxisScaleFactor(1.0f)
{
}

FFirstPersonCameraController::~FFirstPersonCameraController()
{
}

const TRefPtr<FSceneNode>& FFirstPersonCameraController::GetCameraNode() const
{
    return CameraNode;
}

void FFirstPersonCameraController::SetCameraNode(const TRefPtr<FSceneNode>& value)
{
    CameraNode = value;
}

float FFirstPersonCameraController::GetAxisScaleFactor() const
{
    return AxisScaleFactor;
}

void FFirstPersonCameraController::SetAxisScaleFactor(float value)
{
    AxisScaleFactor = value;
}

void FFirstPersonCameraController::AxisEvent(float dx, float dy)
{
    CameraNode->Yaw(dx * AxisScaleFactor);
    CameraNode->Pitch(dy * AxisScaleFactor);
}

}
