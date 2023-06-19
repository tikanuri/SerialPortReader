#ifndef SERIALMODEL_H
#define SERIALMODEL_H

#include <QAbstractItemModel>
#include <QObject>
#include <QList>
#include <QByteArray>
#include <QDateTime>
#include <QVariant>
class SerialModel : public QAbstractItemModel
{

private:
    constexpr int COLUMN_SIZE = 3;
    struct SerialData
    {
        QDateTime dateTime;
        QByteArray byteArray;
    };
    QList<SerialData> listData;

public:
    explicit SerialModel(QObject *parent = nullptr);

    int columnCount(const QModelIndex &parent) const override
    {
        Q_UNUSED(parent);
        return COLUMN_SIZE;
    }

    int rowCount(const QModelIndex &parent) const override
    {
        Q_UNUSED(parent);
        return listData.length();
    }

    QVariant data(const QModelIndex &index, int role) const override
    {
        if(!index.isValid() || index.row()>=listData.length()
                || role!=Qt::DisplayRole)
        {
            return QVariant();
        }
        switch (index.column()) {
        case 0:
            return listData.at(index.row()).dateTime.toString("dd.MM hh:mm:ss:zzz");
            break;
        case 1:
            return QString(listData.at(index.row()).byteArray);
            break;
        case 2:
            return QString(listData.at(index.row()).byteArray.toHex(' '));
            break;
        default:
            return QVariant();
            break;
        }
    }

    bool setData(const QModelIndex &index, const QVariant &value, int role) override
    {
        if(index.isValid() && role==Qt::EditRole && index.row()<listData.length())
        {
            switch (index.column()) {
            case 0:
                listData[index.row()].dateTime = value.toDateTime();
                emit dataChanged(index,index);
                break;
            case 1:
                listData[index.row()].byteArray = value.toByteArray();
                QModelIndex indexRigth = createIndex(index.row(),2);
                emit dataChanged(index,indexRigth);
                break;
            case 2:
                listData[index.row()].byteArray = QByteArray::fromHex(value.toByteArray());
                QModelIndex indexLeft = createIndex(index.row(),1);
                emit dataChanged(indexLeft,index);
                break;
            default:
                return false;
                break;
            }

            return true;
        }
        else
        {
            return false;
        }
    }

    Qt::ItemFlag flags(const QModelIndex &index) const override
    {
        return QAbstractItemModel::flags(index);
    }
};

#endif // SERIALMODEL_H
