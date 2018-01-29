#include "BezierCurve.h"

namespace tc
{

FBezierCurveRenderComponentStaticData::FBezierCurveRenderComponentStaticData()
{
    UserCount++;
}

FBezierCurveRenderComponentStaticData::~FBezierCurveRenderComponentStaticData()
{
    UserCount--;
}

void FBezierCurveRenderComponentStaticData::RenderStaticInit()
{
    //StaticInit preamble
    if (bInitialized)
        return;
    bInitialized = true;

    LOGDEBUG("FBezierCurveRenderComponentStaticData::RenderStaticInit called and ran\n");

    glGenBuffers(1, Buffers);
    glGenVertexArrays(1, VertexArrays);
    glBindVertexArray(VertexArrays[0]);

    const float origin[] = {0.0f, 0.0f, 0.0f};

    glBindBuffer(GL_ARRAY_BUFFER, Buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, (1024) * 3 * sizeof(float), nullptr, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    Shader = FGLSLProgram::CreateFromFiles("fgizmocolor.vert", "fgizmocolor.frag");
}

void FBezierCurveRenderComponentStaticData::RenderStaticDestroy()
{
    //StaticDestroy preamble
    if (!bInitialized || UserCount > 0)
        return;

    delete Shader;
    glDeleteBuffers(1, Buffers);
    glDeleteVertexArrays(1, VertexArrays);
}

} /* namespace tc */
