#pragma once
#include "ComponentInterface.h"
#include "EntityManager.h"
#include <VectorPool.h>
#include <Foundation.h>

namespace tc
{

template <typename TD, typename Child = int>
class TComponent : public IComponent
{
    FEntityManager* Manager;
protected:
    TVectorPool<TD> ComponentPool;
    mutable unordered_map<FEntityId, size_t> LookupFromEntityId;
public:
    //Not use(ed/ful) yet
    //typedef typename TVectorPool<FComponentElement>::Iterator iterator;
    //typedef TD value_type;

    TComponent(FEntityManager* mgr) : Manager(mgr)
    {
    }

    int GetId() const override
    {
        return Child::Id;
    }

    TD* AddEntity(FEntityRef& entity)
    {
        assert(entity.IsValid());
        if (HasDataForEntity(entity))
            return GetComponentData(entity);
        auto compRef = ComponentPool.GetNewElement(kAddToBack);
        LookupFromEntityId[entity.GetId()] = compRef.index();
        TD* dataPtr = &*compRef;
        static_cast<Child*>(this)->OnEntityAdded(entity, dataPtr);
        Manager->NotifyEntityAddedToComponent(entity, this);
        return dataPtr;
    }

    TD* GetComponentData(const FEntityRef& entity)
    {
        assert(entity.IsValid());
        auto compIndex = LookupFromEntityId[entity.GetId()];
        return ComponentPool.GetElementData(compIndex);
    }

    const TD* GetComponentData(const FEntityRef& entity) const
    {
        return const_cast<TComponent*>(this)->GetComponentData(entity);
    }

    void AddEntityGeneric(FEntityRef& entity) override
    {
        AddEntity(entity);
    }

    void RemoveEntity(FEntityRef& entity) override
    {
        Manager->NotifyEntityRemovedFromComponent(entity, this);
        if (!HasDataForEntity(entity))
            return;
        auto compIndex = LookupFromEntityId[entity.GetId()];
        static_cast<Child*>(this)->OnEntityToBeRemoved(entity, GetComponentData(entity));
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
        return GetComponentData(entity);
    }

    const void* GetComponentDataAsVoid(const FEntityRef& entity) const override
    {
        return GetComponentData(entity);
    }
};

} /* namespace tc */
