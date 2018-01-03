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
class SCENEMGR_API FSceneNode: public FRefCount
{
    friend class FScene;
    string Name;
    FScene* Scene;

    Vector3 Translation, Scale;
    Quaternion Rotation;
    mutable Matrix3x4 TransformToParent, TransformFromParent, WorldTransform, InverseWorldTransform;
    mutable bool TransformToParentDirty, TransformFromParentDirty, WorldTransformDirty, InverseWorldTransformDirty;

    FSceneNode *Parent, *RootNode;
    vector<TRefPtr<FSceneNode>> Children;
    set<TRefPtr<FSceneAttachment>> Attachments;

    TRefPtr<FCamera> AttachedCamera;

    void SetParent(FSceneNode *value)
    {
        Parent = value;
        if (Parent)
            SetRootNodeAndSceneRecurse(Parent->GetRootNode(), Parent->GetScene());
        else
            SetRootNodeAndSceneRecurse(this, nullptr);
    }

    void SetRootNodeAndSceneRecurse(FSceneNode* rootNode, FScene* scene)
    {
        RootNode = rootNode;
        Scene = scene;
        for (auto& child : Children)
            child->SetRootNodeAndSceneRecurse(rootNode, scene);
    }

public:
    FSceneNode();
    FSceneNode(const FSceneNode &) = delete;
    FSceneNode(FSceneNode &&) = delete;
    FSceneNode &operator=(const FSceneNode &) = delete;
    FSceneNode &operator=(FSceneNode &&) = delete;

    string GetName() const
    {
        return Name;
    }

    void SetName(const string &value)
    {
        Name = value;
    }

    FScene *GetScene() const
    {
        return Scene;
    }

    Matrix3x4 GetTransformToParent() const;
    Matrix3x4 GetTransformFromParent() const;
    Matrix3x4 GetTransformToWorld() const;
    Matrix3x4 GetTransformFromWorld() const;

    Vector3 GetWorldTranslation() const;

    void MarkWorldTransformDirtyRecursively();

    Vector3 GetTranslation() const
    { return Translation; }
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

    Quaternion GetRotation() const
    { return Rotation; }
    void SetRotation(const Quaternion &value)
    {
        Rotation = value;
        TransformToParentDirty = true;
        MarkWorldTransformDirtyRecursively();
    }

    void Rotate(const Quaternion& q);
    void Rotate(const Vector3& axis, float angle);
    void Yaw(float angle);
    void Pitch(float angle);
    void Roll(float angle);

    Vector3 GetScale() const
    { return Scale; }
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

    Vector3 ConvertVectorFrom(const Vector3 &v, FSceneNode *target) const;
    Vector3 ConvertVectorTo(const Vector3 &v, FSceneNode *target) const;

    FSceneNode *GetRootNode() const
    { return RootNode; }
    FSceneNode *GetParent() const
    { return Parent; }

    void AddChild(FSceneNode *child);
    FSceneNode* CreateChild();
    FSceneNode* CreateChild(const string& name);

    FSceneNode* FindNodeByName(const string& name);

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
        for (TRefPtr<FSceneNode> &child : Children)
        {
            child->ForEachNodeDeep(fn);
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
        for (auto& child : Children)
        {
            child->ForEachAttachmentDeep(fn);
        }
    }

    size_t CountChildren() { return Children.size(); }

    FSceneNode* ChildAt(size_t index)
    {
        return Children[index];
    }

    ptrdiff_t ChildToIndex(FSceneNode* child)
    {
        for (size_t i = 0; i < Children.size(); ++i)
        {
            if (Children[i] == child)
                return i;
        }
        return -1;
    }
};

}
