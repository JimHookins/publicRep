#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "itemmanager.h"
#include "model.h"
#include "myitem.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    //register my graphic item - polygon
    qmlRegisterType<MyItem>("com.myinc.MyItem", 1, 0, "MyItem");
    //register model
    qmlRegisterType<Model>("Model", 1, 0, "Model");
    qmlRegisterUncreatableType<ItemManager>("ItemManager", 1, 0, "ItemManager",
        QStringLiteral("ItemManager must be created in C++ part"));

    ItemManager manager;

    QQmlApplicationEngine engine;
    QQmlComponent component(&engine, QUrl(QStringLiteral("qrc:/main.qml")));
    QQmlContext * context = engine.rootContext();

    context->setContextProperty("ItemManager", &manager);

    QObject * rootObj = component.create();

    //connect qml controls to C++ slots
    QObject * rangeSlider = rootObj->findChild<QObject*>("rangeSlider");
    if (rangeSlider)
    {
        QObject::connect(rangeSlider, SIGNAL(edgesChanged(int,int)), &manager, SLOT(edgesChanged(int,int)));
    }

    QObject * lifeSpanSlider = rootObj->findChild<QObject*>("lifeSpanSlider");
    if (lifeSpanSlider)
    {
        QObject::connect(lifeSpanSlider, SIGNAL(lifeSpanChanged(int)), &manager, SLOT(lifeSpanChanged(int)));
    }

    return app.exec();
}
