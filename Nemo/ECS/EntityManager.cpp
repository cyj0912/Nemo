#include "EntityManager.h"
#include "Entity.h"
#include "ComponentStaticRegister.h"
#include "ComponentInterface.h"
#include <Log.h>

namespace tc
{

void FEntityManager::Init()
{
}

void FEntityManager::Shutdown()
{
    for (FEntityId id : EntitySet)
    {
        FEntityRef entity{id, this};
        for (IComponent* comp : ComponentSet)
        {
            if (comp->HasDataForEntity(entity))
                comp->RemoveEntity(entity);
        }
    }

    for (IComponent* comp : ComponentSet)
        delete comp;
    ComponentSet.clear();

    for (ISystem* system : SystemSet)
        delete system;
    SystemSet.clear();
}

FEntityId FEntityManager::AssignNewEntityId()
{
    while (IsEntity(NextIdToAssign))
        NextIdToAssign++;
    EntitySet.insert(NextIdToAssign);
    return NextIdToAssign++;
}

void FEntityManager::RegisterSystem(ISystem* system)
{
    SystemSet.insert(system);
    system->Init(this);
}

FComponentInfo* FEntityManager::FindComponentInfo(const std::string &name)
{
    FComponentInfo* info = ComponentInfoListHead;
    while (info)
    {
        if (name == string(info->Name))
            return info;
        info = info->ListNext;
    }
    LOGDEBUG("Component %s not found\n", name.c_str());
    return nullptr;
}

FComponentInfo* FEntityManager::FindComponentInfo(int id)
{
    FComponentInfo* info = ComponentInfoListHead;
    while (info)
    {
        if (id == info->Id)
            return info;
        info = info->ListNext;
    }
    LOGDEBUG("Component %d not found\n", id);
    return nullptr;
}

IComponent* FEntityManager::AddComponent(const std::string& name)
{
    IComponent* comp = FindComponentInfo(name)->Factory(this);
    ComponentSet.insert(comp);
    return comp;
}

IComponent *FEntityManager::AddComponent(int id)
{
    IComponent* comp = FindComponentInfo(id)->Factory(this);
    ComponentSet.insert(comp);
    return comp;
}

void FEntityManager::Tick(int dtMsec)
{
    for (auto t : SystemSet)
    {
        t->Tick(dtMsec);
    }
}

void FEntityManager::NotifyEntityAddedToComponent(const FEntityRef &entity, IComponent *component)
{
    for (auto t : SystemSet)
        t->NotifyEntityAddedToComponent(entity, component);
}

void FEntityManager::NotifyEntityRemovedFromComponent(const FEntityRef &entity, IComponent *component)
{
    for (auto t : SystemSet)
        t->NotifyEntityRemovedFromComponent(entity, component);
}

}
