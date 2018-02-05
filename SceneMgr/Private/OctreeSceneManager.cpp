/*
 * Copyright (c) 2017-2018 Toby Chen
 *
 * This file is heavily influenced/inspired by Ogre 3D.
 * Its copyright notice is hereby reproduced.
 *
 * Copyright (c) 2000-2014 Torus Knot Software Ltd
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "OctreeSceneManager.h"

namespace tc
{

void FOctree::AddNode(FOctreeSceneNode* node)
{
    Nodes.push_back(node);
    node->SetOctant(this);
}

void FOctree::RemoveNode(FOctreeSceneNode* node)
{
    Nodes.erase(find(Nodes.begin(), Nodes.end(), node));
    node->SetOctant(nullptr);
}

bool FOctree::CanFitInChild(const BoundingBox& box) const
{
    //We implement a loose octree with k=2
    //According to the original paper, box fits in a child iff. l <= L/4 = 1/2*W*(2^d)
    // W*(2^d) is the same thing as the width of current level
    auto halfActualL = GetExtent().HalfSize();
    auto boxSize = box.Size();
    return boxSize.x_ <= halfActualL.x_ && boxSize.y_ <= halfActualL.y_ && boxSize.z_ <= halfActualL.z_;
}

FOctree* FOctree::WhichOctant(const BoundingBox& box)
{
    auto center = GetExtent().Center();
    auto boxCenter = box.Center();
    int x = 0, y = 0, z = 0;
    if (boxCenter.x_ > center.x_)
        x = 1;
    if (boxCenter.y_ > center.y_)
        y = 1;
    if (boxCenter.z_ > center.z_)
        z = 1;

    if (Children[x][y][z] == nullptr)
    {
        //Create child Octree
        auto& extent = GetExtent();
        auto extentMiddle = extent.Center();
        Vector3 min, max;
        if (x == 0)
        {
            min.x_ = extent.min_.x_;
            max.x_ = extentMiddle.x_;
        }
        else
        {
            min.x_ = extentMiddle.x_;
            max.x_ = extent.max_.x_;
        }

        if (y == 0)
        {
            min.y_ = extent.min_.y_;
            max.y_ = extentMiddle.y_;
        }
        else
        {
            min.y_ = extentMiddle.y_;
            max.y_ = extent.max_.y_;
        }

        if (z == 0)
        {
            min.z_ = extent.min_.z_;
            max.z_ = extentMiddle.z_;
        }
        else
        {
            min.z_ = extentMiddle.z_;
            max.z_ = extent.max_.z_;
        }
        Children[x][y][z] = new FOctree(this);
        Children[x][y][z]->SetExtent({min, max});
    }

    return Children[x][y][z];
}

FOctreeSceneManager::FOctreeSceneManager() : MaxDepth(8)
{
    Octree = new FOctree(nullptr);
    Octree->SetExtent(BoundingBox(-10000.0f, 10000.0f));
}

FOctreeSceneNode* FOctreeSceneManager::CreateSceneNode()
{
    return nullptr;
}

FOctreeSceneNode* FOctreeSceneManager::CreateSceneNode(const string& name)
{
    return nullptr;
}

void FOctreeSceneManager::UpdateNodeInOctree(FOctreeSceneNode* node)
{
    auto bbox = node->GetWorldAABBWithoutUpdate();
    if (!bbox.Defined())
        return;
    if (!Octree)
        return;

    //Node not in octree
    if (!node->GetOctant())
    {
        //Node outside of the extent of octree
        if (!bbox.IsInside(Octree->GetExtent()))
            Octree->AddNode(node);
        else
            InsertNodeIntoOctree(node, Octree);
    }
    else
    {
        //Node not in its octant anymore, need reinsert
        if (!bbox.IsInside(node->GetOctant()->GetExtent()))
        {
            //Node outside of the extent of octree
            if (!bbox.IsInside(Octree->GetExtent()))
                Octree->AddNode(node);
            else
                InsertNodeIntoOctree(node, Octree);
        }
    }
}

void FOctreeSceneManager::RemoveNodeFromOctree(FOctreeSceneNode* node)
{
    if (!Octree)
        return;

    auto* octant = node->GetOctant();
    if (octant)
        octant->RemoveNode(node);

    node->SetOctant(nullptr);
}

void FOctreeSceneManager::InsertNodeIntoOctree(FOctreeSceneNode* node, FOctree* treeNode, int currentDepth)
{
    if (!Octree)
        return;

    if (currentDepth < MaxDepth && treeNode->CanFitInChild(node->GetWorldAABBWithoutUpdate()))
    {
        auto childOctant = treeNode->WhichOctant(node->GetWorldAABBWithoutUpdate());
        InsertNodeIntoOctree(node, childOctant, currentDepth + 1);
    }
    else
    {
        //Insert at this depth
        treeNode->AddNode(node);
    }
}

} /* namespace tc */
