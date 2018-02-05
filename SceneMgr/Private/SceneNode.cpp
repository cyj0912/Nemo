#include "SceneNode.h"
#include "Scene.h"

namespace tc
{

FNode::FNode()
    : Parent(nullptr), RootNode(this), Scale(1.0f, 1.0f, 1.0f), TransformToParentDirty(true), TransformFromParentDirty(true),
      WorldTransformDirty(true), InverseWorldTransformDirty(true)
{
    SetName("unnamedNode");
}

FNode::FNode(const FNode& rhs) : Name(rhs.Name), Parent(rhs.Parent), RootNode(rhs.RootNode), Children(),
                                 Translation(rhs.Translation), Rotation(rhs.Rotation), Scale(rhs.Scale),
                                 TransformToParentDirty(true), TransformFromParentDirty(true),
                                 WorldTransformDirty(true), InverseWorldTransformDirty(true)
{
}

FNode::FNode(FNode&& rhs) noexcept : Name(std::move(rhs.Name)), Parent(rhs.Parent), RootNode(rhs.RootNode), Children(std::move(rhs.Children)),
                                     Translation(rhs.Translation), Rotation(rhs.Rotation), Scale(rhs.Scale),
                                     TransformToParentDirty(true), TransformFromParentDirty(true),
                                     WorldTransformDirty(true), InverseWorldTransformDirty(true)
{
    rhs.Children.clear();
}

FNode& FNode::operator=(const FNode& rhs)
{
    Name = rhs.Name;
    Parent = rhs.Parent;
    RootNode = rhs.RootNode;
    Children.clear()
    Translation = rhs.Translation;
    Rotation = rhs.Rotation;
    Scale = rhs.Scale;
    TransformToParentDirty = true;
    TransformFromParentDirty = true;
    WorldTransformDirty = true;
    InverseWorldTransformDirty = true;
    return *this;
}

FNode& FNode::operator=(FNode&& rhs) noexcept
{
    if (this != &rhs)
    {
        Name = std::move(rhs.Name);
        Parent = rhs.Parent;
        RootNode = rhs.RootNode;
        Children = std::move(rhs.Children);
        rhs.Children.clear();
        Translation = rhs.Translation;
        Rotation = rhs.Rotation;
        Scale = rhs.Scale;
        TransformToParentDirty = true;
        TransformFromParentDirty = true;
        WorldTransformDirty = true;
        InverseWorldTransformDirty = true;
    }
    return *this;
}

void FNode::AddChild(FNode *child)
{
    TRefPtr<FNode> reference(child, true);
    Children.push_back(reference);
    child->SetParent(this);
}

//TODO I'm still thinking how to do caching.
FNode* FNode::FindNodeByName(const string &name)
{
    if (Name == name)
        return this;
    for (const auto child : Children)
    {
        auto* result = child->FindNodeByName(name);
        if (result)
            return result;
    }
    return nullptr;
}

const Matrix3x4& FNode::GetTransformToParent() const
{
    if (TransformToParentDirty) {
        TransformToParent = Matrix3x4(Translation, Rotation, Scale);
        TransformToParentDirty = false;
    }
    return TransformToParent;
}

const Matrix3x4& FNode::GetTransformFromParent() const
{
    //This is based on TransformToParent, check if that's dirty first
    if (TransformFromParentDirty || TransformToParentDirty) {
        TransformFromParent = GetTransformToParent().Inverse();
        TransformFromParentDirty = false;
    }
    return TransformFromParent;
}

const Matrix3x4& FNode::GetTransformToWorld() const
{
    if (!WorldTransformDirty)
        return WorldTransform;

    if (!GetParent())
        return GetTransformToParent();

    auto parentWorld = GetParent()->GetTransformToWorld();
    auto localMat = GetTransformToParent();
    WorldTransform = parentWorld * localMat;
    WorldTransformDirty = false;
    return WorldTransform;
}

const Matrix3x4& FNode::GetTransformFromWorld() const
{
    if (InverseWorldTransformDirty || WorldTransformDirty) {
        InverseWorldTransform = GetTransformToWorld().Inverse();
        InverseWorldTransformDirty = false;
    }
    return InverseWorldTransform;
}

Vector3 FNode::GetWorldTranslation() const
{
    return GetTransformToWorld().Translation();
}

Quaternion FNode::GetWorldRotation() const
{
    return GetTransformToWorld().Rotation();
}

Vector3 FNode::ConvertVectorFrom(const Vector3 &v, FNode *target) const
{
    auto world = target->GetTransformToWorld() * v;
    auto fromWorld = GetTransformFromWorld();
    return fromWorld * world;
}

Vector3 FNode::ConvertVectorTo(const Vector3 &v, FNode *target) const
{
    auto world = GetTransformToWorld() * v;
    auto fromWorld = target->GetTransformFromWorld();
    return fromWorld * world;
}

void FNode::MarkWorldTransformDirtyRecursively()
{
    WorldTransformDirty = true;
    for (TRefPtr<FNode> &ref : Children)
        ref->MarkWorldTransformDirtyRecursively();
}

void FNode::Rotate(const Quaternion &q, ETransformSpace space)
{
    switch (space)
    {
    case TS_LOCAL:
        Rotation = Rotation * q;
        Rotation.Normalize();
        break;
    case TS_PARENT:
        Rotation = q * Rotation;
        Rotation.Normalize();
        break;
    case TS_WORLD:
        if (!GetParent())
            Rotation = q * Rotation;
        else
        {
            Quaternion worldRotation = GetWorldRotation();
            Rotation = Rotation * worldRotation.Inverse() * q * worldRotation;
        }
        Rotation.Normalize();
        break;
    }

    TransformToParentDirty = true;
    MarkWorldTransformDirtyRecursively();
}

void FNode::Rotate(const Vector3 &axis, float angle, ETransformSpace space)
{
    Quaternion q;
    q.FromAngleAxis(angle, axis);
    Rotate(q, space);
}

void FNode::Yaw(float angle, ETransformSpace space)
{
    Rotate(Vector3::UNIT_Y, angle, space);
}

void FNode::Pitch(float angle, ETransformSpace space)
{
    Rotate(Vector3::UNIT_X, angle, space);
}

void FNode::Roll(float angle, ETransformSpace space)
{
    Rotate(Vector3::UNIT_Z, angle, space);
}

void FNode::LookAt(const Vector3 &at)
{
    auto localAt = GetTransformFromWorld() * at;
    Quaternion newRot;
    newRot.FromLookRotation(localAt);
    SetRotation(newRot);
}

FSceneNode::FSceneNode()
{
}

}
