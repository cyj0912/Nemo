#pragma once

namespace tc
{

class FGLSLProgram;
class FDumbRendererPrivData;
class FRenderTarget;
class FRenderHelper;

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
    bool bHasRenderViewInfo;
    FRenderViewInfo RenderViewInfo;
    FDumbRendererPrivData* Pd;
    FRenderTarget* InitRenderTarget;
    FRenderHelper* RenderHelper;

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

    void SetInitialRenderTarget(FRenderTarget* target);
};

}
