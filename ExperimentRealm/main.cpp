#include "Shader.h"
#include "EntityLibrary.h"
#include "InputHandler.h"

#include <SDL.h>
#include <OpenGL/gl3.h>
#include <Vector3.h>
#include <Vector4.h>
#include <Quaternion.h>
#include <Matrix3x4.h>
#include <Ray.h>
#include <ResourceManager.h>

#include <Timeline.h>

#include <Camera.h>
#include <SceneNode.h>

#include <nanovg.h>
#define NANOVG_GL3_IMPLEMENTATION
#include <nanovg_gl.h>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl_gl3.h"

using namespace tc;

class FRenderWorld;
class FBoxPrimitive;
class FTranslateGizmo;
class FTranslateGizmoInputHandler;
class FPointTranslateGizmo;
class FPointTranslateGizmoInputHandler;
class FCameraWithPivot;
class FCameraInputHandler;
class FGrid;
class FRayVisualizer;
class FEditorMaster;
class FPointPrimitive;

FMetaInputHandler* metaInputHandler;
FHighResolutionClock GTimeline;
FRenderWorld* renderWorld;
//FTranslateGizmo* translateGizmo;
//FTranslateGizmoInputHandler* translateGizmoInputHandler;
//FPointTranslateGizmo* pointTranslateGizmo;
//FPointTranslateGizmoInputHandler* pointTranslateGizmoInputHandler;
FCameraWithPivot* cameraWithPivot;
FCameraInputHandler* cameraInputHandler;
FGrid* mainGrid;
FRayVisualizer* rayVisualizer;
FEditorMaster* editorMaster;
//FPointPrimitive* testPointPrimitive;
NVGcontext* vg;
long long frameCount = 0;
bool bShowStyleEditor;

class FCameraWithPivot
{
    FNode CameraPivot;
    FNode CameraTransform;
    FCamera Camera;
public:
    FCameraWithPivot()
    {
        CameraPivot.AddRef();
        CameraTransform.AddRef();
        CameraPivot.AddChild(&CameraTransform);
        Camera.SetZFar(100.f);
    }

    FNode& GetCameraPivot()
    {
        return CameraPivot;
    }

    FNode& GetCameraTransform()
    {
        return CameraTransform;
    }

    FCamera& GetCamera()
    {
        return Camera;
    }

    Matrix4 GetViewMatrix()
    {
        return GetCameraTransform().GetTransformFromWorld().ToMatrix4();
    }

    Matrix4 GetProjectionMatrix()
    {
        return GetCamera().GetProjectionMatrix();
    }

    Matrix4 GetViewProjectionMatrix()
    {
        return GetProjectionMatrix() * GetViewMatrix();
    }

    //TODO: Very very hackish. Rethink the viewport hierarchy
    Ray GetRayTo(int screenX, int screenY)
    {
        //TODO: No hard coded screen size
        const int width = 1600;
        const int height = 900;
        auto cameraPos = CameraTransform.GetWorldTranslation();
        auto vpMatrix = GetViewProjectionMatrix();
        auto ndcToWorld = vpMatrix.Inverse();
        float ndcX = (float)screenX / (float)width * 2.0f - 1.0f;
        float ndcY = 1.0f - 2.0f * (float)screenY / (float)height;
        //Vector4 worldNear = ndcToWorld * Vector4(ndcX, ndcY, -1.0f, 1.0f);
        //Vector3 near(worldNear[0] / worldNear[3], worldNear[1] / worldNear[3], worldNear[2] / worldNear[3]);
        Vector4 worldMiddle = ndcToWorld * Vector4(ndcX, ndcY, 0.0f, 1.0f);
        Vector3 middle(worldMiddle[0] / worldMiddle[3], worldMiddle[1] / worldMiddle[3], worldMiddle[2] / worldMiddle[3]);
        return Ray(cameraPos, middle - cameraPos);
    }
};

class IRenderComponent
{
public:
    virtual void RenderInit(FRenderWorld* rw) = 0;
    virtual void Render() = 0;
    virtual void RenderDestroy() = 0;

    IRenderComponent* PrevRenderComponent = nullptr;
    IRenderComponent* NextRenderComponent = nullptr;

protected:
    ~IRenderComponent()
    {
        if (PrevRenderComponent)
            PrevRenderComponent->NextRenderComponent = NextRenderComponent;
        if (NextRenderComponent)
            NextRenderComponent->PrevRenderComponent = PrevRenderComponent;
    }
};

//Used as linked list head
class FDummyRenderComponent : public IRenderComponent
{
public:
    void RenderInit(FRenderWorld* rw) override {}

    void Render() override {}

    void RenderDestroy() override {}

    void Insert(IRenderComponent* item)
    {
        IRenderComponent* next = this;
        while (next->NextRenderComponent)
            next = next->NextRenderComponent;
        next->NextRenderComponent = item;
        item->PrevRenderComponent = next;
    }
};

