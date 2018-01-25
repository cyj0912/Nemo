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
 *
 * Design:
 *  Intend to support indefinitely many entities and components,
 *  but relatively fewer systems.
 */

struct FComponentInfo;
class FEntityRef;
class FEntityManager
{
    unordered_set<FEntityId> EntitySet;
    unordered_set<FEntityId> EntityMarkedForDeletion;
    unordered_set<ISystem*> SystemSet; //Owning
    unordered_set<IComponent*> ComponentSet; //Owning
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

    //Component static registration related helpers
    static FComponentInfo* FindComponentInfo(const string& name);
    static FComponentInfo* FindComponentInfo(int id);

    IComponent* AddComponent(const string& name);
    IComponent* AddComponent(int id);

    void Tick(int dtMsec);

    //Intended to be used by components, technically not part of the public interface.
    void NotifyEntityAddedToComponent(const FEntityRef& entity, IComponent* component);
    void NotifyEntityRemovedFromComponent(const FEntityRef& entity, IComponent* component);
};

}
