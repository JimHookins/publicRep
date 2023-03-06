#include "dbmanager.h"
#include <QDebug>

DBManager::DBManager(QObject *parent)
    : QObject{parent},
      p_model(nullptr)
{
    connect(&ds, &DataSource::resultReady, this, &DBManager::dataReadyToInsert);

    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName("/home/dmitry/devices.db");

    if (!m_db.open())
    {
        qDebug() << "Error: connection with database failed";
    }
    else
    {
        qDebug() << "Database: connection ok";

        p_model = new TableModel();
        p_model->setTable("devices");
        p_model->setEditStrategy(QSqlTableModel::OnFieldChange);
        qDebug() << "Select is " << p_model->select();
    }
}

QSqlTableModel * DBManager::getModel()
{
    return p_model;
}

void DBManager::updateModel()
{
    //request data from data source (free rest api service)
    ds.request();
}

void DBManager::cleanUpModel()
{
    QSqlQuery query;
    query.exec("DELETE FROM devices");
}

void DBManager::dataReadyToInsert(QMap<int,QString> values)
{
    cleanUpModel();

    QMapIterator<int, QString> it(values);
    while (it.hasNext())
    {
        it.next();
        p_model->addRow(it.key(), it.value());
    }

   qDebug() << "Select is " << p_model->select();
}
