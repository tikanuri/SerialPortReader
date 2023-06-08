#include "widget.h"
#include "ui_widget.h"


Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
    , baseSerialWidget(new SerialWidget(this))
{
    ui->setupUi(this);
    this->setWindowTitle("SerialPort");
    baseSerialWidget->setObjectName("baseSerialWidget");
    ui->horizontalLayout->addWidget(baseSerialWidget);
}

Widget::~Widget()
{
    delete ui;
}

