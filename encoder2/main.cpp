#include <myencoder.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QString usageInfo;
    QFile file("/text/usage");
    if (file.open(QIODevice::ReadOnly))
    {
        usageInfo = QString(file.readAll());
        file.close();
    }

    if (argc > 1)
    {
        MyEncoder * enc = new MyEncoder(QString(argv[1]));//!
        if (enc->start())
            qDebug() << "Convertation completed";
        else
            qDebug() << "Error occured";

    }
    else
        qDebug() << "No arguments\n" << usageInfo;

    return a.exec();
}
