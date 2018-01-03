#include "SceneNode.h"
#include "Scene.h"

namespace tc
{

FSceneNode::FSceneNode()
    : Scene(nullptr), Scale(1.0f, 1.0f, 1.0f), TransformToParentDirty(true), TransformFromParentDirty(true),
      WorldTransformDirty(true), InverseWorldTransformDirty(true), Parent(nullptr), RootNode(this)
{
    SetName("unnamedNode");
}

Matrix3x4 FSceneNode::GetTransformToParent() const
{
    if (TransformToParentDirty) {
        TransformToParent = Matrix3x4(Translation, Rotation, Scale);
        TransformToParentDirty = false;
    }
    return TransformToParent;
}

Matrix3x4 FSceneNode::GetTransformFromParent() const
{
    //This is based on TransformToParent, check if that's dirty first
    if (TransformFromParentDirty || TransformToParentDirty) {
        TransformFromParent = GetTransformToParent().Inverse();
        TransformFromParentDirty = false;
    }
    return TransformFromParent;
}

Matrix3x4 FSceneNode::GetTransformToWorld() const
{
    if (!WorldTransformDirty)
        return WorldTransform;

    if (!GetParent()) {
        return GetTransformToParent();
    }

    auto parentWorld = GetParent()->GetTransformToWorld();
    auto localMat = GetTransformToParent();
    WorldTransform = parentWorld * localMat;
    WorldTransformDirty = false;
    return WorldTransform;
}

Matrix3x4 FSceneNode::GetTransformFromWorld() const
{
    if (InverseWorldTransformDirty || WorldTransformDirty) {
        InverseWorldTransform = GetTransformToWorld().Inverse();
        InverseWorldTransformDirty = false;
    }
    return InverseWorldTransform;
}

Vector3 FSceneNode::GetWorldTranslation() const
{
    return GetTransformToWorld().Translation();
}

void FSceneNode::MarkWorldTransformDirtyRecursively()
{
    WorldTransformDirty = true;
    for (TRefPtr<FSceneNode> &ref : Children)
        ref->MarkWorldTransformDirtyRecursively();
}

void FSceneNode::Rotate(const Quaternion &q)
{
    Rotation = Rotation * q;
    Rotation.Normalize();
    TransformToParentDirty = true;
    MarkWorldTransformDirtyRecursively();
}

void FSceneNode::Rotate(const Vector3 &axis, float angle)
{
    Quaternion q;
    q.FromAngleAxis(angle, axis);
    Rotate(q);
}

void FSceneNode::Yaw(float angle)
{
    Rotate(Vector3::UNIT_Y, angle);
}

void FSceneNode::Pitch(float angle)
{
    Rotate(Vector3::UNIT_X, angle);
}

void FSceneNode::Roll(float angle)
{
    Rotate(Vector3::UNIT_Z, angle);
}

void FSceneNode::LookAt(const Vector3 &at)
{
    auto localAt = ConvertVectorFrom(at, GetRootNode());
    Quaternion newRot;
    newRot.FromLookRotation(localAt);
    SetRotation(newRot);
}

Vector3 FSceneNode::ConvertVectorFrom(const Vector3 &v, FSceneNode *target) const
{
    auto world = target->GetTransformToWorld() * v;
    auto fromWorld = GetTransformFromWorld();
    return fromWorld * world;
}

Vector3 FSceneNode::ConvertVectorTo(const Vector3 &v, FSceneNode *target) const
{
    auto world = GetTransformToWorld() * v;
    auto fromWorld = target->GetTransformFromWorld();
    return fromWorld * world;
}

void FSceneNode::AddChild(FSceneNode *child)
{
    TRefPtr<FSceneNode> reference(child, true);
    Children.push_back(reference);
    child->SetParent(this);
}

FSceneNode *FSceneNode::CreateChild()
{
    auto *node = new FSceneNode();
    AddChild(node);
    return node;
}

FSceneNode *FSceneNode::CreateChild(const string& name)
{
    auto *node = new FSceneNode();
    node->SetName(name);
    AddChild(node);
    return node;
}


//TODO I'm still thinking how to do caching.
FSceneNode* FSceneNode::FindNodeByName(const string &name)
{
    if (Name == name)
        return this;
    for (auto child : Children)
    {
        auto* result = child->FindNodeByName(name);
        if (result)
            return result;
    }
    return nullptr;
}

void FSceneNode::Attach(FSceneAttachment *attachment)
{
    Attachments.insert(TRefPtr<FSceneAttachment>(attachment));
}

void FSceneNode::Detach(FSceneAttachment *attachment)
{
    Attachments.erase(TRefPtr<FSceneAttachment>(attachment));
}

void FSceneNode::AttachCamera(FCamera* camera)
{
    if (AttachedCamera)
        LOGDEBUG("SceneNode %s, replace attached camera\n", GetName().c_str());
    AttachedCamera = camera;
}

void FSceneNode::DetachCamera()
{
    AttachedCamera = nullptr;
}

void FSceneNode::UpdateAttachments()
{
    for (auto attachment : Attachments) {
        //attachment->SetModelTransform(GetTransformToWorld());
    }
}

}
