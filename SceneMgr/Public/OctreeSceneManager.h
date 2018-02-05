#pragma once
#include "SceneNode.h"
#include "SceneManager.h"
#include "Geometry.h"

#include <BoundingBox.h>

namespace tc
{

class FOctreeSceneNode;
class FOctreeSceneManager;

class FOctree
{
public:
    explicit FOctree(FOctree* parent) : Parent(parent), Children{0}
    {
    }

    FOctree* GetParent() const
    {
        return Parent;
    }

    const BoundingBox& GetExtent() const
    {
        return Extent;
    }

    void SetExtent(const BoundingBox& value)
    {
        Extent = value;
    }

    void AddNode(FOctreeSceneNode* node);

    void RemoveNode(FOctreeSceneNode* node);

    bool CanFitInChild(const BoundingBox& box) const;

    FOctree* WhichOctant(const BoundingBox& box);

private:
    FOctree* Parent;
    FOctree* Children[2][2][2];
    BoundingBox Extent;

    vector<FOctreeSceneNode*> Nodes;
};

class FOctreeSceneNode : public FSceneNode
{
public:
    FOctree* GetOctant() const
    {
        return Octant;
    }

    void SetOctant(FOctree* value)
    {
        Octant = value;
    }

    void UpdateBoundingBox()
    {
        LocalAABB.Define(M_INFINITY, -M_INFINITY);
        WorldAABB.Define(M_INFINITY, -M_INFINITY);

        FGeometryListConstIter iter = GeometryBegin();
        for (; iter != GeometryEnd(); iter++)
        {
            BoundingBox boundingBox = (*iter)->GetBoundingBox();
            LocalAABB.Merge(boundingBox);
            boundingBox.Transform(GetTransformToWorld());
            WorldAABB.Merge(boundingBox);
        }
    }

    const BoundingBox& GetLocalAABBWithoutUpdate() const
    {
        return LocalAABB;
    }

    const BoundingBox& GetWorldAABBWithoutUpdate() const
    {
        return WorldAABB;
    }

public:
    FOctreeSceneManager* SceneManager;
    FOctree* Octant = nullptr;
    BoundingBox LocalAABB, WorldAABB;
};

class FOctreeSceneManager : public FSceneManager
{
public:
    FOctreeSceneManager();

    FOctreeSceneNode* CreateSceneNode();

    FOctreeSceneNode* CreateSceneNode(const string& name);

    void UpdateNodeInOctree(FOctreeSceneNode* node);

    void RemoveNodeFromOctree(FOctreeSceneNode* node);

    void InsertNodeIntoOctree(FOctreeSceneNode* node, FOctree* treeNode, int currentDepth = 0);

private:
    FOctree* Octree;
    int MaxDepth;
};

} /* namespace tc */
