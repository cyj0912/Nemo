#include "Shader.h"
#include "EntityLibrary.h"
#include "InputHandler.h"
#include "InputEventFromSDL2.h"
#include "RenderComponent.h"
#include "EditorMaster.h"
#include "RayVisualize.h"
#include "PrimitiveRenderer.h"
#include "EntityManager.h"
#include "Gizmo.h"
#include "BezierCurve.h"

#include <SDL.h>
#include <ResourceManager.h>
#include <Image.h>

#include <Timeline.h>

#include <Camera.h>
#include <SceneNode.h>

#include <nanovg.h>
#define NANOVG_GL3_IMPLEMENTATION
#include <nanovg_gl.h>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl_gl3.h"

using namespace tc;

namespace tc
{

class FCameraInputHandler;
class FGrid;

FEditorMaster* GEditorMaster;

}

FMetaInputHandler* metaInputHandler;
FHighResolutionClock GTimeline;
FViewPort* renderWorld;
FCameraWithPivot* cameraWithPivot;
FCameraInputHandler* cameraInputHandler;
FGrid* mainGrid;
FEditorMaster* editorMaster;
NVGcontext* vg;
long long frameCount = 0;
bool bShowStyleEditor;

namespace tc
{

class FCameraInputHandler : public IInputHandler
{
    FCameraWithPivot* Camera;
    bool AcceptLMBRotate;
    bool MouseCurrentlyOrbit;
    bool MouseInFpsView;
    int MouseLastX, MouseLastY;
public:
    explicit FCameraInputHandler(FCameraWithPivot* Camera) : Camera(Camera), AcceptLMBRotate(false)
    , MouseCurrentlyOrbit(false), MouseInFpsView(false) {}

    bool KeyPressed(const FKeyboardEvent& evt) override
    {
        if (evt.keysym.sym == EKeyCode::Rgui || evt.keysym.sym == EKeyCode::Lalt)
            AcceptLMBRotate = true;
        return false;
    }

    bool KeyReleased(const FKeyboardEvent& evt) override
    {
        if (evt.keysym.sym == EKeyCode::Rgui || evt.keysym.sym == EKeyCode::Lalt)
            AcceptLMBRotate = false;
        return false;
    }

    bool MouseMoved(const FMouseMotionEvent& evt) override
    {
        if (MouseCurrentlyOrbit)
        {
            float deltaX = evt.x - MouseLastX;
            float deltaY = evt.y - MouseLastY;

            Camera->GetCameraPivot().Yaw(-deltaX * 0.25f, TS_WORLD);
            Camera->GetCameraPivot().Pitch(-deltaY * 0.25f);

            MouseLastX = evt.x;
            MouseLastY = evt.y;
            return true;
        }
        else if (MouseInFpsView)
        {
            float deltaX = evt.x - MouseLastX;
            float deltaY = evt.y - MouseLastY;

            //Camera->GetCameraTransform().Yaw(-deltaX * 0.25f, TS_WORLD);
            Camera->GetCameraTransform().Pitch(-deltaY * 0.25f);

            MouseLastX = evt.x;
            MouseLastY = evt.y;
        }
        return false;
    }

    bool MousePressed(const FMouseButtonEvent& evt) override
    {
        if (evt.button == EMouseButton::Left && AcceptLMBRotate)
        {
            MouseCurrentlyOrbit = true;
            MouseLastX = evt.x;
            MouseLastY = evt.y;
        }
        else if (evt.button == EMouseButton::Middle)
        {
            MouseCurrentlyOrbit = true;
            MouseLastX = evt.x;
            MouseLastY = evt.y;
        }
        else if (evt.button == EMouseButton::Right)
        {
            MouseInFpsView = true;
            MouseLastX = evt.x;
            MouseLastY = evt.y;
        }
        return false;
    }

    bool MouseReleased(const FMouseButtonEvent& evt) override
    {
        if (evt.button == EMouseButton::Left || evt.button == EMouseButton::Middle)
            MouseCurrentlyOrbit = false;
        else if (evt.button == EMouseButton::Right)
            MouseInFpsView = false;
        return false;
    }

    bool MouseWheelRolled(const FMouseWheelEvent& evt) override
    {
        return IInputHandler::MouseWheelRolled(evt);
    }
};

template <typename TOwner>
class TBoxRenderComponent : public IRenderComponent
{
public:
    TBoxRenderComponent() : RenderWorld(nullptr), Shader(nullptr) {}

