#include "myitem.h"
#include <QPainter>
//#include <QPolygon>
#include <qmath.h>
#define DEAFAULT_R  (50)
#define EDGES       (3)

MyItem::MyItem(QQuickItem *pqi)
    : QQuickPaintedItem(pqi)
    , m_color(Qt::green)
    , m_edgeNum(EDGES)
{
}

void MyItem::paint(QPainter *painter)
{
    int n = m_edgeNum;
    QVector<QPoint> points;
    points.resize(n);
    qreal angle = 360/n;
    int R = mRadius;
    qreal z = 0;
    for(quint8 i = 0; i < n; i++)
    {
        points[i].setX(R + (qRound(qCos(z/180*M_PI)*R)));
        points[i].setY(R - (qRound(qSin(z/180*M_PI)*R)));
        z = z + angle;
    }
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setPen(Qt::NoPen);
    painter->setBrush(m_color);
    self = QPolygon(points);
    painter->drawPolygon(self);
}

QColor MyItem::colorValue() const
{
    return m_color;
}

void MyItem::setColor(const QColor & value)
{
    m_color = value;
}

int MyItem::edgeNumValue() const
{
    return m_edgeNum;
}

void MyItem::setEdgeNum(int value)
{
    if (value >= EDGES)
    {
        m_edgeNum = value;
    }
}

int MyItem::radius() const
{
    return mRadius;
}

void MyItem::setRadius(int value)
{
    mRadius = value;
}

bool MyItem::insideItem(int x, int y)
{
    return self.containsPoint(QPoint(x,y), Qt::OddEvenFill);
}
