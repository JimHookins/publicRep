#ifndef MYENCODER_H
#define MYENCODER_H

#include <QCoreApplication>
#include <QObject>
#include <QString>
#include <QDir>
#include <QFile>
#include <QThread>
#include "worker.h"

class MyEncoder : public QObject
{
    Q_OBJECT
public:
    explicit MyEncoder(QString _path = QString(), QObject *parent = nullptr);
    ~MyEncoder();
    bool start();

signals:

public slots:

private:
    QString path;
    QList<Worker*> workers;
    inline Worker *findAvailableWorker();
    void releaseWorkers();
};

#endif // MYENCODER_H