class IRayIntersectComponent
{
public:
    virtual bool DoesRayHit(const Ray& ray) = 0;

protected:
    ~IRayIntersectComponent() = default;
};

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

    bool KeyPressed(const SDL_KeyboardEvent& evt) override
    {
        if (evt.keysym.sym == SDLK_RGUI || evt.keysym.sym == SDLK_LALT)
            AcceptLMBRotate = true;
        return false;
    }

    bool KeyReleased(const SDL_KeyboardEvent& evt) override
    {
        if (evt.keysym.sym == SDLK_RGUI || evt.keysym.sym == SDLK_LALT)
            AcceptLMBRotate = false;
        return false;
    }

    bool MouseMoved(const SDL_MouseMotionEvent& evt) override
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

    bool MousePressed(const SDL_MouseButtonEvent& evt) override
    {
        if (evt.button == SDL_BUTTON_LEFT && AcceptLMBRotate)
        {
            MouseCurrentlyOrbit = true;
            MouseLastX = evt.x;
            MouseLastY = evt.y;
        }
        else if (evt.button == SDL_BUTTON_MIDDLE)
        {
            MouseCurrentlyOrbit = true;
            MouseLastX = evt.x;
            MouseLastY = evt.y;
        }
        else if (evt.button == SDL_BUTTON_RIGHT)
        {
            MouseInFpsView = true;
            MouseLastX = evt.x;
            MouseLastY = evt.y;
        }
        return false;
    }

    bool MouseReleased(const SDL_MouseButtonEvent& evt) override
    {
        if (evt.button == SDL_BUTTON_LEFT || evt.button == SDL_BUTTON_MIDDLE)
            MouseCurrentlyOrbit = false;
        else if (evt.button == SDL_BUTTON_RIGHT)
            MouseInFpsView = false;
        return false;
    }

    bool MouseWheelRolled(const SDL_MouseWheelEvent& evt) override
    {
        return IInputHandler::MouseWheelRolled(evt);
    }
};

class FRenderWorld
{
    FCameraWithPivot* Camera;
public:
    explicit FRenderWorld(FCameraWithPivot* c) : Camera(c) {}

    Matrix4 GetViewMatrix()
    {
        return Camera->GetViewMatrix();
    }

    Matrix4 GetProjectionMatrix()
    {
        return Camera->GetProjectionMatrix();
    }

    Matrix4 GetViewProjectionMatrix()
    {
        return Camera->GetViewProjectionMatrix();
    }
};

class FTransformComponent
{
public:
    FTransformComponent()
    {
        TransformNode.AddRef();
    }

    FNode& GetTransformNode()
    {
        return TransformNode;
    }

    const FNode& GetTransformNode() const
    {
        return TransformNode;
    }

    const Matrix3x4& GetTransformMatrix() const
    {
        return TransformNode.GetTransformToWorld();
    }

private:
    FNode TransformNode;
};

class FPositionComponent
{
public:
    Matrix3x4 GetTransformMatrix() const
    {
        return Matrix3x4(Position, Quaternion::IDENTITY, Vector3::ONE);
    }

    Vector3& GetPosition()
    {
        return Position;
    }

protected:
    Vector3 Position;
};

template <typename TOwner>
class TBoxRenderComponent : public IRenderComponent
{
public:
    TBoxRenderComponent() : RenderWorld(nullptr), Shader(nullptr) {}

    void RenderInit(FRenderWorld* rw) override
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
        Matrix3 normalTransform = RenderWorld->GetViewMatrix().ToMatrix3() * modelMatrix.ToMatrix3();
        normalTransform = normalTransform.Inverse();
        normalTransform = normalTransform.Transpose();
        Shader->Enable();
        Shader->SetUniformMatrix4fv("uModelViewProjectionMatrix", mvp.Data(), 1, true);
        Shader->SetUniformMatrix3fv("uNormalTransform", normalTransform.Data(), 1, true);

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
    FRenderWorld* RenderWorld;
    GLuint Buffers[2]{}, VertexArrays[1]{};
    FGLSLProgram* Shader;
};

template <typename TOwner>
class TBoxRayIntersectComponent : public IRayIntersectComponent
{
public:
    bool DoesRayHit(const Ray& ray) override
    {
        auto worldToLocal = static_cast<TOwner*>(this)->GetTransformMatrix().Inverse();
        auto localRay = ray.Transformed(worldToLocal);

        BoundingBox bounding = BoundingBox(-1.0f, 1.0f);
        float hitDist = localRay.HitDistance(bounding);
        return hitDist < M_INFINITY;
    }
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

class FPointRenderComponentStaticData
{
public:
    FPointRenderComponentStaticData()
    {
        UserCount++;
    }

    ~FPointRenderComponentStaticData()
    {
        UserCount--;
    }