    void RenderInit(FViewPort* rw) override
    {
        RenderWorld = rw;

        float boxVertices[] = {
                -1.0f, -1.0f, 1.0f, -1.0f, 0.0f, 0.0f,
                -1.0f, 1.0f, -1.0f, -1.0f, 0.0f, 0.0f,
                -1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f,
                -1.0f, 1.0f, 1.0f, 0.0f, 1.0f, -0.0f,
                1.0f, 1.0f, -1.0f, 0.0f, 1.0f, -0.0f,
                -1.0f, 1.0f, -1.0f, 0.0f, 1.0f, -0.0f,
                1.0f, 1.0f, 1.0f, 1.0f, 0.0f, -0.0f,
                1.0f, -1.0f, -1.0f, 1.0f, 0.0f, -0.0f,
                1.0f, 1.0f, -1.0f, 1.0f, 0.0f, -0.0f,
                1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f,
                -1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f,
                1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f,
                1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f,
                -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f,
                -1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f,
                -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
                1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
                1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
                -1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f,
                1.0f, 1.0f, 1.0f, 0.0f, 1.0f, -0.0f,
                1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
                -1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f,
                1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f,
                -1.0f, -1.0f, 1.0f, 0.0f, -0.0f, 1.0f,
        };

        unsigned short boxIndices[] = {
                0, 1, 2,
                3, 4, 5,
                6, 7, 8,
                9, 10, 11,
                12, 13, 14,
                15, 16, 17,
                0, 18, 1,
                3, 19, 4,
                6, 20, 7,
                9, 21, 10,
                12, 22, 13,
                15, 23, 16
        };

        glGenBuffers(2, Buffers);
        glGenVertexArrays(1, VertexArrays);
        glBindVertexArray(VertexArrays[0]);

        glBindBuffer(GL_ARRAY_BUFFER, Buffers[0]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(boxVertices), boxVertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, nullptr);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)(sizeof(float) * 3));
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Buffers[1]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(boxIndices), boxIndices, GL_STATIC_DRAW);

        glBindVertexArray(0);

        Shader = FGLSLProgram::CreateFromFiles("fbox.vert", "fbox.frag");
    }

    void Render() override
    {
        Matrix4 modelMatrix = static_cast<TOwner*>(this)->GetTransformMatrix().ToMatrix4();
        Matrix4 mvp = RenderWorld->GetProjectionMatrix() * RenderWorld->GetViewMatrix() * modelMatrix;
        Matrix3 viewNoTranslate = RenderWorld->GetViewMatrix().ToMatrix3();
        Matrix3 normalTransform = viewNoTranslate * modelMatrix.ToMatrix3();
        normalTransform = normalTransform.Inverse();
        normalTransform = normalTransform.Transpose();

        Vector3 eyeDirLight = viewNoTranslate * Vector3(-0.90f, 0.04f, 0.43f);

        Shader->Enable();
        Shader->SetUniformMatrix4fv("uModelViewProjectionMatrix", mvp.Data(), 1, true);
        Shader->SetUniformMatrix3fv("uNormalTransform", normalTransform.Data(), 1, true);
        Shader->SetUniform3f("uEyeDirLight", eyeDirLight.x_, eyeDirLight.y_, eyeDirLight.z_);

        glBindVertexArray(VertexArrays[0]);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Buffers[1]);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);
        glBindVertexArray(0);
    }

    void RenderDestroy() override
    {
        delete Shader;
        glDeleteBuffers(2, Buffers);
        glDeleteVertexArrays(1, VertexArrays);
    }

private:
    FViewPort* RenderWorld;
    GLuint Buffers[2]{}, VertexArrays[1]{};
    FGLSLProgram* Shader;
};

class FBoxPrimitive : public FBaseEntity,
                      public FTransformComponent,
                      public TBoxRenderComponent<FBoxPrimitive>,
                      public TBoxRayIntersectComponent<FBoxPrimitive>
{
public:
    const char* GetTypeNameInString() const override
    {
        static const char* name = "FBoxPrimitive";
        return name;
    }
};

