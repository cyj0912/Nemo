#include "EntityManager.h"
#include "EntityLibrary.h"

namespace tc
{

FEntityManager::~FEntityManager()
{
    for (auto* entity : Entities)
        delete entity;
}

void FEntityManager::RegisterEntity(FBaseEntity* entity)
{
    Entities.insert(entity);
    if (entity->CountSubEntities())
        for (size_t i = 0; i < entity->CountSubEntities(); i++)
        {
            RegisterEntity(entity->GetSubEntity(i));
        }
}

} /* namespace tc */
