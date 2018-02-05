#pragma once
#include "SceneMgrAPI.h"
#include <Pointer.h>
#include <Frustum.h>
#include <Matrix4.h>

namespace tc
{

class SCENEMGR_API FCamera
{
    float ZNear, ZFar, FieldOfViewY, AspectRatio;
    bool bUsesOrthoProjection;
public:
    FCamera();
    Matrix4 GetProjectionMatrix();

    float GetZNear() const
    {
        return ZNear;
    }

    void SetZNear(float value)
    {
        ZNear = value;
    }

    float GetZFar() const
    {
        return ZFar;
    }

    void SetZFar(float value)
    {
        ZFar = value;
    }

    float GetFieldOfViewY() const
    {
        return FieldOfViewY;
    }

    void SetFieldOfViewY(float value)
    {
        FieldOfViewY = value;
    }

    float GetAspectRatio() const
    {
        return AspectRatio;
    }

    void SetAspectRatio(float value)
    {
        AspectRatio = value;
    }

    bool DoesUseOrthoProjection() const
    {
        return bUsesOrthoProjection;
    }

    void SetUsesOrthoProjection(bool value)
    {
        bUsesOrthoProjection = value;
    }
};

}