    static void RenderStaticInit()
    {
        if (bInitialized)
            return;

        bInitialized = true;

        LOGDEBUG("FPointRenderComponentStaticData::RenderStaticInit called and ran\n");

        glGenBuffers(1, Buffers);
        glGenVertexArrays(1, VertexArrays);
        glBindVertexArray(VertexArrays[0]);

        const float origin[] = {0.0f, 0.0f, 0.0f};

        glBindBuffer(GL_ARRAY_BUFFER, Buffers[0]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(origin), origin, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
        glEnableVertexAttribArray(0);

        glBindVertexArray(0);

        Shader = FGLSLProgram::CreateFromFiles("fgizmocolor.vert", "fgizmocolor.frag");
    }

    static void RenderStaticDestroy()
    {
        LOGDEBUG("FPointRenderComponentStaticData usercount %d\n", UserCount);
        if (!bInitialized || UserCount > 0)
            return;

        LOGDEBUG("FPointRenderComponentStaticData::RenderStaticDestroy called and ran\n");

        delete Shader;
        glDeleteBuffers(1, Buffers);
        glDeleteVertexArrays(1, VertexArrays);
    }

protected:
    static int UserCount;
    static bool bInitialized;
    static GLuint Buffers[1];
    static GLuint VertexArrays[1];
    static FGLSLProgram* Shader;
};

int FPointRenderComponentStaticData::UserCount = 0;
bool FPointRenderComponentStaticData::bInitialized = false;
GLuint FPointRenderComponentStaticData::Buffers[1];
GLuint FPointRenderComponentStaticData::VertexArrays[1];
FGLSLProgram* FPointRenderComponentStaticData::Shader;

template <typename TOwner>
class TPointRenderComponent : public IRenderComponent, public FPointRenderComponentStaticData
{
public:
    TPointRenderComponent() : RenderWorld(nullptr) {}

    void RenderInit(FRenderWorld* rw) override
    {
        RenderStaticInit();
        RenderWorld = rw;
    }

    void Render() override
    {
        auto modelMatrix = static_cast<TOwner*>(this)->GetTransformMatrix();
        Matrix4 mvp = RenderWorld->GetViewProjectionMatrix() * modelMatrix;
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
    FRenderWorld* RenderWorld;
};

template <typename TOwner>
class TPointRayIntersectComponent : public IRayIntersectComponent
{
public:
    bool DoesRayHit(const Ray& ray) override
    {
        auto worldToLocal = static_cast<TOwner*>(this)->GetTransformMatrix().Inverse();
        auto localRay = ray.Transformed(worldToLocal);

        BoundingBox bounding = BoundingBox(-0.1f, 0.1f);
        float hitDist = localRay.HitDistance(bounding);
        return hitDist < M_INFINITY;
    }
};

class FPointPrimitive : public FBaseEntity, public FPositionComponent, public TPointRenderComponent<FPointPrimitive>, public TPointRayIntersectComponent<FPointPrimitive>
{
public:
    const char* GetTypeNameInString() const override
    {
        static const char* name = "FPointPrimitive";
        return name;
    }
};

class FRayRenderComponentStaticData
{
public:
    FRayRenderComponentStaticData()
    {
        UserCount++;
    }

    ~FRayRenderComponentStaticData()
    {
        UserCount--;
    }

    static void RenderStaticInit()
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

    static void RenderStaticDestroy()
    {
        if (!bInitialized || UserCount > 0)
            return;

        LOGDEBUG("FRayRenderComponentStaticData::RenderStaticDestroy called and ran\n");

        delete Shader;
        glDeleteBuffers(1, Buffers);
        glDeleteVertexArrays(1, VertexArrays);
    }

protected:
    static int UserCount;
    static bool bInitialized;
    static GLuint Buffers[1];
    static GLuint VertexArrays[1];
    static FGLSLProgram* Shader;
};

int FRayRenderComponentStaticData::UserCount = 0;
bool FRayRenderComponentStaticData::bInitialized = false;
GLuint FRayRenderComponentStaticData::Buffers[1];
GLuint FRayRenderComponentStaticData::VertexArrays[1];
FGLSLProgram* FRayRenderComponentStaticData::Shader;

template <typename TOwner>
class TRayRenderComponent : public IRenderComponent, public FRayRenderComponentStaticData
{
public:
    TRayRenderComponent() : RenderWorld(nullptr) {}

    void RenderInit(FRenderWorld* rw) override
    {
        RenderStaticInit();
        RenderWorld = rw;
    }

    void Render() override
    {
        Shader->Enable();
        Shader->SetUniformMatrix4fv("uModelViewProjectionMatrix",
                                    RenderWorld->GetViewProjectionMatrix().Data(), 1, true);
        Shader->SetUniform3f("uColor", 1.0, 1.0, 0.0);

        const auto& ray = static_cast<TOwner*>(this)->GetRay();

        static_assert(sizeof(ray) == sizeof(float) * 6);
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
    FRenderWorld* RenderWorld;
};

class FRayVisualizer : public TRayRenderComponent<FRayVisualizer>
{
public:
    explicit FRayVisualizer(const Ray& r) : TheRay(r) {}

    const Ray& GetRay()
    {
        return TheRay;
    }

private:
    Ray TheRay;
};

enum class EAxis : uint8_t
{
    None,
    X,
    Y,
    Z
};

template <typename TOwner>
class TTranslateGizmoRenderComponent : public IRenderComponent
{
public:
    TTranslateGizmoRenderComponent() : RenderWorld(nullptr), Shader(nullptr) {}

