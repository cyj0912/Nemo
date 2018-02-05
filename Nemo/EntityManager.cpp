#include "EntityManager.h"
#include "EntityLibrary.h"

namespace tc
{

FEntityManager::~FEntityManager()
{
    //Make a copy of Entities because Unregister function modifies it
    auto copyOfEntities = Entities;
    for (auto* entity : copyOfEntities)
        if (!MapIsSubEntity[entity])
            delete entity;
}

void FEntityManager::RegisterEntity(FBaseEntity* entity, bool bIsSub)
{
    entity->SetEntityManager(this);
    Entities.insert(entity);
    MapIsSubEntity[entity] = bIsSub;
    if (entity->CountSubEntities())
        for (size_t i = 0; i < entity->CountSubEntities(); i++)
        {
            RegisterEntity(entity->GetSubEntity(i), true);
        }
}

void FEntityManager::UnregisterEntity(FBaseEntity* entity)
{
    //Warning: MapIsSubEntity is not updated on removal, is it really a good idea?
    entity->SetEntityManager(nullptr);
    auto iter = Entities.find(entity);
    if (iter != Entities.end())
        Entities.erase(iter);
}

} /* namespace tc */
