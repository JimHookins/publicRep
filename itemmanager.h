#ifndef ITEMMANAGER_H
#define ITEMMANAGER_H

#include <QObject>
#include <QColor>
#include <QDebug>
#include <QTime>
#include <QTimerEvent>
#include <QRandomGenerator>

#define ITEMS_NUM   3
#define EDGES       3
#define SIDES_FROM  3
#define SIDES_TO    5
#define LIVE_TIME   1000

class MyItemData : public QObject
{
    Q_OBJECT
public:
    MyItemData(QObject *parent = Q_NULLPTR)
        : QObject(parent)
    {}
    MyItemData(const MyItemData &item)
        : QObject(item.parent())
        , x(item.x)
        , y(item.y)
        , edgeNum(item.edgeNum)
        , color(item.color)
        , color2(item.color2)
        , lifespan(item.lifespan)
    {
        timerId = startTimer(lifespan);
    }

    qreal x = 0;
    qreal y = 0;
    int edgeNum = EDGES;
    QColor color = QColor(Qt::green);
    QColor color2 = QColor(Qt::red);
    int lifespan = LIVE_TIME;

public slots:
    void setLifeSpan(int value)
    {
        lifespan = value;
        if (timerId != 0)
            killTimer(timerId);
        timerId = startTimer(lifespan);
    }

signals:
    void timeIsGone(MyItemData * pItem);

private:
    int timerId = 0;

    void timerEvent(QTimerEvent *event)
    {
        if (event->timerId() == timerId)
        {
            emit timeIsGone(this);
        }
    }
};

class ItemManager : public QObject
{
    Q_OBJECT
public:
    explicit ItemManager(QObject *parent = nullptr);

    QVector<MyItemData *> getItems() const;

    bool setItemAt(int index, MyItemData *item);

signals:
    void preItemAppended();
    void postItemAppended();

    void preItemRemoved(int index, int index2);
    void postItemRemoved();

public slots:
    void create(int count);

    quint8 getSidesNumFrom() const;
    quint8 getSidesNumTo() const;

    int getCount();

    int getEdgeCount(int index);
    QColor getColorValue(int index);

    void itemClicked(int index);
    void edgesChanged(int from, int to);
    void lifeSpanChanged(int value);

    void deleteItem(int index);
    void deleteItemByTimeOut(MyItemData *item);

private:
    QVector<MyItemData*> items;
    quint8 sidesNumFrom = 3;
    quint8 sidesNumTo = 5;
    QRandomGenerator generator;
    int lifespan = LIVE_TIME;
};

#endif // ITEMMANAGER_H
