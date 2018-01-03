#pragma once

namespace tc
{

class FGLSLProgram;

struct FRenderViewInfo
{
    unsigned int Width;
    unsigned int Height;
    float AspectRatio;

    void SetAspectRatioFromWidthHeight()
    {
        AspectRatio = (float)Width / (float)Height;
    }
};

class FDumbRenderer
{
    FRenderViewInfo RenderViewInfo;

public:
    FDumbRenderer();

    //Initialize the renderer, OpenGL must be usable at this point
    bool Init();
    bool Shutdown();
    void RenderFrame();

    const FRenderViewInfo &GetRenderViewInfo() const
    {
        return RenderViewInfo;
    }

    void SetRenderViewInfo(const FRenderViewInfo &value);
};

}
