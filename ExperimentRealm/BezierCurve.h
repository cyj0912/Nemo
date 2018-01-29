#pragma once

#include "RenderComponent.h"
#include "Shader.h"

#include "OpenGL.h"

#include <Foundation.h>
#include <Matrix4.h>

namespace tc
{

class FBezierCurveRenderComponentStaticData
{
public:
    FBezierCurveRenderComponentStaticData();

    ~FBezierCurveRenderComponentStaticData();

    static void RenderStaticInit();

    static void RenderStaticDestroy();

protected:
    static int UserCount;
    static bool bInitialized;
    static GLuint Buffers[1];
    static GLuint VertexArrays[1];
    static FGLSLProgram* Shader;
};

int FBezierCurveRenderComponentStaticData::UserCount = 0;
bool FBezierCurveRenderComponentStaticData::bInitialized = false;
GLuint FBezierCurveRenderComponentStaticData::Buffers[1];
GLuint FBezierCurveRenderComponentStaticData::VertexArrays[1];
FGLSLProgram* FBezierCurveRenderComponentStaticData::Shader;

template <typename TOwner>
class TBezierCurveRenderComponent : public IRenderComponent, public FBezierCurveRenderComponentStaticData
{
public:
    TBezierCurveRenderComponent() : ViewPort(nullptr) {}

    void RenderInit(FViewPort* rw) override
    {
        RenderStaticInit();
        ViewPort = rw;
    }

    void Render() override
    {
        auto modelMatrix = static_cast<TOwner*>(this)->GetTransformMatrix();
        Matrix4 mvp = ViewPort->GetViewProjectionMatrix() * modelMatrix;
        Shader->Enable();
        Shader->SetUniformMatrix4fv("uModelViewProjectionMatrix", mvp.Data(), 1, true);
        Shader->SetUniform3f("uColor", 1.0f, 1.0f, 0.0f);

        glPointSize(10.0f);
        glBindVertexArray(VertexArrays[0]);
        glDrawArrays(GL_POINTS, 0, 1);
        glBindVertexArray(0);
    }

    void RenderDestroy() override
    {
        RenderStaticDestroy();
    }

protected:
    FViewPort* ViewPort;
};

} /* namespace tc */
