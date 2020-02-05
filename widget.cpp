#include "widget.h"

Widget::Widget()
{

    QQuickWidget * qw = new QQuickWidget(QUrl("qrc:/main.qml"));
    QVBoxLayout * pvbx = new QVBoxLayout;
    pvbx->addWidget(qw);
    setLayout(pvbx);
}