class FGrid
{
    // Component: Render
    FViewPort* RenderWorld;
    GLuint Buffers[1]{}, VertexArrays[1]{};
    FGLSLProgram* Shader;
    int NumLines;

    // Data (Shared Data?) (Or What?)
    Vector3 OriginPoint;

public:
    FGrid() : RenderWorld(nullptr), Shader(nullptr), NumLines(0)
    {
    }

    void RenderInit(FViewPort* rw)
    {
        RenderWorld = rw;

        //Generate grid lines
        const int xHalfBoxCount = 10, yHalfBoxCount = 10;
        const float xBoxLength = 1.0f, yBoxLength = 1.0f;
        const float xStart = -xBoxLength * xHalfBoxCount, xEnd = -xStart;
        const float yStart = -yBoxLength * yHalfBoxCount, yEnd = -yStart;

        vector<float> lines;
        NumLines = xHalfBoxCount * 2 + 1 + yHalfBoxCount * 2 + 1;

        auto pushVertex3f = [&lines](float x, float y, float z)
        {
            lines.push_back(x);
            lines.push_back(y);
            lines.push_back(z);
        };

        for (int i = 0; i <= 2 * xHalfBoxCount; i++)
        {
            pushVertex3f(xStart + xBoxLength * i, 0.0f, yStart);
            pushVertex3f(xStart + xBoxLength * i, 0.0f, yEnd);
        }
        for (int j = 0; j <= 2 * yHalfBoxCount; j++)
        {
            pushVertex3f(xStart, 0.0f, yStart + yBoxLength * j);
            pushVertex3f(xEnd, 0.0f, yStart + yBoxLength * j);
        }

        glGenBuffers(1, Buffers);
        glGenVertexArrays(1, VertexArrays);

        glBindVertexArray(VertexArrays[0]);

        glBindBuffer(GL_ARRAY_BUFFER, Buffers[0]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * lines.size(), lines.data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        glBindVertexArray(0);

        Shader = FGLSLProgram::CreateFromFiles("fgrid.vert", "fgrid.frag");
    }

    void Render()
    {
        Matrix4 vpMatrix = RenderWorld->GetProjectionMatrix() * RenderWorld->GetViewMatrix();

        Shader->Enable();

        {
            Matrix3x4 modelMatrix = Matrix3x4(OriginPoint, Quaternion::IDENTITY, Vector3::ONE);
            Matrix4 mvp = vpMatrix * modelMatrix;

            Shader->SetUniformMatrix4fv("uModelViewProjectionMatrix", mvp.Data(), 1, true);

            glBindVertexArray(VertexArrays[0]);
            glDrawArrays(GL_LINES, 0, NumLines * 6);
            glBindVertexArray(0);
        }
    }

    void RenderDestroy()
    {
        delete Shader;
        glDeleteBuffers(1, Buffers);
        glDeleteVertexArrays(1, VertexArrays);
    }
};

FEditorMaster::FEditorMaster() : bWireframe(false), SelectedEntity(nullptr),TranslateGizmo(nullptr),
                  TranslateGizmoInputHandler(nullptr), PointTranslateGizmo(nullptr),
                  PointTranslateGizmoInputHandler(nullptr), bToQuit(false)
{
    EntityManager = new FEntityManager();
    RayVisualizer = new FRayVisualizer(this);
    PrimitiveRenderer = new FPrimitiveRenderer();
}

FEditorMaster::~FEditorMaster()
{
    delete RayVisualizer;
    delete PrimitiveRenderer;
    delete EntityManager;
}


bool FEditorMaster::KeyPressed(const FKeyboardEvent& evt)
{
    if (evt.keysym.sym == EKeyCode::Z)
    {
        ToggleWireframe();
        return true;
    }
    return false;
}

bool FEditorMaster::KeyReleased(const FKeyboardEvent& evt)
{
    return false;
}

bool FEditorMaster::MousePressed(const FMouseButtonEvent& evt)
{
    if (RayVisualizer->MousePressed(evt))
        return true;

    if (evt.button == SDL_BUTTON_LEFT)
    {
        auto worldRay = GetViewPort()->GetRayTo(evt.x, evt.y);

        //The closer entity wins if contested
        FBaseEntity* NewSelectedEntity = nullptr;
        float minHitDistance = M_INFINITY;
        for (auto* entity : EntityManager->GetComponents<IRayIntersectComponent>())
        {
            float distance = entity->RayHitDistance(worldRay);
            if(distance < minHitDistance)
            {
                NewSelectedEntity = dynamic_cast<FBaseEntity*>(entity);
                minHitDistance = distance;
            }
        }

        if (NewSelectedEntity)
        {
            if (NewSelectedEntity != SelectedEntity)
            {
                RemoveGizmos();
                CreateGizmoFor(NewSelectedEntity);
                SelectedEntity = NewSelectedEntity;
                return true;
            }

            //Selected the original selected, nothing happens
            return false;
        }

        //When the new selection is nothing
        if (NewSelectedEntity != SelectedEntity)
        {
            RemoveGizmos();
            SelectedEntity = NewSelectedEntity;
        }
    }
    return false;
}

void FEditorMaster::CreateGizmoFor(FBaseEntity* entity)
{
    if (dynamic_cast<FBoxPrimitive*>(entity))
    {
        auto* box = dynamic_cast<FBoxPrimitive*>(entity);
        TranslateGizmo = new FTranslateGizmo(&box->GetTransformNode());
        TranslateGizmo->RenderInit(renderWorld);
        TranslateGizmoInputHandler = new FTranslateGizmoInputHandler(TranslateGizmo, GetViewPort());
        metaInputHandler->InsertFront(TranslateGizmoInputHandler);
    }
    else if (dynamic_cast<FPointPrimitive*>(entity))
    {
        auto* point = dynamic_cast<FPointPrimitive*>(entity);
        PointTranslateGizmo = new FPointTranslateGizmo(&point->GetPosition());
        PointTranslateGizmo->RenderInit(renderWorld);
        PointTranslateGizmoInputHandler = new FPointTranslateGizmoInputHandler(PointTranslateGizmo, GetViewPort());
        metaInputHandler->InsertFront(PointTranslateGizmoInputHandler);
    }
}

void FEditorMaster::RemoveGizmos()
{
    delete TranslateGizmoInputHandler;
    TranslateGizmoInputHandler = nullptr;

    if (TranslateGizmo)
        TranslateGizmo->RenderDestroy();
    delete TranslateGizmo;
    TranslateGizmo = nullptr;

    delete PointTranslateGizmoInputHandler;
    PointTranslateGizmoInputHandler = nullptr;

    if (PointTranslateGizmo)
        PointTranslateGizmo->RenderDestroy();
    delete PointTranslateGizmo;
    PointTranslateGizmo = nullptr;
}

void FEditorMaster::ToggleWireframe()
{
    bWireframe = !bWireframe;
}

bool FEditorMaster::IsWireframe() const
{
    return bWireframe;
}

void FEditorMaster::ImGuiUpdate()
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Exit"))
                SetQuitting(true);
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit"))
        {
            if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
            if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
            ImGui::Separator();
            if (ImGui::MenuItem("Cut", "CTRL+X")) {}
            if (ImGui::MenuItem("Copy", "CTRL+C")) {}
            if (ImGui::MenuItem("Paste", "CTRL+V")) {}
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

    ImGui::Begin("Primitives");
    ImGui::Text("Current selection:");
    if (SelectedEntity)
        ImGui::Text("%s", SelectedEntity->GetTypeNameInString());
    else
        ImGui::Text("<None>");
    ImGui::Text("You can add primitives with this panel");
    if (ImGui::Button("Box"))
    {
        auto* box = new FBoxPrimitive();
        box->RenderInit(renderWorld);
        EntityManager->RegisterEntity(box);
        RenderComponentListHead.Insert(box);
    }
    if (ImGui::Button("Point"))
    {
        auto* point = new FPointPrimitive();
        point->RenderInit(renderWorld);
        EntityManager->RegisterEntity(point);
        RenderComponentListHead.Insert(point);
    }
    if (ImGui::Button("Bezier Control Point"))
    {
        auto* primitive = new FBezierCurveControlPointPrimitive();
        primitive->RenderInit(renderWorld);
        EntityManager->RegisterEntity(primitive);
        RenderComponentListHead.Insert(primitive);
    }
    ImGui::End();

    RayVisualizer->ImGuiUpdate();
}

void FEditorMaster::RenderInit()
{
    PrimitiveRenderer->RenderInit(GetViewPort());
}

void FEditorMaster::Render()
{
    //TODO: Rethink this
    PrimitiveRenderer->BeginFrame();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    auto* item = RenderComponentListHead.NextRenderComponent;
    while (item)
    {
        item->Render();
        item = item->NextRenderComponent;
    }

    RayVisualizer->Render();

    PrimitiveRenderer->Render();

    glDisable(GL_DEPTH_TEST);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    if (TranslateGizmo)
        TranslateGizmo->Render();

    if (PointTranslateGizmo)
        PointTranslateGizmo->Render();
}

void FEditorMaster::RenderDestroy()
{
    RemoveGizmos();

    auto* item = RenderComponentListHead.NextRenderComponent;
    while (item)
    {
        item->RenderDestroy();
        item = item->NextRenderComponent;
    }

    RayVisualizer->RenderDestroy();

    PrimitiveRenderer->RenderDestroy();
}

bool FEditorMaster::IsQuitting() const
{
    return bToQuit;
}

void FEditorMaster::SetQuitting(bool v)
{
    bToQuit = v;
}

FViewPort* FEditorMaster::GetViewPort() const
{
    return renderWorld;
}

void FEditorMaster::InsertRenderAndInit(IRenderComponent* comp)
{
    comp->RenderInit(GetViewPort());
    RenderComponentListHead.Insert(comp);
}

}

