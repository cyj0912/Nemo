#include "PrimitiveRenderer.h"
#include "Shader.h"
#include "RenderComponent.h"

namespace tc
{

enum
{
    VB_POINT_ORIGIN,
    VB_LINE,
    VB_COUNT
};

enum
{
    VA_POINT,
    VA_LINE,
    VA_COUNT
};

static bool bInitialized = false;
static GLuint Buffers[VB_COUNT];
static GLuint VertexArrays[VA_COUNT];
static FGLSLProgram* Shader;

void FPrimitiveRenderer::RenderStaticInit()
{
    assert(!bInitialized);
    bInitialized = true;

    glGenBuffers(VB_COUNT, Buffers);
    glGenVertexArrays(VA_COUNT, VertexArrays);

    const float origin[] = {0.0f, 0.0f, 0.0f};

    glBindVertexArray(VertexArrays[VA_POINT]);
    glBindBuffer(GL_ARRAY_BUFFER, Buffers[VB_POINT_ORIGIN]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(origin), origin, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);

    glBindVertexArray(VertexArrays[VA_LINE]);
    glBindBuffer(GL_ARRAY_BUFFER, Buffers[VB_LINE]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6, nullptr, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    Shader = FGLSLProgram::CreateFromFiles("fgizmocolor.vert", "fgizmocolor.frag");
}

void FPrimitiveRenderer::RenderStaticDestroy()
{
    assert(bInitialized);
    bInitialized = false;

    delete Shader;
    glDeleteBuffers(VB_COUNT, Buffers);
    glDeleteVertexArrays(VA_COUNT, VertexArrays);
}

void FPrimitiveRenderer::RenderInit(FViewPort* vp)
{
    ViewPort = vp;
}

void FPrimitiveRenderer::Render()
{
    Shader->Enable();
    for (const auto& pointInfo : PointsVector)
    {
        Matrix3x4 modelMatrix = Matrix3x4(pointInfo.Location, Quaternion::IDENTITY, Vector3::ONE);
        Matrix4 mvp = ViewPort->GetViewProjectionMatrix() * modelMatrix;
        Shader->SetUniformMatrix4fv("uModelViewProjectionMatrix", mvp.Data(), 1, true);
        Shader->SetUniform3fv("uColor", pointInfo.Color.Data(), 1);

        glPointSize(pointInfo.Size);
        glBindVertexArray(VertexArrays[0]);
        glDrawArrays(GL_POINTS, 0, 1);
        glBindVertexArray(0);
    }
    glPointSize(1.0f);

    for (const auto& lineInfo : LinesVector)
    {
        Shader->SetUniformMatrix4fv("uModelViewProjectionMatrix", ViewPort->GetViewProjectionMatrix().Data(), 1, true);
        Shader->SetUniform3fv("uColor", lineInfo.Color.Data(), 1);

        Vector3 ends[2];
        ends[0] = lineInfo.From;
        ends[1] = lineInfo.To;

        glBindBuffer(GL_ARRAY_BUFFER, Buffers[VB_LINE]);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * 6, ends[0].Data());

        //TODO: bug with glLineWidth
        glBindVertexArray(VertexArrays[VA_LINE]);
        glDrawArrays(GL_LINES, 0, 2);
        glBindVertexArray(0);
    }
}

void FPrimitiveRenderer::RenderDestroy()
{
}

} /* namespace tc */
