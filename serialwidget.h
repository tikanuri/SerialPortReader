#ifndef SERIALWIDGET_H
#define SERIALWIDGET_H

#include <QWidget>
#include <QSerialPort>

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

private:
    Ui::SerialWidget *ui;
    QSerialPort serialPort;
};

#endif // SERIALWIDGET_H