//Testing area
class FSkyboxRenderData
{
public:
    static void RenderStaticInit()
    {
        // Skybox geometry
        float boxVertices[] = {
                -1.0f, -1.0f, 1.0f,
                -1.0f, 1.0f, -1.0f,
                -1.0f, -1.0f, -1.0f,
                -1.0f, 1.0f, 1.0f,
                1.0f, 1.0f, -1.0f,
                -1.0f, 1.0f, -1.0f,
                1.0f, 1.0f, 1.0f,
                1.0f, -1.0f, -1.0f,
                1.0f, 1.0f, -1.0f,
                1.0f, -1.0f, 1.0f,
                -1.0f, -1.0f, -1.0f,
                1.0f, -1.0f, -1.0f,
                1.0f, 1.0f, -1.0f,
                -1.0f, -1.0f, -1.0f,
                -1.0f, 1.0f, -1.0f,
                -1.0f, 1.0f, 1.0f,
                1.0f, -1.0f, 1.0f,
                1.0f, 1.0f, 1.0f,
                -1.0f, 1.0f, 1.0f,
                1.0f, 1.0f, 1.0f,
                1.0f, -1.0f, 1.0f,
                -1.0f, -1.0f, 1.0f,
                1.0f, -1.0f, -1.0f,
                -1.0f, -1.0f, 1.0f,
        };

        unsigned short boxIndices[] = {
                0, 1, 2,
                3, 4, 5,
                6, 7, 8,
                9, 10, 11,
                12, 13, 14,
                15, 16, 17,
                0, 18, 1,
                3, 19, 4,
                6, 20, 7,
                9, 21, 10,
                12, 22, 13,
                15, 23, 16
        };

        glGenBuffers(2, Buffers);
        glGenVertexArrays(1, VertexArrays);
        glBindVertexArray(VertexArrays[0]);

        glBindBuffer(GL_ARRAY_BUFFER, Buffers[0]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(boxVertices), boxVertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Buffers[1]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(boxIndices), boxIndices, GL_STATIC_DRAW);

        glBindVertexArray(0);

        ShaderSkybox = FGLSLProgram::CreateFromFiles("fskybox.vert", "fskybox.frag");
    }

