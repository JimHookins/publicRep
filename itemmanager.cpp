#include "itemmanager.h"

ItemManager::ItemManager(QObject *parent) : QObject(parent)
{
    qRegisterMetaType<MyItemData>("MyItemData");
}

void ItemManager::create(int count)
{
    items.reserve(count);
    for (int i = 0; i < count; i++)
    {
        MyItemData * item = new MyItemData();
        connect(item, &MyItemData::timeIsGone, this, &ItemManager::deleteItemByTimeOut);
        item->color = QColor(generator.bounded(Qt::transparent, Qt::black));
        item->color2 = QColor(generator.bounded(Qt::transparent, Qt::black));
        item->edgeNum = generator.bounded(sidesNumFrom, sidesNumTo+1);
        item->x = generator.generateDouble();
        item->y = generator.generateDouble();
        item->setLifeSpan(lifespan);
        emit preItemAppended();
        items.append(item);
        emit postItemAppended();
    }
}

int ItemManager::getCount()
{
    if (items.isEmpty())
        return ITEMS_NUM;
    else
        return items.count();
}

int ItemManager::getEdgeCount(int index)
{
    if (index < items.count())
        return items.at(index)->edgeNum;
    else
        return 0;
}

QColor ItemManager::getColorValue(int index)
{
    if (index < items.count())
        return items.at(index)->color;
    else
        return QColor(Qt::green);
}

void ItemManager::itemClicked(int index)
{
    qDebug() << "ItemManager::itemClicked" << index << items.count();

    int n = items.at(index)->edgeNum;

    deleteItem(index);

    create(n);
}

void ItemManager::edgesChanged(int from, int to)
{
    qDebug() << "ItemManager::edgesChanged" << from << to;
    sidesNumFrom = from;
    sidesNumTo = to;
}

void ItemManager::lifeSpanChanged(int value)
{
    qDebug() << "ItemManager::lifeSpanChanged" << value;
    lifespan = value;

    for(MyItemData * item : items)
        item->setLifeSpan(lifespan);
}

void ItemManager::deleteItem(int index)
{
    emit preItemRemoved(index, index);
    items.removeAt(index);
    emit postItemRemoved();
    qDebug() << "Item" << index << "deleted";
}

void ItemManager::deleteItemByTimeOut(MyItemData * item)
{
    int index = items.indexOf(item);
    if (index >= 0)
        deleteItem(index);
}


QVector<MyItemData*> ItemManager::getItems() const
{
    return items;
}

bool ItemManager::setItemAt(int index, MyItemData *item)
{
    if (index < 0 || index >= items.count())
        return false;

    MyItemData * oldItem = items.at(index);
    if (oldItem->color == item->color
            && oldItem->edgeNum == item->edgeNum
            && oldItem->color2 == item->color2
            && oldItem->x == item->x
            && oldItem->y == item->y)
        return false;

    items[index] = item;
    oldItem->deleteLater();
    return true;
}

quint8 ItemManager::getSidesNumTo() const
{
    return sidesNumTo;
}

quint8 ItemManager::getSidesNumFrom() const
{
    return sidesNumFrom;
}
