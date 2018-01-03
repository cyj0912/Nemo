#pragma once
#include "EntityCommon.h"

namespace tc
{

class FEntityManager;
class FEntityRef
{
    FEntityId Id;
    FEntityManager* Manager;

    //If set to true, we assume the id is valid
    bool bIsValid;
public:
    explicit FEntityRef() : Id(0), Manager(nullptr), bIsValid(false)
    {
    }

    explicit FEntityRef(FEntityId id, FEntityManager* manager) : Id(id), Manager(manager), bIsValid(true)
    {
    }

    FEntityId GetId() const
    {
        return Id;
    }

    bool IsValid() const;

    //Mostly just foward to EntityManager
};

}
