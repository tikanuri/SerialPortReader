#include <QPushButton>
#include "widget.h"
#include "ui_widget.h"


Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
    , splitter(new QSplitter(Qt::Orientation::Horizontal,this))
{
    ui->setupUi(this);
    this->setWindowTitle("SerialPort");

    SerialWidget *baseSerialWidget = new SerialWidget(false,this);
    ui->horizontalLayout->addWidget(splitter);
    splitter->addWidget(baseSerialWidget);
    connect(baseSerialWidget, &SerialWidget::clickedPlus, this, &Widget::newSerialWidget);
    connect(baseSerialWidget, &SerialWidget::clickedMinus, this, &Widget::deleteSerialWidget);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::newSerialWidget()
{
    if(splitter->count() == 1)
    {
        qobject_cast<SerialWidget*>(sender())->getMinusButton()->show();
    }
    if(splitter->count() < 5)
    {
        SerialWidget *sw = new SerialWidget(true,this);
        splitter->addWidget(sw);
        connect(sw, &SerialWidget::clickedPlus, this, &Widget::newSerialWidget);
        connect(sw, &SerialWidget::clickedMinus, this, &Widget::deleteSerialWidget);
    }
}

void Widget::deleteSerialWidget()
{
    if(splitter->count() > 1)
    {
        SerialWidget *sw = qobject_cast<SerialWidget*>(sender());
        sw->setParent(nullptr);
        sw->deleteLater();
    }
    if(splitter->count() == 1)
    {
        SerialWidget *sw = qobject_cast<SerialWidget*>(splitter->widget(0));
        sw->getMinusButton()->hide();
    }
}
