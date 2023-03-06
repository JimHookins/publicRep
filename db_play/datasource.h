#ifndef DATASOURCE_H
#define DATASOURCE_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

struct SomeData {
    int id = 0;
    QString text = "";
};

class DataSource : public QObject
{
    Q_OBJECT
public:
    explicit DataSource(QObject *parent = nullptr);

    bool request();

signals:
    void resultReady(QMap<int, QString> values);

private slots:
    void readReply();
    void finishReplyReading();

private:
    QNetworkAccessManager * networkManager;
    QNetworkReply * networkReply;
    QByteArray buffer;
};

#endif // DATASOURCE_H
