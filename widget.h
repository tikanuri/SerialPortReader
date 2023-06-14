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

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void newSerialWidget();
    void deleteSerialWidget();

private:
    Ui::Widget *ui;
    QSplitter *splitter;
    SerialWidget *baseSerialWidget;
    QList<SerialWidget*> listSerialWidget;
};
#endif // WIDGET_H
