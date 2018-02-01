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

int FBezierCurveRenderComponentStaticData::UserCount = 0;
bool FBezierCurveRenderComponentStaticData::bInitialized = false;
GLuint FBezierCurveRenderComponentStaticData::Buffers[1];
GLuint FBezierCurveRenderComponentStaticData::VertexArrays[1];
FGLSLProgram* FBezierCurveRenderComponentStaticData::Shader;

FBezierCurveControlPointPrimitive::FBezierCurveControlPointPrimitive()
{
    FrontPoint.GetPosition() = Vector3(1.0f, 1.0f, 0.0f);
    MiddlePoint.GetPosition() = Vector3(0.0f, 0.0f, 0.0f);
    BackPoint.GetPosition() = Vector3(-1.0f, -1.0f, 0.0f);
}

const char* FBezierCurveControlPointPrimitive::GetTypeNameInString() const
{
    static const char* name = "FBezierCurveControlPointPrimitive";
    return name;
}

size_t FBezierCurveControlPointPrimitive::CountSubEntities() const
{
    return 3;
}

FBaseEntity* FBezierCurveControlPointPrimitive::GetSubEntity(size_t index)
{
    assert(index < 3);
    switch(index)
    {
    case 0:
        return &BackPoint;
    case 1:
        return &MiddlePoint;
    case 2:
        return &FrontPoint;
    default:
        break;
    }
    return nullptr;
}

void FBezierCurveControlPointPrimitive::RenderInit(FViewPort* rw)
{
    FrontPoint.RenderInit(rw);
    MiddlePoint.RenderInit(rw);
    BackPoint.RenderInit(rw);
}

void FBezierCurveControlPointPrimitive::Render()
{
    FrontPoint.Render();
    MiddlePoint.Render();
    BackPoint.Render();
    GEditorMaster->GetPrimitiveRenderer()->DrawLine(BackPoint.GetPosition(), MiddlePoint.GetPosition(), 1.0f, Color::GREEN);
    GEditorMaster->GetPrimitiveRenderer()->DrawLine(MiddlePoint.GetPosition(), FrontPoint.GetPosition(), 1.0f, Color::RED);
}

void FBezierCurveControlPointPrimitive::RenderDestroy()
{
    FrontPoint.RenderDestroy();
    MiddlePoint.RenderDestroy();
    BackPoint.RenderDestroy();
}

} /* namespace tc */
