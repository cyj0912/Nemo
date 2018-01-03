#include "Scene.h"
namespace tc
{

FScene::FScene()
{
    auto* rootNode = new FSceneNode();
    rootNode->Scene = this;
    SetRootNode(rootNode);
}

}
