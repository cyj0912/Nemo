#pragma once
#include "SceneMgrAPI.h"
#include <Foundation.h>

namespace tc
{

class FSceneNode;
class FScene;
class SCENEMGR_API FViewport
{
    FScene *Scene;
    TRefPtr<FSceneNode> CameraNode;
public:
    FViewport();
    ~FViewport();
    FSceneNode *GetCameraNode() const;
    void SetCameraNode(FSceneNode *value);
    void Render(); //Tentative
};

}
