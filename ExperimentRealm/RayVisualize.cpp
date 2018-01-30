#include "RayVisualize.h"
#include "EditorMaster.h"
#include "imgui/imgui.h"

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
    if (!bIsEnabled)
        return false;

    if (evt.button == EMouseButton::Left)
    {
        auto ray = EditorMaster->GetViewPort()->GetRayTo(evt.x, evt.y);
        auto rayDisp = new FRayDisplay(ray);
        rayDisp->RenderInit(EditorMaster->GetViewPort());
        RayDisplayVector.push_back(rayDisp);
    }
    return false;
}

void FRayVisualizer::ImGuiUpdate()
{
    if (ImGui::TreeNode("Ray Visualizer"))
    {
        ImGui::Text("Total Rays: %d", (int)RayDisplayVector.size());
        ImGui::Checkbox("Enabled", &bIsEnabled);
        if (ImGui::Button("Remove All Rays"))
        {
            RenderDestroy();
            for (auto* ray : RayDisplayVector)
                delete ray;
            RayDisplayVector.clear();
        }
        if (!RayDisplayVector.empty())
        {
            auto* lastRay = RayDisplayVector[RayDisplayVector.size() - 1];
            const Ray& ray = lastRay->GetRay();
            ImGui::Text("Last Ray");
            ImGui::Text("\tOrigin: <%s>", ray.Origin.ToString().c_str());
            ImGui::Text("\tDirection: <%s>",  ray.Direction.ToString().c_str());
        }
        ImGui::TreePop();
    }
}

void FRayVisualizer::Render()
{
    for (auto* ray : RayDisplayVector)
        ray->Render();
}

void FRayVisualizer::RenderDestroy()
{
    for (auto* ray : RayDisplayVector)
        ray->RenderDestroy();
}

} /* namespace tc */
