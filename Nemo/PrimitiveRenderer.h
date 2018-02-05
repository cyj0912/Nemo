#pragma once

#include <Vector3.h>
#include <Color.h>
#include <UsingStl.h>

namespace tc
{

class FViewPort;

class FPrimitiveRenderer
{
public:
    static void RenderStaticInit();
    static void RenderStaticDestroy();

    FPrimitiveRenderer() : ViewPort(nullptr) {}

    void RenderInit(FViewPort* vp);
    void Render();
    void RenderDestroy();

    void BeginFrame()
    {
        PointsVector.clear();
        LinesVector.clear();
    }

    void DrawPoint(const Vector3& location, float size, const Color& color)
    {
        PointsVector.push_back({location, size, color});
    }

    void DrawLine(const Vector3& from, const Vector3& to, float width, const Color& color)
    {
        LinesVector.push_back({from, to, width, color});
    }

private:
    struct FPointInfo
    {
        Vector3 Location;
        float Size;
        Color Color;
    };
    vector<FPointInfo> PointsVector;

    struct FLineInfo
    {
        Vector3 From;
        Vector3 To;
        float Width;
        Color Color;
    };
    vector<FLineInfo> LinesVector;

    FViewPort* ViewPort;
};

} /* namespace tc */
