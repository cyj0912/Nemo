#include "RayVisualize.h"
#include "EditorMaster.h"

namespace tc
{

FRayRenderComponentStaticData::FRayRenderComponentStaticData()
{
    UserCount++;
}

FRayRenderComponentStaticData::~FRayRenderComponentStaticData()
{
    UserCount--;
}

void FRayRenderComponentStaticData::RenderStaticInit()
{
    if (bInitialized)
        return;
    bInitialized = true;

    LOGDEBUG("FRayRenderComponentStaticData::RenderStaticInit called and ran\n");

    glGenBuffers(1, Buffers);
    glGenVertexArrays(1, VertexArrays);
    glBindVertexArray(VertexArrays[0]);

    glBindBuffer(GL_ARRAY_BUFFER, Buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6, nullptr, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    Shader = FGLSLProgram::CreateFromFiles("fgizmocolor.vert", "fgizmocolor.frag");
}

void FRayRenderComponentStaticData::RenderStaticDestroy()
{
    if (!bInitialized || UserCount > 0)
        return;

    LOGDEBUG("FRayRenderComponentStaticData::RenderStaticDestroy called and ran\n");

    delete Shader;
    glDeleteBuffers(1, Buffers);
    glDeleteVertexArrays(1, VertexArrays);
}

int FRayRenderComponentStaticData::UserCount = 0;
bool FRayRenderComponentStaticData::bInitialized = false;
GLuint FRayRenderComponentStaticData::Buffers[1];
GLuint FRayRenderComponentStaticData::VertexArrays[1];
FGLSLProgram* FRayRenderComponentStaticData::Shader;

bool FRayVisualizer::MousePressed(const FMouseButtonEvent& evt)
{
    //auto ray = CurrentCamera->GetRayTo(evt.x, evt.y);
    //auto rayVisualizer = new FRayVisualizer(ray);
    //rayVisualizer->RenderInit(renderWorld);

    return IInputHandler::MousePressed(evt);
}

} /* namespace tc */
