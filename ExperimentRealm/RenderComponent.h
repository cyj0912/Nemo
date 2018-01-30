#pragma once

#include <SceneNode.h>
#include <Ray.h>

namespace tc
{

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
};

class FViewPort
{
public:
    explicit FViewPort(FCameraWithPivot* c, int w, int h) : Camera(c), Width(w), Height(h)
    {
        c->GetCamera().SetAspectRatio((float)Width / (float)Height);
    }

    FCameraWithPivot* GetCamera() const
    {
        return Camera;
    }

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

    Ray GetRayTo(int screenX, int screenY)
    {
        auto cameraPos = GetCamera()->GetCameraTransform().GetWorldTranslation();
        auto vpMatrix = GetViewProjectionMatrix();
        auto ndcToWorld = vpMatrix.Inverse();
        float ndcX = (float)screenX / (float)Width * 2.0f - 1.0f;
        float ndcY = 1.0f - 2.0f * (float)screenY / (float)Height;
        Vector4 worldMiddle = ndcToWorld * Vector4(ndcX, ndcY, 0.0f, 1.0f);
        Vector3 middle(worldMiddle[0] / worldMiddle[3], worldMiddle[1] / worldMiddle[3], worldMiddle[2] / worldMiddle[3]);
        return Ray(cameraPos, middle - cameraPos);
    }

private:
    FCameraWithPivot* Camera;
    int Width, Height;
};

class IRenderComponent
{
public:
    virtual void RenderInit(FViewPort* rw) = 0;
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
    void RenderInit(FViewPort* rw) override {}

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

} /* namespace tc */
