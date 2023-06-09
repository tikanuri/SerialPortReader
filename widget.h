#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QList>
#include <QSplitter>
#include "serialwidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT
    static constexpr int COUNT_OF_SERIAL_WIDGET = 4;
public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void newSerialWidget();
    void deleteSerialWidget();

private:
    Ui::Widget *ui;
    QSplitter *splitter;
};
#endif // WIDGET_H
