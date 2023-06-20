#ifndef SERIALMODEL_H
#define SERIALMODEL_H

#include <QAbstractItemModel>
#include <QObject>
#include <QList>
#include <QByteArray>
#include <QDateTime>
#include <QVariant>
#include <QModelIndex>

class SerialModel : public QAbstractItemModel
{

private:
    static constexpr int COLUMN_SIZE_MAX = 3;

    struct SerialData
    {
        QDateTime dateTime;
        QByteArray byteArray;
    };
    QList<SerialData> listData;

public:
    explicit SerialModel(QObject *parent = nullptr);

    //Inherits QAbstractItemModel
    int columnCount(const QModelIndex &parent) const override;
    int rowCount(const QModelIndex &parent) const override;

    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    bool setData(const QModelIndex &index, const QVariant &value, int role) override;

    QFlags<Qt::ItemFlag> flags(const QModelIndex &index) const override;
    QModelIndex index(int row, int column, const QModelIndex &parent) const override;
    QModelIndex parent(const QModelIndex &child) const override;
    bool hasChildren(const QModelIndex &parent) const override;

    //Custom
    void add(const QDateTime &dt, const QByteArray & ba);
};

#endif // SERIALMODEL_H
