#ifndef QUICKWIDGET_H
#define QUICKWIDGET_H
#include <QQuickWidget>
#include <QGridLayout>
#include <QVBoxLayout>
#include "myitem.h"


class Widget : public QWidget
{
public:
    Widget();

private:
    QList<MyItem> items;
};

#endif // QUICKWIDGET_H
