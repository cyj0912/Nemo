#pragma once
#include "EntityCommon.h"
#include "Entity.h"

namespace tc
{

class IComponent
{
public:
    virtual ~IComponent() {}
    virtual void AddEntityGeneric(FEntityRef& entity) = 0;
    virtual void RemoveEntity(FEntityRef& entity) = 0;
    virtual bool HasDataForEntity(const FEntityRef& entity) const = 0;
    virtual void ClearComponentData() = 0;
    virtual void* GetComponentDataAsVoid(const FEntityRef& entity) = 0;
    virtual const void* GetComponentDataAsVoid(const FEntityRef& entity) const = 0;
};

}
