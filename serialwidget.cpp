#include "serialwidget.h"
#include "ui_serialwidget.h"

SerialWidget::SerialWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SerialWidget),
    serialPort(this),
    comboBoxUpdateEventFilter(this)
{
    ui->setupUi(this);
    ui->comboBoxPort->installEventFilter(comboBoxUpdateEventFilter);
    connect(ui->pushButtonStart,&QPushButton::clicked,this,&SerialWidget::changeState);
    connect(&comboBoxUpdateEventFilter,&ComboBoxUpdateEventFilter::clicked,this,&SerialWidget::updatePortInfo);
}

SerialWidget::~SerialWidget()
{
    delete ui;
}

void SerialWidget::changeState()
{
    if(serialPort.isOpen())
    {
        serialPort.close();
        ui->pushButtonStart->setText("Start");
    }
    else if(serialPort.open(QIODeviceBase::ReadWrite))
    {
        ui->pushButtonStart->setText("Stop");
    }
    else
    {
        ui->debugInfo->setText(QString("Can`t be opened"));
    }
}

void SerialWidget::updatePortInfo()
{

}
