#include "Engine.h"
#include "EntityManager.h"
#include "ComponentStaticRegister.h"
#include "Physics.h"
#include "SystemInterface.h"
#include "ComponentInterface.h"
#include "RenderSystem.h"
#include <Timeline.h>
#include <Log.h>
#include <thread>
#include <chrono>
#include <algorithm>
#include <atomic>

namespace tc
{

FHighResolutionClock GSystemClock, GMainClock;
FEntityManager GEntityManager;

std::atomic_int MainLoopFlag;

template <typename T, int C>
class FRunningAverage
{
    T RingBuffer[C];
    int NextIndex;
    int Count;
    T Result;
public:
    FRunningAverage()
    {
        NextIndex = 0;
        Count = 0;
    }

    void Feed(T value)
    {
        int i = NextIndex;

        RingBuffer[NextIndex++] = value;
        NextIndex += C;
        NextIndex %= C;
        if (Count < C)
            Count++;

        T sum = T();
        for (int k = 0; k < Count; k++)
        {
            sum += RingBuffer[i--];
            i += C;
            i %= C;
        }
        Result = sum / Count;
    }

    T Get()
    {
        return Result;
    }
};

void SetupWorld()
{
    FComponentInfo* info = ComponentInfoListHead;
    while (info)
    {
        LOGDEBUG("[Component] %s %d\n", info->Name, info->Id);
        info = info->ListNext;
    }
    //LOGDEBUG("[FBulletComponent] %d\n", FStaticComponentInfo<class FBulletComponent>::Id);

    GEntityManager.Init();

    auto* phySys = CreatePhysicsSystem(&GEntityManager);
    auto* renderSys = new FRenderSystem();
    GEntityManager.RegisterSystem(renderSys);
    auto* transformComp = GEntityManager.AddComponent("Transform");
    auto* geomComp = GEntityManager.AddComponent("Geometry");
    auto* physComp = GEntityManager.AddComponent("Physics");

    auto newEnt = GEntityManager.NewEntity();
    transformComp->AddEntityGeneric(newEnt);
    geomComp->AddEntityGeneric(newEnt);
    physComp->AddEntityGeneric(newEnt);
}

void MainLoop()
{
    GSystemClock.Init();
    GMainClock.Init();
    SetupWorld();
    while (true)
    {
        GEntityManager.Tick(16);
        std::this_thread::sleep_for(std::chrono::milliseconds(16));

        if (MainLoopFlag != 0)
            goto cleanup;
    }
cleanup:
    GEntityManager.Shutdown();
}

void FireMainLoop()
{
    MainLoopFlag = 0;
    std::thread mainThread(&MainLoop);
    mainThread.detach();
}

void StopMainLoop()
{
    MainLoopFlag = 1;
}

}