    static void RenderStaticDestroy()
    {
        glDeleteBuffers(2, Buffers);
        glDeleteVertexArrays(1, VertexArrays);
        delete ShaderSkybox;
    }

    void PrepareSkybox()
    {
        // Cubemapping
        RImage* cubemaps[6];
        cubemaps[0] = new RImage("sunset_posx.png");
        cubemaps[1] = new RImage("sunset_negx.png");
        cubemaps[2] = new RImage("sunset_posy.png");
        cubemaps[3] = new RImage("sunset_negy.png");
        cubemaps[4] = new RImage("sunset_posz.png");
        cubemaps[5] = new RImage("sunset_negz.png");

        int texWidth = cubemaps[0]->GetWidth();
        int texHeight = cubemaps[0]->GetHeight();

        glGenTextures(1, &CubemapTex);
        glBindTexture(GL_TEXTURE_CUBE_MAP, CubemapTex);
        for (int i = 0; i < 6; i++)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, texWidth, texHeight, 0,
                         GL_RGB, GL_UNSIGNED_BYTE, cubemaps[i]->GetData());
            delete cubemaps[i];
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    }

    void DrawSkybox(const Matrix4& mvpMatrix)
    {
        ShaderSkybox->Enable();

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, CubemapTex);

        ShaderSkybox->SetUniform1i("uCubemap", 0);
        ShaderSkybox->SetUniformMatrix4fv("uModelViewProjectionMatrix", mvpMatrix.Data(), 1, true);

        glBindVertexArray(VertexArrays[0]);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Buffers[1]);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);
    }

    void DeleteSkybox()
    {
        glDeleteTextures(1, &CubemapTex);
    }

