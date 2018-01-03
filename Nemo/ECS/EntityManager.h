#pragma once
#include "EntityCommon.h"
#include "SystemInterface.h"
#include "Entity.h"
#include <UsingStl.h>

namespace tc
{

/* Operations needed
 * Entity to Component of type/name/id
 * Entity to Components
 * Component to Entities
 */

class FEntityRef;
class FEntityManager
{
    unordered_set<FEntityId> EntitySet;
    unordered_set<FEntityId> EntityMarkedForDeletion;
    unordered_set<ISystem*> SystemSet; //Owning
    FEntityId NextIdToAssign;
public:
    FEntityManager() : NextIdToAssign(0)
    {
    }

    void Init();
    void Shutdown();

    bool IsEntity(FEntityId id) const
    {
        return EntitySet.find(id) != EntitySet.end();
    }

    bool IsEntityMarkedForDeletion(FEntityId id) const
    {
        return EntityMarkedForDeletion.find(id) != EntityMarkedForDeletion.end();
    }

    FEntityId AssignNewEntityId();

    FEntityRef NewEntity()
    {
        return FEntityRef(AssignNewEntityId(), this);
    }

    void RegisterSystem(ISystem* system);
};

}
