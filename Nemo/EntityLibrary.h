#pragma once
#include "RenderComponent.h"
#include "EditorMaster.h"
#include "PrimitiveRenderer.h"
#include "InteractionSystem.h"
#include <SceneNode.h>
#include <Ray.h>

namespace tc
{

class FInteractionSystem;

class FBaseEntity
{
public:
    FBaseEntity() : Manager(nullptr) {}
    virtual ~FBaseEntity();
    virtual const char* GetTypeNameInString() const;
    virtual size_t CountSubEntities() const;
    virtual FBaseEntity* GetSubEntity(size_t index);
    virtual void ImGuiUpdate(FInteractionSystem* interactionSystem);

    FEntityManager* GetEntityManager() const
    {
        return Manager;
    }

    void SetEntityManager(FEntityManager* value)
    {
        Manager = value;
    }

private:
    FEntityManager* Manager;
};

class FTransformComponent
{
public:
    FTransformComponent()
    {
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

    const Vector3& GetPosition() const
    {
        return Position;
    }

    void SetPosition(const Vector3& v)
    {
        Position = v;
    }

protected:
    Vector3 Position;
};

class IRayIntersectComponent
{
public:
    virtual float RayHitDistance(const Ray& ray) = 0;

protected:
    ~IRayIntersectComponent() = default;
};

template <typename TOwner>
class TBoxRayIntersectComponent : public IRayIntersectComponent
{
public:
    float RayHitDistance(const Ray& ray) override
    {
        auto worldToLocal = static_cast<TOwner*>(this)->GetTransformMatrix().Inverse();
        auto localRay = ray.Transformed(worldToLocal);

        BoundingBox bounding = BoundingBox(-1.0f, 1.0f);
        float hitDist = localRay.HitDistance(bounding);
        return hitDist;
    }
};

template <typename TOwner>
class TPointRayIntersectComponent : public IRayIntersectComponent
{
public:
    float RayHitDistance(const Ray& ray) override
    {
        auto worldToLocal = static_cast<TOwner*>(this)->GetTransformMatrix().Inverse();
        auto localRay = ray.Transformed(worldToLocal);

        Sphere bounding = Sphere(Vector3::ZERO, 0.11f);
        float hitDist = localRay.HitDistance(bounding);
        return hitDist;
    }
};

template <typename TOwner>
class TPointRenderComponent : public IRenderComponent
{
public:
    void RenderInit(FViewPort* rw) override
    {
    }

    void Render() override
    {
        GEditorMaster->GetPrimitiveRenderer()->DrawPoint(static_cast<TOwner*>(this)->GetPosition()
                , 10.0f, Color::YELLOW);
    }

    void RenderDestroy() override
    {
    }
};

class FPointPrimitive : public FBaseEntity,
                        public FPositionComponent,
                        public TPointRenderComponent<FPointPrimitive>,
                        public TPointRayIntersectComponent<FPointPrimitive>,
                        public IInteractionComponent
{
public:
    const char* GetTypeNameInString() const override
    {
        static const char* name = "FPointPrimitive";
        return name;
    }

    EGizmoFlags GetGizmoFlags() override
    {
        return GF_TRANSLATE;
    }

    Matrix3x4 QueryPreferredGizmoTransform() override
    {
        return {GetPosition(), Quaternion::IDENTITY, Vector3::ONE};
    }

    void SetGizmoTransformStart(const FNode& node) override
    {
        GizmoTranslateStart = node.GetTranslation();
    }

    void UpdateFromGizmoTransform(const FNode& node) override
    {
        SetPosition(GetPosition() + node.GetTranslation() - GizmoTranslateStart);
        GizmoTranslateStart = node.GetTranslation();
    }

    IRayIntersectComponent* GetRayIntersectComponent() override
    {
        return this;
    }

private:
    Vector3 GizmoTranslateStart;
};

} /* namespace tc */
