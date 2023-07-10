#ifndef SERIALWIDGET_H
#define SERIALWIDGET_H

#include <QWidget>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QTcpSocket>
#include <QMap>
#include <QMetaEnum>
#include <QPushButton>
#include "comboboxupdateeventfilter.h"
#include "serialmodel.h"
#include "serialview.h"

namespace Ui {
class SerialWidget;
}

class SerialWidget : public QWidget
{
    Q_OBJECT
private:
    enum SendType
    {
        Text,
        Hex
    };

public:
    explicit SerialWidget(bool visibleMinusButton, QWidget *parent = nullptr);
    ~SerialWidget();
    QPushButton *getPlusButton();
    QPushButton *getMinusButton();

public slots:
    void changeState();
    void updatePortInfo();
    void read();
    void write();
    void connectedTcpSocket();
    void tcpSocketError(QAbstractSocket::SocketError socketError);

signals:
    void clickedPlus();
    void clickedMinus();

private:
    void initMetaEnum();

private:
    Ui::SerialWidget *ui;
    SerialModel *model;
    QSerialPort serialPort;
    QTcpSocket tcpSocket;
    ComboBoxUpdateEventFilter comboBoxUpdateEventFilter;
    QMap<QString,QSerialPortInfo> portInfoMap;
    QMetaEnum enumBaudrate;
    QMetaEnum enumParity;
    QMetaEnum enumStopBits;
    QMetaEnum enumDataBits;
};

#endif // SERIALWIDGET_H
