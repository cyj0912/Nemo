#include "OpenGLRHI.h"
#include "OpenGL.h"

namespace tc
{

static GLenum FillModeToGLEnum(ERasterizerFillMode fillMode)
{
    if (fillMode == ERasterizerFillMode::Point)
        return GL_POINT;
    else if (fillMode == ERasterizerFillMode::Wireframe)
        return GL_LINE;
    return GL_FILL;
}

static GLenum CullModeToGLEnum(ERasterizerCullMode cullMode)
{
    if (cullMode == ERasterizerCullMode::None)
        return GL_NONE;
    else if (cullMode == ERasterizerCullMode::CW)
        return GL_FRONT;
    else
        return GL_BACK;
}

class FGL3RasterizerState : public FRHIRasterizerState
{
public:
    GLenum FillMode;
    GLenum CullMode;
};

FRHIRasterizerStateRef FOpenGLRHI::CreateRasterizerState(const FRasterizerDesc& desc)
{
    auto* rastState = new FGL3RasterizerState();
    rastState->FillMode = FillModeToGLEnum(desc.FillMode);
    rastState->CullMode = CullModeToGLEnum(desc.CullMode);
    return FRHIRasterizerStateRef(rastState);
}

} /* namespace tc */