    void RenderInit(FRenderWorld* rw) override
    {
        RenderWorld = rw;

        float coneVertices[] = {
                0.000000f, -1.000000f, -1.000000f,
                0.000000f, 1.000000f, 0.000000f,
                0.866025f, -1.000000f, -0.500000f,
                0.866025f, -1.000000f, 0.500000f,
                -0.000000f, -1.000000f, 1.000000f,
                -0.866025f, -1.000000f, 0.500000f,
                -0.866025f, -1.000000f, -0.500000f
        };

        unsigned short coneIndices[] = {
                0, 1, 2,
                2, 1, 3,
                3, 1, 4,
                4, 1, 5,
                5, 1, 6,
                6, 1, 0,
                0, 2, 3,
                0, 3, 4,
                0, 4, 5,
                0, 5, 6
        };

        float lineVertices[] = {
                0.0, 0.0, 0.0,
                0.0, 1.0, 0.0
        };

        glGenBuffers(3, Buffers);
        glGenVertexArrays(2, VertexArrays);

        //Cone
        glBindVertexArray(VertexArrays[0]);

        glBindBuffer(GL_ARRAY_BUFFER, Buffers[0]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(coneVertices), coneVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Buffers[1]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(coneIndices), coneIndices, GL_STATIC_DRAW);

        //Line
        glBindVertexArray(VertexArrays[1]);

        glBindBuffer(GL_ARRAY_BUFFER, Buffers[2]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(lineVertices), lineVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        glBindVertexArray(0);

        Shader = FGLSLProgram::CreateFromFiles("fgizmocolor.vert", "fgizmocolor.frag");
    }

    void Render() override
    {
        Matrix4 vpMatrix = RenderWorld->GetProjectionMatrix() * RenderWorld->GetViewMatrix();

        Shader->Enable();

        {
            //Y axis
            if (static_cast<TOwner*>(this)->GetHighlightAxis() == EAxis::Y)
                Shader->SetUniform3f("uColor", 0.5, 1.0, 0.5);
            else
                Shader->SetUniform3f("uColor", 0.0, 0.77, 0.0);
            Matrix3x4 localMatrix = Matrix3x4(Vector3(0.0f, 1.0f, 0.0f), Quaternion::IDENTITY, Vector3(0.1f, 0.1f, 0.1f));
            Matrix3x4 modelMatrix = static_cast<TOwner*>(this)->GetTransformMatrix() * localMatrix;
            Matrix4 mvp = vpMatrix * modelMatrix;

            Shader->SetUniformMatrix4fv("uModelViewProjectionMatrix", mvp.Data(), 1, true);

            glBindVertexArray(VertexArrays[0]);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Buffers[1]);
            glDrawElements(GL_TRIANGLES, 30, GL_UNSIGNED_SHORT, nullptr);

            localMatrix = Matrix3x4::IDENTITY;
            modelMatrix = static_cast<TOwner*>(this)->GetTransformMatrix() * localMatrix;
            mvp = vpMatrix * modelMatrix;
            Shader->SetUniformMatrix4fv("uModelViewProjectionMatrix", mvp.Data(), 1, true);

            glBindVertexArray(VertexArrays[1]);
            glDrawArrays(GL_LINES, 0, 2);
            glBindVertexArray(0);
        }
        {
            //X axis
            if (static_cast<TOwner*>(this)->GetHighlightAxis() == EAxis::X)
                Shader->SetUniform3f("uColor", 1.0, 0.5, 0.5);
            else
                Shader->SetUniform3f("uColor", 1.0, 0.0, 0.0);
            Matrix3x4 localMatrix = Matrix3x4(Vector3(1.0f, 0.0f, 0.0f), Quaternion(Vector3::UNIT_Y, Vector3::UNIT_X), Vector3(0.1f, 0.1f, 0.1f));
            Matrix3x4 modelMatrix = static_cast<TOwner*>(this)->GetTransformMatrix() * localMatrix;
            Matrix4 mvp = vpMatrix * modelMatrix;

            Shader->SetUniformMatrix4fv("uModelViewProjectionMatrix", mvp.Data(), 1, true);

            glBindVertexArray(VertexArrays[0]);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Buffers[1]);
            glDrawElements(GL_TRIANGLES, 30, GL_UNSIGNED_SHORT, nullptr);

            localMatrix = Matrix3x4(Vector3::ZERO, Quaternion(Vector3::UNIT_Y, Vector3::UNIT_X), Vector3::ONE);
            modelMatrix = static_cast<TOwner*>(this)->GetTransformMatrix() * localMatrix;
            mvp = vpMatrix * modelMatrix;
            Shader->SetUniformMatrix4fv("uModelViewProjectionMatrix", mvp.Data(), 1, true);

            glBindVertexArray(VertexArrays[1]);
            glDrawArrays(GL_LINES, 0, 2);
            glBindVertexArray(0);
        }
        {
            //Z axis
            if (static_cast<TOwner*>(this)->GetHighlightAxis() == EAxis::Z)
                Shader->SetUniform3f("uColor", 0.5, 0.5, 1.0);
            else
                Shader->SetUniform3f("uColor", 0.0, 0.0, 1.0);
            Matrix3x4 localMatrix = Matrix3x4(Vector3(0.0f, 0.0f, 1.0f), Quaternion(Vector3::UNIT_Y, Vector3::UNIT_Z), Vector3(0.1f, 0.1f, 0.1f));
            Matrix3x4 modelMatrix = static_cast<TOwner*>(this)->GetTransformMatrix() * localMatrix;
            Matrix4 mvp = vpMatrix * modelMatrix;

            Shader->SetUniformMatrix4fv("uModelViewProjectionMatrix", mvp.Data(), 1, true);

            glBindVertexArray(VertexArrays[0]);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Buffers[1]);
            glDrawElements(GL_TRIANGLES, 30, GL_UNSIGNED_SHORT, nullptr);

