#pragma once
#include <QAbstractItemModel>
#include <vector>
#include <string>

namespace tc
{
class FNode;
}

class FSceneModel : public QAbstractItemModel
{
    Q_OBJECT

    tc::FNode* RootNode;

public:
    FSceneModel(QObject *parent, tc::FNode* rootNode);
    // QAbstractItemModel interface
    QModelIndex index(int row, int column, const QModelIndex &parent) const override;
    QModelIndex parent(const QModelIndex &child) const override;
    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
};
