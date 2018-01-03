#include "View.h"
#include "Scene.h"

namespace tc
{

FViewport::FViewport()
{
}

FViewport::~FViewport()
{
}

FSceneNode* FViewport::GetCameraNode() const
{
    return CameraNode;
}

void FViewport::SetCameraNode(FSceneNode* value)
{
    CameraNode = value;
}

void FViewport::Render()
{
    // Currently there is no optimization implemented

    // TODO: DeadCode
}

}
