#include "datasource.h"
#include <QJsonObject>
#include <QJsonParseError>
#include <QJsonArray>

DataSource::DataSource(QObject *parent)
    : QObject{parent}
{
    networkManager = new QNetworkAccessManager(this);
}

bool DataSource::request()
{
    //create request
    QNetworkRequest request = QNetworkRequest(QUrl("https://api.restful-api.dev/objects"));//list of devices, free

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    networkReply = networkManager->get(request);

    connect(networkReply, &QNetworkReply::readyRead, this, &DataSource::readReply);
    connect(networkReply, &QNetworkReply::finished, this, &DataSource::finishReplyReading);

    return true;
}

void DataSource::readReply()
{
    qDebug() << "MLData came to slot";
    buffer.append(networkReply->readAll());
}

void DataSource::finishReplyReading()
{
    if(networkReply->error() != QNetworkReply::NoError)
    {
        qDebug() << "Error : " << networkReply->errorString();
    }
    else
    {
        //retrieve data from buffer and parse it
        QMap<int,QString> result;
        QJsonParseError error;
        QJsonDocument doc = QJsonDocument::fromJson(buffer, &error);
        if (error.error != QJsonParseError::NoError)
        {
            qDebug() << error.errorString();
        }
        if (doc.isArray())
        {

            QJsonArray array = doc.array();

            foreach (const QJsonValue & v, array)
            {
                QJsonObject obj = v.toObject();
                int id = obj.value("id").toString().toInt();
                QString name = obj.value("name").toString();
                result.insert(id, name);

                qDebug() << id << name;
            }
        }

        //then empty the buffer
//        qDebug() << "MLData buffer \n" << buffer;
        buffer.clear();

        emit resultReady(result);
    }

    networkReply->deleteLater();
}
