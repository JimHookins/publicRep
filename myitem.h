#pragma once
#include <QQuickPaintedItem>
class QPainter;

class MyItem : public QQuickPaintedItem
{
    Q_OBJECT

    Q_PROPERTY(QColor color READ colorValue WRITE setColor)
    Q_PROPERTY(int edgeNum READ edgeNumValue WRITE setEdgeNum)// NOTIFY edgeNumChanged)
    Q_PROPERTY(int radius READ radius WRITE setRadius)

public:
    MyItem();
    void paint(QPainter *painter);

    QColor colorValue() const;
    void setColor(const QColor&);

    int edgeNumValue() const;
    void setEdgeNum(int);

    int radius() const;
    void setRadius(int value);

private:
    QColor m_color;
    int m_edgeNum;
    int mRadius;

signals:

public slots:
};

