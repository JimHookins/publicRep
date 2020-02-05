#include "model.h"
#include "itemmanager.h"

Model::Model(QObject *parent)
    : QAbstractListModel(parent)
    , pItemManager(nullptr)
{
}

int Model::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid() || !pItemManager)
        return 0;

    return pItemManager->getItems().count();
}

QVariant Model::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || !pItemManager)
        return QVariant();

    const MyItemData * item = pItemManager->getItems().at(index.row());
    switch (role)
    {
    case EdgesRole:
        return QVariant(item->edgeNum);
    case ColorRole:
        return QVariant(item->color);
    case CoordXRole:
        return QVariant(item->x);
    case CoordYRole:
        return QVariant(item->y);
    }

    return QVariant();
}

bool Model::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!pItemManager)
        return false;

    MyItemData * item = pItemManager->getItems().at(index.row());
    switch (role)
    {
    case EdgesRole:
        item->edgeNum = value.toInt();
        break;
    case ColorRole:
        item->color = value.value<QColor>();
        break;
    case CoordXRole:
        item->x = value.toReal();
        break;
    case CoordYRole:
        item->y = value.toReal();
        break;
    }

    if (pItemManager->setItemAt(index.row(), item))
    {
        emit dataChanged(index, index, QVector<int>() << role);
        return true;
    }

    return false;
}

Qt::ItemFlags Model::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEditable;
}

QHash<int, QByteArray> Model::roleNames() const
{
    QHash<int, QByteArray> names;
    names[EdgesRole] = "edges";
    names[ColorRole] = "color";
    names[CoordXRole] = "x";
    names[CoordYRole] = "y";
    return names;
}

ItemManager *Model::manager() const
{
    return pItemManager;
}

void Model::setManager(ItemManager *value)
{
    beginResetModel();

    if (pItemManager)
        pItemManager->disconnect(this);

    pItemManager = value;

    if (pItemManager)
    {
        connect(pItemManager, &ItemManager::preItemAppended, this, [=]() {
            const int index = pItemManager->getItems().size();
            beginInsertRows(QModelIndex(), index, index);
        });
        connect(pItemManager, &ItemManager::postItemAppended, this, [=]() {
            endInsertRows();
        });

        connect(pItemManager, &ItemManager::preItemRemoved, this, [=](int index, int index2) {
            beginRemoveRows(QModelIndex(), index, index2);
        });
        connect(pItemManager, &ItemManager::postItemRemoved, this, [=]() {
            endRemoveRows();
        });
    }

    endResetModel();
}