            localMatrix = Matrix3x4(Vector3::ZERO, Quaternion(Vector3::UNIT_Y, Vector3::UNIT_Z), Vector3::ONE);
            modelMatrix = static_cast<TOwner*>(this)->GetTransformMatrix() * localMatrix;
            mvp = vpMatrix * modelMatrix;
            Shader->SetUniformMatrix4fv("uModelViewProjectionMatrix", mvp.Data(), 1, true);

            glBindVertexArray(VertexArrays[1]);
            glDrawArrays(GL_LINES, 0, 2);
            glBindVertexArray(0);
        }
    }

    void RenderDestroy() override
    {
        delete Shader;
        glDeleteBuffers(3, Buffers);
        glDeleteVertexArrays(2, VertexArrays);
    }

private:
    FRenderWorld* RenderWorld;
    GLuint Buffers[3]{}, VertexArrays[2]{};
    FGLSLProgram* Shader;
};

class FTranslateGizmo : public TTranslateGizmoRenderComponent<FTranslateGizmo>
{
public:
    explicit FTranslateGizmo(FNode* trackedTransform)
            : TransformNode(trackedTransform), HighlightAxis(EAxis::None)
    {
        TransformNode->AddRef();
    }

    FNode& GetTransformNode()
    {
        return *TransformNode;
    }

    const FNode& GetTransformNode() const
    {
        return *TransformNode;
    }

    const Matrix3x4& GetTransformMatrix() const
    {
        return TransformNode->GetTransformToWorld();
    }

    EAxis HitWorldSpaceRay(const Ray& worldRay)
    {
        auto worldToLocal = GetTransformMatrix().Inverse();
        auto localRay = worldRay.Transformed(worldToLocal);

        BoundingBox xAabb = BoundingBox(Vector3(0.0f, -0.1f, -0.1f), Vector3(1.1f, 0.1f, 0.1f));
        BoundingBox yAabb = BoundingBox(Vector3(-0.1f, 0.0f -0.1f), Vector3(0.1f, 1.1f, 0.1f));
        BoundingBox zAabb = BoundingBox(Vector3(-0.1f, -0.1f, 0.0f), Vector3(0.1f, 0.1f, 1.1f));
        if (localRay.HitDistance(xAabb) < M_INFINITY)
            return EAxis::X;
        else if (localRay.HitDistance(yAabb) < M_INFINITY)
            return EAxis::Y;
        else if (localRay.HitDistance(zAabb) < M_INFINITY)
            return EAxis::Z;
        else
            return EAxis::None;
    }

    EAxis GetHighlightAxis() const
    {
        return HighlightAxis;
    }

    void SetHighlightAxis(EAxis value)
    {
        HighlightAxis = value;
    }

    Ray GetAxisRayWorldSpace(EAxis axis)
    {
        Vector3 dir;
        switch (axis)
        {
        case EAxis::X:
            dir = Vector3::UNIT_X;
            break;
        case EAxis::Y:
            dir = Vector3::UNIT_Y;
            break;
        case EAxis::Z:
            dir = Vector3::UNIT_Z;
            break;
        default:
            //Or probably throw an error or something
            return Ray();
        }
        Ray localRay = Ray(Vector3::ZERO, dir);
        return localRay.Transformed(GetTransformMatrix());
    }

private:
    // Component: Transform
    FNode* TransformNode;

    // Data (Shared Data?) (Or What?)
    EAxis HighlightAxis; //1=x 2=y 3=z
};

class FPointTranslateGizmo : public FTransformComponent, public TTranslateGizmoRenderComponent<FPointTranslateGizmo>
{
public:
    explicit FPointTranslateGizmo(Vector3* trackedPoint) : TrackedPoint(trackedPoint), HighlightAxis(EAxis::None)
    {
        SetTransformFromPoint();
    }

    void SetTransformFromPoint()
    {
        GetTransformNode().SetTranslation(*TrackedPoint);
    }

    void MoveAlongAxis(EAxis axis, float offset)
    {
        switch (axis)
        {
        case EAxis::X:
            GetTransformNode().Translate(offset, 0.0f, 0.0f);
            break;
        case EAxis::Y:
            GetTransformNode().Translate(0.0f, offset, 0.0f);
            break;
        case EAxis::Z:
            GetTransformNode().Translate(0.0f, 0.0f, offset);
            break;
        default:
            break;
        }
        *TrackedPoint = GetTransformNode().GetTranslation();
    }

    EAxis HitWorldSpaceRay(const Ray& worldRay)
    {
        auto worldToLocal = GetTransformMatrix().Inverse();
        auto localRay = worldRay.Transformed(worldToLocal);

        BoundingBox xAabb = BoundingBox(Vector3(0.0f, -0.1f, -0.1f), Vector3(1.1f, 0.1f, 0.1f));
        BoundingBox yAabb = BoundingBox(Vector3(-0.1f, 0.0f -0.1f), Vector3(0.1f, 1.1f, 0.1f));
        BoundingBox zAabb = BoundingBox(Vector3(-0.1f, -0.1f, 0.0f), Vector3(0.1f, 0.1f, 1.1f));
        if (localRay.HitDistance(xAabb) < M_INFINITY)
            return EAxis::X;
        else if (localRay.HitDistance(yAabb) < M_INFINITY)
            return EAxis::Y;
        else if (localRay.HitDistance(zAabb) < M_INFINITY)
            return EAxis::Z;
        else
            return EAxis::None;
    }

