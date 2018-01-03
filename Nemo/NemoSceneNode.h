#pragma once
#include "Serialization.h"
#include <SceneNode.h>

namespace tc
{

class FNemoSceneNode : public FSceneNode
{
public:
    virtual void Reconstruct(const FYamlDeserializer* deserializer);
    virtual void PostReconstruct();
};

class FBSplineControl : public FNemoSceneNode
{
    Vector3 FrontPointOffset, BackPointOffset;

    class FSceneNodeRefByName
    {
        mutable TRefPtr<FBSplineControl> Target;
        string Name;
        FBSplineControl* Node;
    public:
        explicit FSceneNodeRefByName(FBSplineControl* node) : Node(node) {}

        void UpdateTarget() const
        {
            if (Name.empty())
                return;
            if (!Target)
            {
                auto* node = Node->GetRootNode()->FindNodeByName(Name);
                auto* target = dynamic_cast<FBSplineControl*>(node);
                if (!target)
                    throw "Uh oh";
                Target = target;
            }
        }

        operator FBSplineControl* () const { UpdateTarget(); return Target.Get(); }

        FBSplineControl& operator*() const
        {
            UpdateTarget();
            return Target.operator*();
        }

        FBSplineControl* operator->() const
        {
            UpdateTarget();
            return Target.operator->();
        }

        FSceneNodeRefByName& operator=(const string& name)
        {
            Name = name;
            Target.Clear();
            return *this;
        }

        operator bool() const { UpdateTarget(); return (bool)Target; }
    } FrontConnectTo;

public:
    FBSplineControl() : FNemoSceneNode(), FrontConnectTo(this)
    {
    }

    void Reconstruct(const FYamlDeserializer* deserializer) override;
    void PostReconstruct() override;

    const Vector3 &GetFrontPointOffset() const
    {
        return FrontPointOffset;
    }

    void SetFrontPointOffset(const Vector3 &value)
    {
        FrontPointOffset = value;
    }

    const Vector3 &GetBackPointOffset() const
    {
        return BackPointOffset;
    }

    void SetBackPointOffset(const Vector3 &value)
    {
        BackPointOffset = value;
    }
};

}
