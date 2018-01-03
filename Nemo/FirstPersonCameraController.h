#pragma once
#include <Pointer.h>

namespace tc
{

class FSceneNode;

class FFirstPersonCameraController
{
    TRefPtr<FSceneNode> CameraNode;
    float AxisScaleFactor;
public:
    FFirstPersonCameraController();
    ~FFirstPersonCameraController();

    const TRefPtr<FSceneNode> &GetCameraNode() const;
    void SetCameraNode(const TRefPtr<FSceneNode> &value);

    float GetAxisScaleFactor() const;
    void SetAxisScaleFactor(float value);

    void AxisEvent(float dx, float dy);
};

}
