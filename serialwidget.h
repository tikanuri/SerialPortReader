#ifndef SERIALWIDGET_H
#define SERIALWIDGET_H

#include <QWidget>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QMap>
#include <QMetaEnum>
#include "comboboxupdateeventfilter.h"

namespace Ui {
class SerialWidget;
}

class SerialWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SerialWidget(QWidget *parent = nullptr);
    ~SerialWidget();

public slots:
    void changeState();
    void updatePortInfo();

private:
    void initMetaEnum();


private:
    Ui::SerialWidget *ui;
    QSerialPort serialPort;
    ComboBoxUpdateEventFilter comboBoxUpdateEventFilter;
    QMap<QString,QSerialPortInfo> portInfoMap;
    QMetaEnum enumBaudrate;
    QMetaEnum enumParity;
    QMetaEnum enumStopBits;
    QMetaEnum enumDataBits;
};

#endif // SERIALWIDGET_H
