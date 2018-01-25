#pragma once

namespace tc
{

class FRenderTarget
{
    unsigned int Framebuffer;
public:
    FRenderTarget();
    explicit FRenderTarget(unsigned int fbo) : Framebuffer(fbo)
    {
    }
};

}
