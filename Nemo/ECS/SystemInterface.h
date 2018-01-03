#pragma once
#include "Entity.h"

namespace tc
{

class FEntityManager;
class IComponent;
class ISystem
{
public:
    virtual ~ISystem() {}
    virtual void Init(FEntityManager* manager) = 0;
    virtual void Shutdown() = 0;
    virtual void NotifyEntityAddedToComponent(const FEntityRef& entity, IComponent* component) = 0;
    virtual void NotifyEntityRemovedFromComponent(const FEntityRef& entity, IComponent* component) = 0;
    virtual void Tick(int deltaMsec) = 0;
};

}
