#ifndef ROOTCONTEXT_H
#define ROOTCONTEXT_H
#include <QObject>

#include "dbmanager.h"

class RootContext : public QObject
{
    Q_OBJECT

public:
    RootContext();

    QSqlTableModel * getModel();
    Q_INVOKABLE void updateModel();

private:
    DBManager db;
};

#endif // ROOTCONTEXT_H