    EAxis GetHighlightAxis() const
    {
        return HighlightAxis;
    }

    void SetHighlightAxis(EAxis value)
    {
        HighlightAxis = value;
    }

    Ray GetAxisRayWorldSpace(EAxis axis)
    {
        Vector3 dir;
        switch (axis)
        {
        case EAxis::X:
            dir = Vector3::UNIT_X;
            break;
        case EAxis::Y:
            dir = Vector3::UNIT_Y;
            break;
        case EAxis::Z:
            dir = Vector3::UNIT_Z;
            break;
        default:
            //Or probably throw an error or something
            return Ray();
        }
        Ray localRay = Ray(Vector3::ZERO, dir);
        return localRay.Transformed(GetTransformMatrix());
    }

private:
    // Data (Shared Data?) (Or What?)
    Vector3* TrackedPoint;
    EAxis HighlightAxis; //1=x 2=y 3=z
};

class FTranslateGizmoInputHandler : public IInputHandler
{
public:
    FTranslateGizmoInputHandler(FTranslateGizmo* gizmo, FCameraWithPivot* camera)
            : Gizmo(gizmo), CurrentCamera(camera), DragAlongAxis(EAxis::None), bDraggingAlong(false)
    {
    }

    bool MouseMoved(const SDL_MouseMotionEvent& evt) override
    {
        if (bDraggingAlong)
        {
            Gizmo->SetHighlightAxis(DragAlongAxis);
            auto worldRay = CurrentCamera->GetRayTo(evt.x, evt.y);

            Vector3 closestPoint = DragAlongRay.ClosestPoint(worldRay);
            Vector3 offset = closestPoint - LastClosestPoint;
            float offsetDist = offset.Length();
            float offsetDotRaydir = offset.DotProduct(DragAlongRay.Direction);
            float oneWithSign = offsetDotRaydir > 0.0f ? 1.0f : -1.0f;
            switch (DragAlongAxis)
            {
            case EAxis::X:
                Gizmo->GetTransformNode().Translate(offsetDist * oneWithSign, 0.0f, 0.0f);
                break;
            case EAxis::Y:
                Gizmo->GetTransformNode().Translate(0.0f, offsetDist * oneWithSign, 0.0f);
                break;
            case EAxis::Z:
                Gizmo->GetTransformNode().Translate(0.0f, 0.0f, offsetDist * oneWithSign);
                break;
            default:
                break;
            }
            LastClosestPoint = closestPoint;
            return true;
        }
        auto worldRay = CurrentCamera->GetRayTo(evt.x, evt.y);
        auto hit = Gizmo->HitWorldSpaceRay(worldRay);
        Gizmo->SetHighlightAxis(hit);
        return false;
    }

    bool MousePressed(const SDL_MouseButtonEvent& evt) override
    {
        if (evt.button == SDL_BUTTON_LEFT)
        {
            auto ray = CurrentCamera->GetRayTo(evt.x, evt.y);
            delete rayVisualizer;
            rayVisualizer = new FRayVisualizer(ray);
            rayVisualizer->RenderInit(renderWorld);

            auto hit = Gizmo->HitWorldSpaceRay(ray);
            if (hit != EAxis::None)
            {
                DragAlongRay = Gizmo->GetAxisRayWorldSpace(hit);
                DragAlongAxis = hit;
                bDraggingAlong = true;

                LastClosestPoint = DragAlongRay.ClosestPoint(ray);
                return true;
            }
        }
        return false;
    }

    bool MouseReleased(const SDL_MouseButtonEvent& evt) override
    {
        bDraggingAlong = false;
        return false;
    }

private:
    FTranslateGizmo* Gizmo;
    FCameraWithPivot* CurrentCamera;

    Ray DragAlongRay;
    EAxis DragAlongAxis;
    bool bDraggingAlong;
    Vector3 LastClosestPoint;
};

class FPointTranslateGizmoInputHandler : public IInputHandler
{
public:
    FPointTranslateGizmoInputHandler(FPointTranslateGizmo* gizmo, FCameraWithPivot* camera)
            : Gizmo(gizmo), CurrentCamera(camera), DragAlongAxis(EAxis::None), bDraggingAlong(false)
    {
    }

    bool MouseMoved(const SDL_MouseMotionEvent& evt) override
    {
        if (bDraggingAlong)
        {
            Gizmo->SetHighlightAxis(DragAlongAxis);
            auto worldRay = CurrentCamera->GetRayTo(evt.x, evt.y);

            Vector3 closestPoint = DragAlongRay.ClosestPoint(worldRay);
            Vector3 offset = closestPoint - LastClosestPoint;
            float offsetDist = offset.Length();
            float offsetDotRaydir = offset.DotProduct(DragAlongRay.Direction);
            float oneWithSign = offsetDotRaydir > 0.0f ? 1.0f : -1.0f;
            Gizmo->MoveAlongAxis(DragAlongAxis, offsetDist * oneWithSign);
            LastClosestPoint = closestPoint;
            return true;
        }
        auto worldRay = CurrentCamera->GetRayTo(evt.x, evt.y);
        auto hit = Gizmo->HitWorldSpaceRay(worldRay);
        Gizmo->SetHighlightAxis(hit);
        return false;
    }

