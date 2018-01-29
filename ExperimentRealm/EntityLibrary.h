#pragma once

#include <SceneNode.h>
#include <Ray.h>

namespace tc
{

class FBaseEntity
{
public:
    virtual ~FBaseEntity();
    virtual const char* GetTypeNameInString() const;
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

        BoundingBox bounding = BoundingBox(-0.1f, 0.1f);
        float hitDist = localRay.HitDistance(bounding);
        return hitDist;
    }
};

} /* namespace tc */
