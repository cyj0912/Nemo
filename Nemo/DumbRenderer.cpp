#include "DumbRenderer.h"
#include "RenderableDumbStaticModel.h"
#include "Shader.h"
#include "NemoRoot.h"
#include <Scene.h>

namespace tc
{

FDumbRenderer::FDumbRenderer()
{
}

bool FDumbRenderer::Init()
{
    return true;
}

bool FDumbRenderer::Shutdown()
{
    return true;
}

void FDumbRenderer::RenderFrame()
{
    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    auto* scn = GNemoRoot.GetMainScene();
    scn->GetRootNode()->ForEachAttachmentDeep([](FSceneNode* node, FSceneAttachment* attachment){
        auto* renderable = dynamic_cast<FBaseRenderable*>(attachment);
        if (renderable)
        {
            //LOGDEBUG("Found %s for node %s\n", renderable->GetName().c_str(), node->GetName().c_str());
            renderable->Draw(node);
        }
    });
}

void FDumbRenderer::SetRenderViewInfo(const FRenderViewInfo &value)
{
    RenderViewInfo = value;

    auto camNode = GNemoRoot.GetMainScene()->GetActiveCameraNode();
    if (camNode)
    {
        auto camera = camNode->GetAttachedCamera();
        if (camera)
        {
            LOGDEBUG("FDumbRenderer::SetRenderViewInfo updating main camera\n");
            camera->SetAspectRatio(value.AspectRatio);
        }
    }
}

}
