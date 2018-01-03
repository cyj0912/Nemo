#include "NemoSceneNode.h"
#include "NodeFactory.h"
#include "RenderableDumbStaticModel.h"
#include "BezierSegment.h"
#include <ResourceManager.h>
#include <StaticMesh.h>

namespace tc
{

void FNemoSceneNode::Reconstruct(const FYamlDeserializer *deserializer)
{
    SetName(deserializer->Deserialize<string>("Name"));
    SetTranslation(deserializer->Deserialize<Vector3>("Translation"));
    SetRotation(deserializer->Deserialize<Quaternion>("Rotation"));
    SetScale(deserializer->Deserialize<Vector3>("Scale"));
    auto deserializerArray = deserializer->GetArray("Children");
    for (auto *childDeserializer : deserializerArray)
    {
        string kind = childDeserializer->Deserialize<string>("Kind");
        FNemoSceneNode* childNode = FNodeFactory::NewSceneNode(kind);
        AddChild(childNode);
        childNode->Reconstruct(childDeserializer);
        delete childDeserializer;
    }
}

void FNemoSceneNode::PostReconstruct()
{
}

void FBSplineControl::Reconstruct(const FYamlDeserializer *deserializer)
{
    FNemoSceneNode::Reconstruct(deserializer);

    SetFrontPointOffset(deserializer->Deserialize<Vector3>("FrontPointOffset"));
    SetBackPointOffset(deserializer->Deserialize<Vector3>("BackPointOffset"));
    FrontConnectTo = deserializer->Deserialize<string>("FrontConnectTo");

    TRefPtr<RStaticMesh> staticMesh =
        GResourceManager.GetResource<RStaticMesh>("ball-d=1.obj");
    auto* dumbModel = new FRenderableDumbStaticModel(staticMesh);
    Attach(dumbModel);
}

void FBSplineControl::PostReconstruct()
{
    FNemoSceneNode::PostReconstruct();
    if (FrontConnectTo)
    {
        auto* segment = new FBezierSegment(this, FrontConnectTo);
        this->Attach(segment);
        FrontConnectTo->Attach(segment);
    }
}

}
