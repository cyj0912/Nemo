#pragma once
#include "SceneMgrAPI.h"
#include "SceneNode.h"

#include <Foundation.h>

namespace tc
{

/*
 * *Tentative* plan for rendering sequence (Nov. 8, 2017)
 *     Technically, the SceneGraph lies at a rather low level since it
 * shouldn't need to know about implementation details of the
 * graphical/rendering subsystem. Future implementation of raytracing
 * may also base on SceneGraph without depending on the rendering
 * subsystem.
 *     But since only FScene knows about where everything is, it's only
 * reasonable to let FScene drive the rendering sequence. So the idea
 * is letting renderable objects expose an as simple as possible
 * interface. And FSceneNode shall call it just to notify the rendering
 * subsystem and be done. That said, FSceneNode does not assume the
 * rendering order and other details. Consequently, SceneNodes are not
 * responsible for sorting, etc.
 *     P.S. Is this really a good idea?
 * Thoughts:
 *     How about letting the camera sort the scene
 * Other questions:
 *     What distinguishes a subclass of SceneNode from an attachment?
 *     Are all attachments necessarily renderable?
 */

class FSceneNode;
class SCENEMGR_API FScene
{
    TRefPtr<FSceneNode> RootNode;
    TRefPtr<FSceneNode> ActiveCameraNode;

public:
    FScene();

    FSceneNode *GetRootNode() const
    {
        return RootNode;
    }

    void SetRootNode(FSceneNode *value)
    {
        RootNode = value;
    }

    const TRefPtr<FSceneNode> &GetActiveCameraNode() const
    {
        return ActiveCameraNode;
    }

    void SetActiveCameraNode(const TRefPtr<FSceneNode> &value)
    {
        ActiveCameraNode = value;
    }
};

}