    bool MousePressed(const SDL_MouseButtonEvent& evt) override
    {
        if (evt.button == SDL_BUTTON_LEFT)
        {
            auto ray = CurrentCamera->GetRayTo(evt.x, evt.y);
            delete rayVisualizer;
            rayVisualizer = new FRayVisualizer(ray);
            rayVisualizer->RenderInit(renderWorld);

            auto hit = Gizmo->HitWorldSpaceRay(ray);
            if (hit != EAxis::None)
            {
                DragAlongRay = Gizmo->GetAxisRayWorldSpace(hit);
                DragAlongAxis = hit;
                bDraggingAlong = true;

                LastClosestPoint = DragAlongRay.ClosestPoint(ray);
                return true;
            }
        }
        return false;
    }

    bool MouseReleased(const SDL_MouseButtonEvent& evt) override
    {
        bDraggingAlong = false;
        return false;
    }

private:
    FPointTranslateGizmo* Gizmo;
    FCameraWithPivot* CurrentCamera;

    Ray DragAlongRay;
    EAxis DragAlongAxis;
    bool bDraggingAlong;
    Vector3 LastClosestPoint;

};

class FGrid
{
    // Component: Render
    FRenderWorld* RenderWorld;
    GLuint Buffers[1]{}, VertexArrays[1]{};
    FGLSLProgram* Shader;
    int NumLines;

    // Data (Shared Data?) (Or What?)
    Vector3 OriginPoint;

public:
    FGrid() : RenderWorld(nullptr), Shader(nullptr), NumLines(0)
    {
    }

    void RenderInit(FRenderWorld* rw)
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

class FEditorMaster : public IInputHandler
{
public:
    FEditorMaster() : bWireframe(false), SelectedEntity(nullptr),TranslateGizmo(nullptr),
                      TranslateGizmoInputHandler(nullptr), PointTranslateGizmo(nullptr),
                      PointTranslateGizmoInputHandler(nullptr)
    {
    }

    ~FEditorMaster() override
    {
        for (auto* entity : EntityVector)
            delete entity;
    }

    bool KeyPressed(const SDL_KeyboardEvent& evt) override
    {
        if (evt.keysym.sym == SDLK_z)
        {
            ToggleWireframe();
            return true;
        }
        return false;
    }

    bool KeyReleased(const SDL_KeyboardEvent& evt) override
    {
        return false;
    }

    bool MousePressed(const SDL_MouseButtonEvent& evt) override
    {
        if (evt.button == SDL_BUTTON_LEFT)
        {
            auto worldRay = cameraWithPivot->GetRayTo(evt.x, evt.y);

            for (auto* entity : EntityVector)
            {
                auto* rayIntersectComp = dynamic_cast<IRayIntersectComponent*>(entity);
                if (rayIntersectComp && rayIntersectComp->DoesRayHit(worldRay))
                {
                    NewSelectedEntity = entity;
                    if (NewSelectedEntity != SelectedEntity)
                    {
                        RemoveGizmos();
                        CreateGizmoFor(NewSelectedEntity);
                        SelectedEntity = NewSelectedEntity;
                    }
                    return true;
                }
            }
            NewSelectedEntity = nullptr;
            if (NewSelectedEntity != SelectedEntity)
            {
                RemoveGizmos();
                SelectedEntity = NewSelectedEntity;
            }
        }

        return false;
    }

    void CreateGizmoFor(FBaseEntity* entity)
    {
        if (dynamic_cast<FBoxPrimitive*>(entity))
        {
            auto* box = dynamic_cast<FBoxPrimitive*>(entity);
            TranslateGizmo = new FTranslateGizmo(&box->GetTransformNode());
            TranslateGizmo->RenderInit(renderWorld);
            TranslateGizmoInputHandler = new FTranslateGizmoInputHandler(TranslateGizmo, cameraWithPivot);
            metaInputHandler->InsertFront(TranslateGizmoInputHandler);
        }
        else if (dynamic_cast<FPointPrimitive*>(entity))
        {
            auto* point = dynamic_cast<FPointPrimitive*>(entity);
            PointTranslateGizmo = new FPointTranslateGizmo(&point->GetPosition());
            PointTranslateGizmo->RenderInit(renderWorld);
            PointTranslateGizmoInputHandler = new FPointTranslateGizmoInputHandler(PointTranslateGizmo, cameraWithPivot);
            metaInputHandler->InsertFront(PointTranslateGizmoInputHandler);
        }
    }

    void RemoveGizmos()
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

    void ToggleWireframe()
    {
        bWireframe = !bWireframe;
    }

    bool IsWireframe() const
    {
        return bWireframe;
    }

    void ImGuiUpdate()
    {
        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
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
            EntityVector.push_back(box);
            RenderComponentListHead.Insert(box);
        }
        if (ImGui::Button("Point"))
        {
            auto* point = new FPointPrimitive();
            point->RenderInit(renderWorld);
            EntityVector.push_back(point);
            RenderComponentListHead.Insert(point);
        }
        ImGui::Button("Bezier Control Point");
        ImGui::End();
    }

    void Render()
    {
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);

        auto* item = RenderComponentListHead.NextRenderComponent;
        while (item)
        {
            item->Render();
            item = item->NextRenderComponent;
        }

