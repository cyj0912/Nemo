#include "EntityManager.h"
#include "EntityLibrary.h"

namespace tc
{

FEntityManager::~FEntityManager()
{
    for (auto* entity : Entities)
        if (!MapIsSubEntity[entity])
            delete entity;
}

void FEntityManager::RegisterEntity(FBaseEntity* entity, bool bIsSub)
{
    Entities.insert(entity);
    MapIsSubEntity[entity] = bIsSub;
    if (entity->CountSubEntities())
        for (size_t i = 0; i < entity->CountSubEntities(); i++)
        {
            RegisterEntity(entity->GetSubEntity(i), true);
        }
}

} /* namespace tc */
