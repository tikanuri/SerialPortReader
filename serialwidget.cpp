#include "serialwidget.h"
#include "ui_serialwidget.h"

SerialWidget::SerialWidget(bool visibleMinusButton,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SerialWidget),
    serialPort(this),
    comboBoxUpdateEventFilter(this),
    portInfoMap()
{
    ui->setupUi(this);
    ui->pushButtonMinus->setVisible(visibleMinusButton);
    qDebug() << "Constructor:  " << (uint64_t)this;
    connect(ui->pushButtonPlus, &QPushButton::clicked, this, [=](){emit clickedPlus();});
    connect(ui->pushButtonMinus, &QPushButton::clicked, this, [=](){emit clickedMinus();});

    ui->comboBoxPort->installEventFilter(&comboBoxUpdateEventFilter);
    connect(&comboBoxUpdateEventFilter,&ComboBoxUpdateEventFilter::clicked,this,&SerialWidget::updatePortInfo);
    //connect(ui->comboBoxPort,&QComboBox::activated,this,&SerialWidget::choosesPort);
    connect(ui->pushButtonStart,&QPushButton::clicked,this,&SerialWidget::changeState);
    ui->comboBoxBaudrate->setEditable(true);
    updatePortInfo();
    initMetaEnum();


}

SerialWidget::~SerialWidget()
{
    qDebug() << "Destructor:  " << (uint64_t)this;
    delete ui;
}

void SerialWidget::changeState()
{
    if(serialPort.isOpen())
    {
        serialPort.close();
        ui->pushButtonStart->setText("Start");
    }
    else
    {
        serialPort.setPort(portInfoMap[ui->comboBoxPort->currentText()]);
        serialPort.setBaudRate(9600);
        serialPort.setDataBits(QSerialPort::Data8);
        serialPort.setStopBits(QSerialPort::OneStop);
        serialPort.setParity(QSerialPort::NoParity);
        if(serialPort.open(QIODeviceBase::ReadWrite))
        {
            ui->pushButtonStart->setText("Stop");
        }
        else
        {
            ui->debugInfo->setText(QString("Can`t be opened"));
        }
    }
}

void SerialWidget::updatePortInfo()
{
    ui->comboBoxPort->clear();
    QList<QString> keys = portInfoMap.keys();
    //portInfoMap.clear();
    QList<QSerialPortInfo> list = QSerialPortInfo::availablePorts();
    for(QSerialPortInfo info : list)
    {
        qDebug() << info.description() << info.portName() << info.systemLocation();
        keys.removeAll(info.portName());
        portInfoMap[info.portName()] = info;
    }
    portInfoMap.removeIf([&keys](QMap<QString, QSerialPortInfo>::iterator i){return keys.contains(i.key());});
    ui->comboBoxPort->addItems(portInfoMap.keys());
}

void SerialWidget::initMetaEnum()
{
    enumBaudrate = QMetaEnum::fromType<QSerialPort::BaudRate>();
    for(int i = 0; i < enumBaudrate.keyCount(); i++)
    {
        ui->comboBoxBaudrate->addItem(QString::number(enumBaudrate.value(i)));
    }

    enumParity = QMetaEnum::fromType<QSerialPort::Parity>();
    for(int i = 0; i < enumParity.keyCount(); i++)
    {
        ui->comboBoxParity->addItem(QString(enumParity.key(i)));
    }
}

QPushButton *SerialWidget::getPlusButton()
{
    return ui->pushButtonPlus;
}

QPushButton *SerialWidget::getMinusButton()
{
    return ui->pushButtonMinus;
}
