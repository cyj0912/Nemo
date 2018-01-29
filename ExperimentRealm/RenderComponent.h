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

class FViewPort
{
    FCameraWithPivot* Camera;
public:
    explicit FViewPort(FCameraWithPivot* c) : Camera(c) {}

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
