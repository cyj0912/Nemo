#include "Camera.h"

namespace tc
{

FCamera::FCamera()
    : ZNear(1.0f), ZFar(1000.0f), FieldOfViewY(1.22f), AspectRatio(1.0f),
      bUsesOrthoProjection(false)
{
}

Matrix4 FCamera::GetProjectionMatrix()
{
    /*
     * TODO:
     * OpenGL has z from -1 to 1, but DX uses 0 to 1
     */
    auto projection = Matrix4::ZERO;

    if (!bUsesOrthoProjection)
    {
        float h = (1.0f / tanf(FieldOfViewY * 0.5f));
        float w = h / AspectRatio;
        float q = -(ZFar + ZNear) / (ZFar - ZNear);
        float r = -(2.0f * ZFar * ZNear) / (ZFar - ZNear);

        projection[0][0] = w;
        projection[1][1] = h;
        projection[2][2] = q;
        projection[2][3] = r;
        projection[3][2] = -1.0f;
    }
    else
    {
        throw "unimplemented";
    }
    return projection;
}

}