        glDisable(GL_DEPTH_TEST);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        if (TranslateGizmo)
            TranslateGizmo->Render();

        if (PointTranslateGizmo)
            PointTranslateGizmo->Render();
    }

    void RenderDestroy()
    {
        RemoveGizmos();

        auto* item = RenderComponentListHead.NextRenderComponent;
        while (item)
        {
            item->RenderDestroy();
            item = item->NextRenderComponent;
        }
    }

private:
    bool bWireframe;
    vector<FBaseEntity*> EntityVector;
    FDummyRenderComponent RenderComponentListHead;

    FBaseEntity* SelectedEntity;
    FBaseEntity* NewSelectedEntity;

    FTranslateGizmo* TranslateGizmo;
    FTranslateGizmoInputHandler* TranslateGizmoInputHandler;
    FPointTranslateGizmo* PointTranslateGizmo;
    FPointTranslateGizmoInputHandler* PointTranslateGizmoInputHandler;
};

void myGlSetup()
{
    GTimeline.Init();
    metaInputHandler = new FMetaInputHandler();
    editorMaster = new FEditorMaster();
    metaInputHandler->Insert(editorMaster);

    cameraWithPivot = new FCameraWithPivot();
    cameraInputHandler = new FCameraInputHandler(cameraWithPivot);
    metaInputHandler->Insert(cameraInputHandler);

    renderWorld = new FRenderWorld(cameraWithPivot);
    cameraWithPivot->GetCameraTransform().Translate(0, 3, 6.18);
    cameraWithPivot->GetCameraTransform().LookAt(Vector3::ZERO);
    cameraWithPivot->GetCamera().SetAspectRatio((float)1600/(float)900);

    FRayVisualizer::RenderStaticInit();

    //testPointPrimitive = new FPointPrimitive();
    //testPointPrimitive->GetPosition() = Vector3::ONE;
    //testPointPrimitive->RenderStaticInit();
    //testPointPrimitive->RenderInit(renderWorld);

    mainGrid = new FGrid();
    mainGrid->RenderInit(renderWorld);

    //static FNode nodetest;
    //translateGizmo = new FTranslateGizmo(&box->GetTransformNode());
    //translateGizmo = new FTranslateGizmo(&nodetest);
    //translateGizmo->RenderInit(renderWorld);

    //translateGizmoInputHandler = new FTranslateGizmoInputHandler(translateGizmo, cameraWithPivot);
    //metaInputHandler->Insert(translateGizmoInputHandler);

    //pointTranslateGizmo = new FPointTranslateGizmo(&testPointPrimitive->GetPosition());
    //pointTranslateGizmo->RenderInit(renderWorld);
    //pointTranslateGizmoInputHandler = new FPointTranslateGizmoInputHandler(pointTranslateGizmo, cameraWithPivot);
    //metaInputHandler->Insert(pointTranslateGizmoInputHandler);

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
    io.Fonts->AddFontDefault();
    io.Fonts->AddFontFromFileTTF(GResourceManager.FindFile("Roboto-Light.ttf").c_str(), 20.0f);
}

void myGlRender(SDL_Window* window)
{
    glClearColor(0, 0, 0, 1);
    glClearDepth(1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    if (editorMaster->IsWireframe())
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    if (rayVisualizer)
        rayVisualizer->Render();

    //testPointPrimitive->Render();
    mainGrid->Render();

    editorMaster->Render();

    //glDisable(GL_DEPTH_TEST);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    //translateGizmo->Render();
    //pointTranslateGizmo->Render();

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
}

void myGlCleanup()
{
    editorMaster->RenderDestroy();
    ImGui_ImplSdlGL3_Shutdown();
    nvgDeleteGL3(vg);
    if (rayVisualizer)
        rayVisualizer->RenderDestroy();
    delete rayVisualizer;
    //testPointPrimitive->RenderDestroy();
    mainGrid->RenderDestroy();
    //translateGizmo->RenderDestroy();
    //delete translateGizmo;

    //pointTranslateGizmo->RenderDestroy();
    //delete pointTranslateGizmo;
    //delete pointTranslateGizmoInputHandler;

    //delete testPointPrimitive;
    delete mainGrid;
    delete renderWorld;
    delete cameraWithPivot;
    delete cameraInputHandler;
    //delete translateGizmoInputHandler;
    delete editorMaster;
    delete metaInputHandler;

    FRayVisualizer::RenderStaticDestroy();
    //testPointPrimitive->RenderStaticDestroy();
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
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 8);
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
                metaInputHandler->KeyPressed(e.key);
            else if (e.type == SDL_KEYUP)
                metaInputHandler->KeyReleased(e.key);
            else if (e.type == SDL_MOUSEMOTION)
                metaInputHandler->MouseMoved(e.motion);
            else if (e.type == SDL_MOUSEBUTTONDOWN)
                metaInputHandler->MousePressed(e.button);
            else if (e.type == SDL_MOUSEBUTTONUP)
                metaInputHandler->MouseReleased(e.button);
            else if (e.type == SDL_MOUSEWHEEL)
                metaInputHandler->MouseWheelRolled(e.wheel);
        }
        myGlRender(window);
        SDL_GL_SwapWindow(window);
    }

    myGlCleanup();

    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);

    GResourceManager.Shutdown();

    return 0;
}
