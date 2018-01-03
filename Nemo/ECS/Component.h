#pragma once
#include "ComponentInterface.h"
#include <VectorPool.h>
#include <Foundation.h>

namespace tc
{

template <typename TD>
class TComponent : public IComponent
{
public:
    struct FComponentElement
    {
        FEntityId EntityId;
        TD Data;
    };
protected:
    TVectorPool<FComponentElement> ComponentPool;
    unordered_map<FEntityId, size_t> LookupFromEntityId;
public:
    //Not use(ed/ful) yet
    //typedef typename TVectorPool<FComponentElement>::Iterator iterator;
    //typedef TD value_type;

    TD* AddEntity(FEntityRef& entity)
    {
        assert(entity.IsValid());
        if (HasDataForEntity(entity))
            return GetComponentData(entity);
        auto compRef = ComponentPool.GetNewElement(kAddToBack);
        LookupFromEntityId[entity.GetId()] = compRef.index();
        compRef->EntityId = entity.GetId();
        return &compRef->Data;
    }

    TD* GetComponentData(const FEntityRef& entity)
    {
        return GetComponentDataAsVoid(entity);
    }

    void AddEntityGeneric(FEntityRef& entity) override
    {
        AddEntity(entity);
    }

    void RemoveEntity(FEntityRef& entity) override
    {
        if (!HasDataForEntity(entity))
            return;
        auto compIndex = LookupFromEntityId[entity.GetId()];
        ComponentPool.FreeElement(compIndex);
        LookupFromEntityId.erase(entity.GetId());
    }

    bool HasDataForEntity(const FEntityRef& entity) const override
    {
        assert(entity.IsValid());
        return LookupFromEntityId.find(entity.GetId()) != LookupFromEntityId.end();
    }

    void ClearComponentData() override
    {
        ComponentPool.Clear();
        LookupFromEntityId.clear();
    }

    void* GetComponentDataAsVoid(const FEntityRef& entity) override
    {
        assert(entity.IsValid());
        auto compIndex = LookupFromEntityId[entity.GetId()];
        return &ComponentPool.GetElementData(compIndex)->Data;
    }

    const void* GetComponentDataAsVoid(const FEntityRef& entity) const override;
};

} /* namespace tc */
