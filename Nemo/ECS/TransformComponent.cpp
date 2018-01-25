#include "TransformComponent.h"
#include "ComponentStaticRegister.h"
#include <SceneNode.h>

namespace tc
{

REGISTER_COMPONENT(Transform, FTransformComponent);

void FTransformComponent::OnEntityAdded(FEntityRef &entity, FTransformComponentData *data)
{
    data->Node = new FNode();
}

void FTransformComponent::OnEntityToBeRemoved(FEntityRef &entity, FTransformComponentData *data)
{
    if (data->Node)
        delete data->Node;
}

}
