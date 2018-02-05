#pragma once
#include "RenderComponent.h"
#include "Shader.h"
#include "InputHandler.h"

namespace tc
{

class FEditorMaster;

class FRayRenderComponentStaticData
{
public:
    FRayRenderComponentStaticData();

    ~FRayRenderComponentStaticData();

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
class TRayRenderComponent : public IRenderComponent, public FRayRenderComponentStaticData
{
public:
    TRayRenderComponent() : RenderWorld(nullptr) {}

    void RenderInit(FViewPort* rw) override
    {
        RenderStaticInit();
        RenderWorld = rw;
    }

    void Render() override
    {
        Shader->Enable();
        Shader->SetUniformMatrix4fv("uModelViewProjectionMatrix",
                                    RenderWorld->GetViewProjectionMatrix().Data(), 1, true);
        Shader->SetUniform3f("uColor", 0.0, 1.0, 1.0);

        const auto& ray = static_cast<TOwner*>(this)->GetRay();

        static_assert(sizeof(ray) == sizeof(float) * 6, "Ray is not 6 floats");
        Vector3 ends[2];
        ends[0] = ray.Origin;
        ends[1] = ray.Direction * 50.0f + ray.Origin;

        glBindBuffer(GL_ARRAY_BUFFER, Buffers[0]);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * 6, ends[0].Data());

        glBindVertexArray(VertexArrays[0]);
        glDrawArrays(GL_LINES, 0, 2);
        glBindVertexArray(0);
    }

    void RenderDestroy() override
    {
        RenderStaticDestroy();
    }

protected:
    FViewPort* RenderWorld;
};

class FRayDisplay : public TRayRenderComponent<FRayDisplay>
{
public:
    explicit FRayDisplay(const Ray& r) : TheRay(r) {}

    const Ray& GetRay()
    {
        return TheRay;
    }

private:
    Ray TheRay;
};

class FRayVisualizer : public IInputHandler
{
public:
    explicit FRayVisualizer(FEditorMaster* EditorMaster) : EditorMaster(EditorMaster), bIsEnabled(false) {}

    bool MousePressed(const FMouseButtonEvent& evt) override;

    void ImGuiUpdate();

    void Render();

    void RenderDestroy();

private:
    FEditorMaster* EditorMaster;
    vector<FRayDisplay*> RayDisplayVector;
    bool bIsEnabled;
};

} /* namespace tc */
