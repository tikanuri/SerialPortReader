#ifndef SERIALWIDGET_H
#define SERIALWIDGET_H

#include <QWidget>
#include <QSerialPort>
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
    Ui::SerialWidget *ui;
    QSerialPort serialPort;
    ComboBoxUpdateEventFilter comboBoxUpdateEventFilter;
};

#endif // SERIALWIDGET_H
