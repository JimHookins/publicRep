#include "itemmanager.h"

static int getRandomFromRange(int max, int min)
{
    return (((qreal)qrand())/RAND_MAX) * (max - min) + min;
}

static qreal getRandomFromRange(qreal max, qreal min)
{
    return (((qreal)qrand())/RAND_MAX) * (max - min) + min;
}

ItemManager::ItemManager(QObject *parent) : QObject(parent)
{
    qRegisterMetaType<MyItemData>("MyItemData");
    colorGenerator.bounded(Qt::darkYellow, Qt::black);
}

void ItemManager::create(int count)
{
    items.reserve(count);
    for (int i = 0; i < count; i++)
    {
        MyItemData * item = new MyItemData();
        connect(item, &MyItemData::timeIsGone, this, &ItemManager::deleteItemByTimeOut);
        item->color = QColor(colorGenerator.generate());
        item->edgeNum = getRandomFromRange(sidesNumTo, sidesNumFrom);
        item->x = getRandomFromRange(0.0, 1.0);
        item->y = getRandomFromRange(0.0, 1.0);
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
}

QColor ItemManager::getColorValue(int index)
{
    if (index < items.count())
        return items.at(index)->color;
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
    if (oldItem->color == item->color && oldItem->edgeNum == item->edgeNum)
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
