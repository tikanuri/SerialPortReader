#include "serialmodel.h"
#include <QDateTime>

SerialModel::SerialModel(QObject *parent)
    : QAbstractItemModel{parent}
{
    SerialData a{
        .dateTime = QDateTime::currentDateTime(),
        .byteArray = QByteArray("Test constructor model\r\n 2 string")
    };
    listData.append(a);
}

int SerialModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return COLUMN_SIZE_MAX;
}

int SerialModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return listData.length();
}

QVariant SerialModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid() || index.row()>=listData.length()
            || role!=Qt::DisplayRole)
    {
        return QVariant();
    }

    switch (index.column()) {
    case 0:
        return listData.at(index.row()).dateTime.toString("dd.MM hh:mm:ss:zzz");

    case 1:
        return QString(listData.at(index.row()).byteArray);

    case 2:
        return QString(listData.at(index.row()).byteArray.toHex(' '));

    default:
        break;
    }
    return QVariant();
}

bool SerialModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    Q_UNUSED(index);
    Q_UNUSED(value);
    Q_UNUSED(role);
    return false;
}

QFlags<Qt::ItemFlag> SerialModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;
    return QAbstractItemModel::flags(index);
}

QVariant SerialModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role == Qt::DisplayRole)
    {
        switch (orientation) {
        case Qt::Horizontal:
            switch(section) {
            case 0:
                return QString("Time");
            case 1:
                return QString("Text");
            case 2:
                return QString("Hex");
            }
            break;
        case Qt::Vertical:
            return QString("Row %0").arg(section);
        }
    }
    return QVariant();
}

QModelIndex SerialModel::index(int row, int column, const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    if( !hasIndex(row,column,parent) )
    {
        return QModelIndex();
    }
    return createIndex(row,column);
}

QModelIndex SerialModel::parent(const QModelIndex &child) const
{
    Q_UNUSED(child);
    return QModelIndex();
}

void SerialModel::add(const QDateTime &dt, const QByteArray &ba)
{
    SerialData sd{dt, ba};
    listData.append(sd);

}

void SerialModel::setColumnFlags(const ColumnFlags &cf, const bool b)
{
    if(b)
    {
        columnFlags = static_cast<ColumnFlags>(columnFlags | cf);
    }
    else
    {
        columnFlags = static_cast<ColumnFlags>(columnFlags & ~cf);
    }

}
