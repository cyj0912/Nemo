#pragma once
#include "InputEvent.h"
#include <Foundation.h>
#include <queue>

namespace tc
{

class FScene;
class FDumbRenderer;

class FNemoRoot
{
    FScene* MainScene;
    FDumbRenderer* MainRenderer;
    FPerfTimer RootTimer;
    queue<FInputEvent> InputEventQueue;

public:
    bool Init();
    bool InitGL(void* (*glLoader)(const char *name));
    bool Shutdown();
    FScene* GetMainScene();
    FDumbRenderer* GetMainRenderer();
    FPerfTimer* GetRootTimer();
    void StampAndQueueInputEvent(FInputEvent event);

    void RenderScene();
};

extern FNemoRoot GNemoRoot;

}
