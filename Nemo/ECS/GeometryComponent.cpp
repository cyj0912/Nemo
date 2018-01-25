#include "GeometryComponent.h"
#include "ComponentStaticRegister.h"

namespace tc
{

REGISTER_COMPONENT(Geometry, FGeometryComponent);

void FGeometryComponent::OnEntityAdded(FEntityRef &entity, FGeometryComponentData *data)
{
}

void FGeometryComponent::OnEntityToBeRemoved(FEntityRef &entity, FGeometryComponentData *data)
{
}

}
