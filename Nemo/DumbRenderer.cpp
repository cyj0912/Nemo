#include "DumbRenderer.h"
#include "RenderableDumbStaticModel.h"
#include "Shader.h"
#include "NemoRoot.h"
#include <Scene.h>
#include <Camera.h>
#include <Vector4.h>

namespace tc
{

class FRenderHelper
{
    TRefPtr<FCamera> ActiveCamera;
public:
    void SetActiveCamera(FCamera* cam)
    {
        ActiveCamera = cam;
    }

    //void DrawLine()
};

class FDumbRendererPrivData
{
public:
    GLuint Color0Tex;
    GLuint Framebuffer;
};

FDumbRenderer::FDumbRenderer() : bHasRenderViewInfo(false)
{
}

bool FDumbRenderer::Init()
{
    Pd = new FDumbRendererPrivData();
    RenderHelper = new FRenderHelper();
    return true;
}

bool FDumbRenderer::Shutdown()
{
    delete RenderHelper;
    delete Pd;
    return true;
}

void FDumbRenderer::RenderFrame()
{
    if (!bHasRenderViewInfo)
        return;

    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    //Draw grid


    auto* scn = GNemoRoot.GetMainScene();
    scn->GetRootNode()->ForEachAttachmentDeep([](FSceneNode* node, FSceneAttachment* attachment){
        auto* renderable = dynamic_cast<FBaseRenderable*>(attachment);
        if (renderable)
        {
            renderable->Draw(node);
        }
    });
}

void FDumbRenderer::SetRenderViewInfo(const FRenderViewInfo &value)
{
    bHasRenderViewInfo = true;
    RenderViewInfo = value;

//    glGenTextures(1, &Pd->Color0Tex);
//    glBindTexture(GL_TEXTURE_2D, Pd->Color0Tex);
//    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, RenderViewInfo.Width, RenderViewInfo.Height,
//                 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

//    glGenFramebuffers(1, &Pd->Framebuffer);
//    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, Pd->Framebuffer);
//    glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, Pd->Color0Tex, 0);

//    GLenum drawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
//    glDrawBuffers(1, drawBuffers);

//    if (glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
//        LOGERROR("glCheckFramebufferStatus not ok\n");

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

void FDumbRenderer::SetInitialRenderTarget(FRenderTarget *target)
{
    InitRenderTarget = target;
}

}
