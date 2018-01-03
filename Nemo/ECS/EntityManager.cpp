#include "EntityManager.h"
#include "Entity.h"

namespace tc
{

FEntityId FEntityManager::AssignNewEntityId()
{
    while (IsEntity(NextIdToAssign))
        NextIdToAssign++;
    return NextIdToAssign++;
}

void FEntityManager::RegisterSystem(ISystem* system)
{
    SystemSet.insert(system);
    system->Init(this);
}

}
