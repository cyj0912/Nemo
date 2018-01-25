#pragma once
#include "Component.h"

namespace tc
{

class FNode;

struct FTransformComponentData
{
    FNode* Node;
};

class FTransformComponent : public TComponent<FTransformComponentData, FTransformComponent>
{
public:
    static const int Id = 1;
    FTransformComponent(FEntityManager* mgr) : TComponent(mgr)
    {
    }
    void OnEntityAdded(FEntityRef& entity, FTransformComponentData* data);
    void OnEntityToBeRemoved(FEntityRef& entity, FTransformComponentData* data);
};

}
