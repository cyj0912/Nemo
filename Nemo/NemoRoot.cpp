#include "NemoRoot.h"
#include "RenderableDumbStaticModel.h"
#include "DumbRenderer.h"
#include "Engine.h"
#include <Scene.h>
#include <Camera.h>
#include <Mesh.h>
#include <StaticMesh.h>
#include <ResourceManager.h>

#include <glad/glad.h>

namespace tc
{

bool FNemoRoot::Init()
{
    RootTimer.Init();
    MainScene = new FScene();
    return true;
}

bool FNemoRoot::InitGL(void *(*glLoader)(const char *))
{
    if (!gladLoadGLLoader(glLoader))
        return false;
    LOGINFO("Glad loaded successfully\n");
    LOGINFO("OpenGL %d.%d\n", GLVersion.major, GLVersion.minor);

    LOGINFO("GLAD_GL_ARB_bindless_texture %d\n", GLAD_GL_ARB_bindless_texture);

    auto* mainCam = new FCamera();
    auto* camPivot = GetMainScene()->GetRootNode()->CreateChild("cameraPivotNode");
    auto* camNode = camPivot->CreateChild("cameraNode");
    camNode->SetTranslation(0.0f, 20.0f, 20.0f);
    camNode->LookAt(Vector3::ZERO);
    camNode->AttachCamera(mainCam);
    GetMainScene()->SetActiveCameraNode(camNode);

    RootTimer.Tick();

    FireMainLoop();

    MainRenderer = new FDumbRenderer();
    return true;
}

bool FNemoRoot::Shutdown()
{
    StopMainLoop();
    MainRenderer->Shutdown();
    delete MainRenderer;
    delete MainScene;
    return true;
}

FScene *FNemoRoot::GetMainScene()
{
    return MainScene;
}

FDumbRenderer* FNemoRoot::GetMainRenderer()
{
    return MainRenderer;
}

FPerfTimer* FNemoRoot::GetRootTimer()
{
    return &RootTimer;
}

void FNemoRoot::StampAndQueueInputEvent(FInputEvent event)
{
    // every type has type and timestamp, might as well assume one
    event.MouseMotion.TimeStamp = 0;
    //InputEventQueue.push(event);
}

void FNemoRoot::RenderScene()
{
    //High level: Scene level
    RootTimer.Tick();
    MainRenderer->RenderFrame();
}

FNemoRoot GNemoRoot;

}
