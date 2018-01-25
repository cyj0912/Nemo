#include "Scene.h"
namespace tc
{

FScene::FScene()
{
    auto* rootNode = new FSceneNode();
    SetRootNode(rootNode);
}

}
