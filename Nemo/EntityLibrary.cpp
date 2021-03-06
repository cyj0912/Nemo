#include "EntityLibrary.h"
#include "EntityManager.h"

namespace tc
{

FBaseEntity::~FBaseEntity()
{
    if (Manager)
        Manager->UnregisterEntity(this);
}

const char* FBaseEntity::GetTypeNameInString() const
{
    static const char* name = "FBaseEntity";
    return name;
}

size_t FBaseEntity::CountSubEntities() const
{
    return 0;
}

FBaseEntity* FBaseEntity::GetSubEntity(size_t index)
{
    return nullptr;
}

void FBaseEntity::ImGuiUpdate(FInteractionSystem* interactionSystem)
{
}

} /* namespace tc */
