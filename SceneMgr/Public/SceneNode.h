#pragma once
#include "SceneMgrAPI.h"
#include "Camera.h"

#include <Foundation.h>
#include <Vector3.h>
#include <Quaternion.h>
#include <Matrix3x4.h>

#include <set>
#include <functional>
using std::set;
using std::function;

namespace tc
{

class FScene;

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

    Vector3 Translation, Scale;
    Quaternion Rotation;
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
    FNode(const FNode &) = delete;
    FNode(FNode &&) = delete;
    FNode &operator=(const FNode &) = delete;
    FNode &operator=(FNode &&) = delete;

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

class SCENEMGR_API FSceneNode: public FNode
{
    set<TRefPtr<FSceneAttachment>> Attachments;

    TRefPtr<FCamera> AttachedCamera;

public:
    FSceneNode();

    FSceneNode* CreateChild();
    FSceneNode* CreateChild(const string& name);

    void Attach(FSceneAttachment *attachment);
    void Detach(FSceneAttachment *attachment);
    void AttachCamera(FCamera* camera);
    void DetachCamera();
    FCamera* GetAttachedCamera()
    {
        return AttachedCamera;
    }
    void UpdateAttachments();

    template<typename Func> void ForEachNodeDeep(Func fn)
    {
        fn(this);
        int limit = CountChildren();
        for (int i = 0; i < limit; i++)
        {
            FNode* child = ChildAt(i);
            auto* scnNodeChild = static_cast<FSceneNode*>(child);
            scnNodeChild->ForEachNodeDeep(fn);
        }
    }

    template<typename Func> void ForEachAttachment(Func fn)
    {
        for (auto attachment : Attachments)
        {
            fn(attachment);
        }
    }

    template<typename T, typename Func> void ForEachAttachmentOfType(Func fn)
    {
        for (auto &attachment : Attachments)
        {
            T *ptr = dynamic_cast<T *>(attachment.Get());
            if (!ptr) {
                fn(this, ptr);
            }
        }
    }

    template<typename Func> void ForEachAttachmentDeep(Func fn)
    {
        for (auto attachment : Attachments)
        {
            fn(this, attachment);
        }
        int limit = CountChildren();
        for (int i = 0; i < limit; i++)
        {
            FNode* child = ChildAt(i);
            auto* scnNodeChild = static_cast<FSceneNode*>(child);
            scnNodeChild->ForEachAttachmentDeep(fn);
        }
    }
};

}
