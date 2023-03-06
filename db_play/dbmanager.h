#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QSqlQueryModel>

#include "datasource.h"
#include "tablemodel.h"

class DBManager : public QObject
{
    Q_OBJECT
public:
    explicit DBManager(QObject *parent = nullptr);
    QSqlTableModel *getModel();
    void updateModel();

signals:

private:
    QSqlDatabase m_db;
    TableModel * p_model;
    DataSource ds;

    inline void cleanUpModel();

private slots:
    void dataReadyToInsert(QMap<int, QString> values);

};

#endif // DBMANAGER_H
