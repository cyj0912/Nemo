#include "Entity.h"
#include "EntityManager.h"

namespace tc
{

bool FEntityRef::IsValid() const
{
    return bIsValid && Manager && Manager->IsEntity(Id);
}

}
