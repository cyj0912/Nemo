#include "SceneAttachment.h"
#include "SceneNode.h"

namespace tc
{

FBaseRenderable::FBaseRenderable()
{
    Instances.Reserve(4);
}

void FBaseRenderable::NotifyNewFrame(FSceneNode *node, FFrameRenderInfo* info)
{
    Instances.Clear();
}

void FBaseRenderable::NotifyRender(FSceneNode *node, FFrameRenderInfo* info)
{
    FInstance inst;
    inst.ModelTrsfm = node->GetTransformToWorld();
    Instances.Push(inst);
}

}