private:
    static FGLSLProgram* ShaderSkybox;
    static GLuint Buffers[2], VertexArrays[1];
    GLuint CubemapTex = 0;
};

FGLSLProgram* FSkyboxRenderData::ShaderSkybox = nullptr;
GLuint FSkyboxRenderData::Buffers[2];
GLuint FSkyboxRenderData::VertexArrays[1];

class FSkyboxRenderComponent : public IRenderComponent, public FSkyboxRenderData
{
public:
    void RenderInit(FViewPort* rw) override
    {
        ViewPort = rw;
        PrepareSkybox();
    }

    void Render() override
    {
        float zFar = ViewPort->GetCamera()->GetCamera().GetZFar();
        zFar /= 1.8f; //A bit over sqrt(3)
        Matrix3x4 enlargeSkybox = Matrix3x4(Vector3::ZERO, Quaternion::IDENTITY, Vector3(zFar, zFar, zFar));
        Matrix4 viewMat = ViewPort->GetViewMatrix();
        viewMat.SetTranslation(Vector3::ZERO);
        Matrix4 mvp = ViewPort->GetProjectionMatrix() * viewMat * enlargeSkybox;
        DrawSkybox(mvp);
    }

    void RenderDestroy() override
    {
        DeleteSkybox();
    }

private:
    FViewPort* ViewPort;
};

FSkyboxRenderComponent* testSkybox;

void myGlSetup()
{
    GTimeline.Init();
    GEditorMaster = editorMaster = new FEditorMaster();
    metaInputHandler = new FMetaInputHandler();
    metaInputHandler->Insert(editorMaster);

    cameraWithPivot = new FCameraWithPivot();
    cameraInputHandler = new FCameraInputHandler(cameraWithPivot);
    metaInputHandler->Insert(cameraInputHandler);

    renderWorld = new FViewPort(cameraWithPivot, 1600, 900);
    cameraWithPivot->GetCameraTransform().Translate(0, 3, 6.18);
    cameraWithPivot->GetCameraTransform().LookAt(Vector3::ZERO);

    FRayDisplay::RenderStaticInit();
    FSkyboxRenderComponent::RenderStaticInit();
    FPrimitiveRenderer::RenderStaticInit();

    mainGrid = new FGrid();
    mainGrid->RenderInit(renderWorld);

    testSkybox = new FSkyboxRenderComponent();
    testSkybox->RenderInit(renderWorld);

    editorMaster->RenderInit();

    vg = nvgCreateGL3(NVG_ANTIALIAS | NVG_STENCIL_STROKES | NVG_DEBUG);

    int fontResult;
    fontResult = nvgCreateFont(vg, "sans", GResourceManager.FindFile("Roboto-Regular.ttf").c_str());
    if (fontResult == -1)
        LOGWARN("sans wasn't successfully created\n");
    fontResult = nvgCreateFont(vg, "sans-bold", GResourceManager.FindFile("Roboto-Bold.ttf").c_str());
    if (fontResult == -1)
        LOGWARN("sans-bold wasn't successfully created\n");

    ImGui_ImplSdlGL3_Init();
    ImGui::StyleColorsDark();
    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->AddFontFromFileTTF(GResourceManager.FindFile("Roboto-Light.ttf").c_str(), 20.0f);
    io.Fonts->AddFontDefault();
}

