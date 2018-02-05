#pragma once

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

class FOctreeSceneManager
{
public:
    FOctreeSceneManager();

private:
    FOctree* Octree;
};

} /* namespace tc */
