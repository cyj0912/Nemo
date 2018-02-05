#pragma once
#include "SceneMgrAPI.h"

#include <Pointer.h>
#include <UsingStl.h>
#include <Vector3.h>
#include <Quaternion.h>
#include <Matrix3x4.h>

namespace tc
{

enum ETransformSpace
{
    TS_LOCAL = 0,
    TS_PARENT,
    TS_WORLD
};

class SCENEMGR_API FNode : public FRefCount
{
    string Name;

    FNode *Parent, *RootNode;
    vector<TRefPtr<FNode>> Children;

    Vector3 Translation;
    Quaternion Rotation;
    Vector3 Scale;
    mutable Matrix3x4 TransformToParent, TransformFromParent, WorldTransform, InverseWorldTransform;
    mutable bool TransformToParentDirty, TransformFromParentDirty, WorldTransformDirty, InverseWorldTransformDirty;

    void SetRootNodeRecurse(FNode* rootNode)
    {
        RootNode = rootNode;
        for (auto& child : Children)
            child->SetRootNodeRecurse(rootNode);
    }

    void SetParent(FNode *value)
    {
        Parent = value;
        if (Parent)
            SetRootNodeRecurse(Parent->GetRootNode());
        else
            SetRootNodeRecurse(this);
    }

public:
    FNode();

    FNode(const FNode& rhs);

    FNode(FNode&& rhs) noexcept;

    FNode& operator= (const FNode& rhs);

    FNode& operator= (FNode&& rhs) noexcept;

    string GetName() const { return Name; }
    void SetName(const string &value) { Name = value; }

    FNode* GetParent() const { return Parent; }
    FNode* GetRootNode() const { return RootNode; }

    void AddChild(FNode *child);

    FNode* FindNodeByName(const string& name);

    size_t CountChildren() { return Children.size(); }

    FNode* ChildAt(size_t index)
    {
        return Children[index];
    }

    ptrdiff_t ChildToIndex(FNode* child)
    {
        for (size_t i = 0; i < Children.size(); ++i)
        {
            if (Children[i] == child)
                return i;
        }
        return -1;
    }

    const Matrix3x4& GetTransformToParent() const;
    const Matrix3x4& GetTransformFromParent() const;
    const Matrix3x4& GetTransformToWorld() const;
    const Matrix3x4& GetTransformFromWorld() const;

    Vector3 GetWorldTranslation() const;
    Quaternion GetWorldRotation() const;

    Vector3 ConvertVectorFrom(const Vector3 &v, FNode *target) const;
    Vector3 ConvertVectorTo(const Vector3 &v, FNode *target) const;

    void MarkWorldTransformDirtyRecursively();

    Vector3 GetTranslation() const { return Translation; }

    void SetTranslation(const Vector3 &value)
    {
        Translation = value;
        TransformToParentDirty = true;
        MarkWorldTransformDirtyRecursively();
    }

    void SetTranslation(float x, float y, float z)
    {
        Translation.x_ = x;
        Translation.y_ = y;
        Translation.z_ = z;
        TransformToParentDirty = true;
        MarkWorldTransformDirtyRecursively();
    }

    void Translate(float x, float y, float z)
    {
        Translation.x_ += x;
        Translation.y_ += y;
        Translation.z_ += z;
        TransformToParentDirty = true;
        MarkWorldTransformDirtyRecursively();
    }

    Quaternion GetRotation() const { return Rotation; }

    void SetRotation(const Quaternion &value)
    {
        Rotation = value;
        TransformToParentDirty = true;
        MarkWorldTransformDirtyRecursively();
    }

    void Rotate(const Quaternion& q, ETransformSpace space = TS_LOCAL);
    void Rotate(const Vector3& axis, float angle, ETransformSpace space = TS_LOCAL);
    void Yaw(float angle, ETransformSpace space = TS_LOCAL);
    void Pitch(float angle, ETransformSpace space = TS_LOCAL);
    void Roll(float angle, ETransformSpace space = TS_LOCAL);

    Vector3 GetScale() const { return Scale; }

    void SetScale(const Vector3 &value)
    {
        Scale = value;
        TransformToParentDirty = true;
        MarkWorldTransformDirtyRecursively();
    }

    void SetScale(float x, float y, float z)
    {
        Scale.x_ = x;
        Scale.y_ = y;
        Scale.z_ = z;
        TransformToParentDirty = true;
        MarkWorldTransformDirtyRecursively();
    }

    void LookAt(const Vector3 &at);
};

} /* namespace tc */
