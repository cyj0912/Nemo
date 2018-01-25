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

} /* namespace tc */
