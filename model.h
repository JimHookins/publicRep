#ifndef MODEL_H
#define MODEL_H

#include <QAbstractListModel>

class ItemManager;

class Model : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY(ItemManager * manager READ manager WRITE setManager)

public:
    explicit Model(QObject *parent = nullptr);


    enum {
        EdgesRole = Qt::UserRole,
        ColorRole,
        CoordXRole,
        CoordYRole
    };

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // Editable:
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;

    Qt::ItemFlags flags(const QModelIndex& index) const override;


    QHash<int, QByteArray> roleNames() const override;

    ItemManager * manager() const;
    void setManager(ItemManager *value);

private:
    ItemManager * pItemManager;
};

#endif // MODEL_H
