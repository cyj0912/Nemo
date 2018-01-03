#pragma once
#include "SceneMgrAPI.h"
#include "FrameRenderInfo.h"
#include <Matrix3x4.h>
#include <PODVector.h>
#include <Pointer.h>

namespace tc
{

class FSceneNode;

class SCENEMGR_API FSceneAttachment: public FRefCount
{
public:
    virtual void NotifyNewFrame(FSceneNode *node, FFrameRenderInfo *info)
    {}
    virtual void NotifyRender(FSceneNode *node, FFrameRenderInfo *info)
    {}
};

class SCENEMGR_API FBaseRenderable: public FSceneAttachment
{
protected:
    struct FInstance
    {
        Matrix3x4 ModelTrsfm;
        float DistFromCamera;
    };
    PODVector<FInstance> Instances;
public:
    FBaseRenderable();
    void NotifyNewFrame(FSceneNode *node, FFrameRenderInfo *info) override;
    void NotifyRender(FSceneNode *node, FFrameRenderInfo *info) override;
    virtual void Draw(FSceneNode* sceneNode) = 0;
};

}
