#pragma once
#include "Component.h"

namespace tc
{

struct FGeometryComponentData
{
};

class FGeometryComponent : public TComponent<FGeometryComponentData, FGeometryComponent>
{
public:
    static const int Id = 2;
    FGeometryComponent(FEntityManager* mgr) : TComponent(mgr)
    {
    }
    void OnEntityAdded(FEntityRef& entity, FGeometryComponentData* data);
    void OnEntityToBeRemoved(FEntityRef& entity, FGeometryComponentData* data);
};

}
