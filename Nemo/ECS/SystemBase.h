#pragma once
#include "SystemInterface.h"
#include <UsingStl.h>

namespace tc
{

class FSystem : public ISystem
{
    FEntityManager* Manager;
    int TrackedComponents[32];
    unordered_set<int> TrackedComponentLookup;
    unordered_map<FEntityId, uint32_t> EntToComp;

protected:
    void TrackComponent(int id);
    void TrackComponent(string name);
    bool IsComponentTracked(IComponent* component);
    uint32_t GetComponentBit(IComponent* component);
    uint32_t GetTrackedBitset();

public:
    void Init(FEntityManager* manager) override;
    void Shutdown() override;
    void NotifyEntityAddedToComponent(const FEntityRef& entity, IComponent* component) override;
    void NotifyEntityRemovedFromComponent(const FEntityRef& entity, IComponent* component) override;
    void Tick(int deltaMsec) override;
};

} /* namespace tc */
