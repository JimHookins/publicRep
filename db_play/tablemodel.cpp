#include "tablemodel.h"
#include <QSqlRecord>
#include <QSqlQuery>
#include <QDebug>

TableModel::TableModel(QObject *parent)
    : QSqlTableModel{parent}
{

}
QHash<int, QByteArray> TableModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    for (int i = 0; i < record().count(); i ++) {
        roles.insert(Qt::UserRole + i + 1, record().fieldName(i).toUtf8());
    }
    return roles;
}

bool TableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    QModelIndex modelIndex = this->index(index.row(), role - Qt::UserRole - 1);
    qDebug() << "Editing result is " << QSqlTableModel::setData(modelIndex,value, Qt::EditRole);
    emit dataChanged(modelIndex, modelIndex);
    return true;
}

Qt::ItemFlags TableModel::flags(const QModelIndex &index) const
{
    Q_UNUSED(index)
    return Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

void TableModel::addRow(int id, const QString& name, const QString& comment)
{
    Q_UNUSED(comment)
    QSqlQuery query;
    query.exec(QString("INSERT INTO devices(id, name) VALUES(") + QString::number(id) + QString(", \"") + name + QString("\");"));
}

QVariant TableModel::data(const QModelIndex &index, int role) const
{
    QVariant value;
    if (index.isValid()) {
        if (role < Qt::UserRole) {
            value = QSqlTableModel::data(index, role);
        } else {
            int columnIdx = role - Qt::UserRole - 1;
            QModelIndex modelIndex = this->index(index.row(), columnIdx);
            value = QSqlTableModel::data(modelIndex, Qt::DisplayRole);
        }
    }
    return value;
}
