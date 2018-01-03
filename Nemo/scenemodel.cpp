#include "scenemodel.h"
#include <SceneNode.h>

using namespace tc;

FSceneModel::FSceneModel(QObject *parent, FSceneNode* rootNode) : QAbstractItemModel(parent), RootNode(rootNode)
{
}

QModelIndex FSceneModel::index(int row, int column, const QModelIndex &parent) const
{
    if (parent.isValid())
    {
        auto* grandParent = static_cast<FSceneNode*>(parent.internalPointer());
        FSceneNode* parentNode;
        if (!grandParent)
            parentNode = RootNode;
        else
            parentNode = grandParent->ChildAt(parent.row());
        return createIndex(row, column, parentNode);
    }
    return createIndex(row, column, nullptr);
}

QModelIndex FSceneModel::parent(const QModelIndex &child) const
{
    auto* parentNode = static_cast<FSceneNode*>(child.internalPointer());
    if (!parentNode)
        return QModelIndex();
    auto* grandParent = parentNode->GetParent();
    if (!grandParent)
        return createIndex(0, 0, nullptr);
    return createIndex(grandParent->ChildToIndex(parentNode), 0, grandParent);
}

int FSceneModel::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
        return 1;
    auto* grandParent = static_cast<FSceneNode*>(parent.internalPointer());
    if (!grandParent)
        return RootNode->CountChildren();
    auto* parentNode = grandParent->ChildAt(parent.row());
    return parentNode->CountChildren();
}

int FSceneModel::columnCount(const QModelIndex &parent) const
{
    return 1;
}

QVariant FSceneModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role == Qt::DisplayRole)
    {
        auto* parentNode = static_cast<FSceneNode*>(index.internalPointer());
        if (!parentNode)
        {
            return QVariant(RootNode->GetName().c_str());
        }
        auto* thisNode = parentNode->ChildAt(index.row());
        auto string = thisNode->GetName();
        return QVariant(string.c_str());
    }

    return QVariant();
}
