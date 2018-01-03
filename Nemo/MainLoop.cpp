#include "Engine.h"
#include "EntityManager.h"
#include "Physics.h"
#include <Timeline.h>
#include <thread>
#include <chrono>

namespace tc
{

FHighResolutionClock GSystemClock, GMainClock;
FEntityManager GEntityManager;

void SetupWorld()
{
    auto* phySys = CreatePhysicsSystem(&GEntityManager);
    auto ent = GEntityManager.NewEntity();
}

void MainLoop()
{
    GSystemClock.Init();
    GMainClock.Init();
    SetupWorld();
    while (true)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(15));
    }
}

void FireMainLoop()
{
}

}
