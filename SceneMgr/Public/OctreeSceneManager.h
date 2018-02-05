#pragma once
#include "SceneNode.h"

#include <BoundingBox.h>

namespace tc
{

class FOctree
{
public:
    explicit FOctree(FOctree* parent) : Parent(parent) {}

private:
    FOctree* Parent;
    BoundingBox Extent;
};

class FOctreeSceneNode : public FSceneNode
{
public:
};

class FOctreeSceneManager
{
public:
    FOctreeSceneManager();

private:
    FOctree* Octree;
    FOctreeSceneNode* RootNode;
};

} /* namespace tc */
