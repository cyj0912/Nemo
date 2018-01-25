#include "RenderSystem.h"

void tc::FRenderSystem::Init(tc::FEntityManager *manager)
{
    FSystem::Init(manager);
    TrackComponent("Transform");
    TrackComponent("Geometry");
}

void tc::FRenderSystem::Shutdown()
{
    FSystem::Shutdown();
}

void tc::FRenderSystem::Tick(int deltaMsec)
{
    FSystem::Tick(deltaMsec);
}