//Update and render, returns true if quitting
bool myUpdateAndRender(SDL_Window* window)
{
    glClearColor(0, 0, 0, 0);
    glClearDepth(1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    if (editorMaster->IsWireframe())
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    testSkybox->Render();

    mainGrid->Render();

    editorMaster->Render();

    frameCount++;
    double fps = (double)frameCount * 1000.0 / (double)GTimeline.NowMilliSec();
    //UI
    float textX = 5, textY = 50;
    char fpsText[128];

    sprintf(fpsText, "clk: %d, frm: %lld, fps: %f", GTimeline.NowMilliSec(), frameCount, fps);

    nvgBeginFrame(vg, 1600, 900, 1.0f);
    nvgFontSize(vg, 32.0f);
    nvgFontFace(vg, "sans");
    nvgTextAlign(vg, NVG_ALIGN_LEFT | NVG_ALIGN_TOP);

    nvgFontBlur(vg, 2);
    nvgFillColor(vg, nvgRGBA(0, 0, 0, 128));
    nvgText(vg, textX, textY + 1, fpsText, nullptr);

    nvgFontBlur(vg, 0);
    nvgFillColor(vg, nvgRGBA(220, 220, 220, 160));
    nvgText(vg, textX, textY, fpsText, nullptr);
    nvgEndFrame(vg);

    ImGui_ImplSdlGL3_NewFrame(window);
    {
        ImGui::Text("Welcome to Toby's Workbench!");
        if (ImGui::Button("Toggle Wireframe"))
            editorMaster->ToggleWireframe();
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        if (ImGui::Button("Show Style Editor"))
            bShowStyleEditor = !bShowStyleEditor;
    }

    if (bShowStyleEditor)
    {
        ImGui::Begin("Style Editor", &bShowStyleEditor);
        ImGui::ShowStyleEditor();
        ImGui::End();
    }

    editorMaster->ImGuiUpdate();

    ImGui::Render();

    return editorMaster->IsQuitting();
}

void myGlCleanup()
{
    editorMaster->RenderDestroy();
    ImGui_ImplSdlGL3_Shutdown();
    nvgDeleteGL3(vg);
    mainGrid->RenderDestroy();
    testSkybox->RenderDestroy();
    delete testSkybox;

    delete mainGrid;
    delete renderWorld;
    delete cameraWithPivot;
    delete cameraInputHandler;
    delete editorMaster;
    delete metaInputHandler;

    FRayDisplay::RenderStaticDestroy();
    FSkyboxRenderComponent::RenderStaticDestroy();
    FPrimitiveRenderer::RenderStaticDestroy();
}

int main()
{
    GResourceManager.Init();

    SDL_Init(SDL_INIT_VIDEO);

    auto* window = SDL_CreateWindow("Nemo Tester",
                                    SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                    1600, 900, SDL_WINDOW_OPENGL);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    //SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    //SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 8);
    SDL_GLContext glContext = SDL_GL_CreateContext(window);

    auto* verStr = glGetString(GL_VERSION);
    printf("%s\n", verStr);

    myGlSetup();

    bool quit = false;
    SDL_Event e{};
    while (!quit)
    {
        while (SDL_PollEvent(&e))
        {
            ImGui_ImplSdlGL3_ProcessEvent(&e);
            if (e.type == SDL_QUIT)
                quit = true;
            else if (e.type == SDL_KEYDOWN)
                metaInputHandler->KeyPressed(ConvertKeyboardEvent(e.key));
            else if (e.type == SDL_KEYUP)
                metaInputHandler->KeyReleased(ConvertKeyboardEvent(e.key));
            else if (e.type == SDL_MOUSEMOTION)
                metaInputHandler->MouseMoved(ConvertMouseMotionEvent(e.motion));
            else if (e.type == SDL_MOUSEBUTTONDOWN)
                metaInputHandler->MousePressed(ConvertMouseButtonEvent(e.button));
            else if (e.type == SDL_MOUSEBUTTONUP)
                metaInputHandler->MouseReleased(ConvertMouseButtonEvent(e.button));
            else if (e.type == SDL_MOUSEWHEEL)
                metaInputHandler->MouseWheelRolled(ConvertMouseWheelEvent(e.wheel));
        }
        quit = myUpdateAndRender(window) || quit;
        SDL_GL_SwapWindow(window);
    }

    myGlCleanup();

    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);

    GResourceManager.Shutdown();

    return 0;
}
