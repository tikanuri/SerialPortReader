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
    return COLUMN_SIZE;
}

int SerialModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return listData.length();
}

QVariant SerialModel::data(const QModelIndex &index, int role) const
{
    if(index.row() >= listData.length())
    {
        qDebug() << "Row: " << index.row();
    }

    if( !index.isValid() || index.row()>=listData.length() )
    {
        return QVariant();
    }

    SerialData &d = listData.at(index.row());
    QString str(d.byteArray);

    switch (role) {
    case Qt::DisplayRole:
        switch (index.column()) {
        case 0:
            return d.dateTime.toString("dd.MM hh:mm:ss:zzz");

        case 1:
            return QString(d.byteArray);

        case 2:
            return QString(d.byteArray.toHex(' '));

        default:
            break;
        }
    case Qt::SizeHintRole:
        case 1:
            return QSize(0,30);
        default:
            return QSize();
        }
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
    if( !hasIndex(row,column) )
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

bool SerialModel::hasChildren(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return false;
}

void SerialModel::addSerialData(const QDateTime &dt, const QByteArray &ba)
{
    int l = listData.length();
    this->beginInsertRows(QModelIndex(),l,l);
    listData.append({dt,ba});
    this->endInsertRows();
}

void SerialModel::clearSerialData()
{
    int l = listData.length()-1;
    this->beginRemoveRows(QModelIndex(),0,l);
    listData.clear();
    this->endRemoveRows();
}


