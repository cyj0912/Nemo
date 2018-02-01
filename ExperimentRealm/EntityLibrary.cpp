#include "EntityLibrary.h"

namespace tc
{

FBaseEntity::~FBaseEntity()
{
}

const char* FBaseEntity::GetTypeNameInString() const
{
    static const char* name = "FBaseEntity";
    return name;
}

size_t FBaseEntity::CountSubentities() const
{
    return 0;
}

FBaseEntity* FBaseEntity::GetSubentity(size_t index)
{
    return nullptr;
}

} /* namespace tc */
