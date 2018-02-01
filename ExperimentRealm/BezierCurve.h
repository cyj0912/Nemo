#pragma once

#include "RenderComponent.h"
#include "Shader.h"
#include "EntityLibrary.h"

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

class FBezierCurveControlPointPrimitive : public FBaseEntity, public IRenderComponent
{
public:
    FBezierCurveControlPointPrimitive();

    const char* GetTypeNameInString() const override;

    size_t CountSubEntities() const override;

    FBaseEntity* GetSubEntity(size_t index) override;

    void RenderInit(FViewPort* rw) override;

    void Render() override;

    void RenderDestroy() override;

private:
    FPointPrimitive FrontPoint;
    FPointPrimitive MiddlePoint;
    FPointPrimitive BackPoint;
};

} /* namespace tc */
