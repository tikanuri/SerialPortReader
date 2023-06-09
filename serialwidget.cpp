#include "serialwidget.h"
#include "ui_serialwidget.h"
#include "utils.h"

SerialWidget::SerialWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SerialWidget),
    serialPort(this),
    comboBoxUpdateEventFilter(this),
    portInfoMap()
{
    ui->setupUi(this);

    ui->comboBoxPort->installEventFilter(&comboBoxUpdateEventFilter);
    connect(&comboBoxUpdateEventFilter,&ComboBoxUpdateEventFilter::clicked,this,&SerialWidget::updatePortInfo);

    connect(ui->pushButtonStart,&QPushButton::clicked,this,&SerialWidget::changeState);
    ui->comboBoxBaudrate->setEditable(true);


    updatePortInfo();
    initMetaEnum();

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
        ui->debugInfo->setText(QString("Close"));
        ui->comboBoxPort->setEnabled(true);
    }
    else
    {
        serialPort.setPort(portInfoMap[ui->comboBoxPort->currentText()]);
        serialPort.setBaudRate(ui->comboBoxBaudrate->currentText().toInt());
        QSerialPort::DataBits dbits = comboBoxToEnum<QSerialPort::DataBits>(ui->comboBoxBits,enumDataBits);
        QSerialPort::StopBits sbits = comboBoxToEnum<QSerialPort::StopBits>(ui->comboBoxStop,enumStopBits);
        QSerialPort::Parity parity = comboBoxToEnum<QSerialPort::Parity>(ui->comboBoxParity,enumParity);
        serialPort.setDataBits(dbits);
        serialPort.setStopBits(sbits);
        serialPort.setParity(parity);
        if(serialPort.open(QIODeviceBase::ReadWrite))
        {
            ui->pushButtonStart->setText("Stop");
            ui->debugInfo->setText(QString("Open: %0 %1 %2 %3")
                                   .arg(ui->comboBoxPort->currentText())
                                   .arg(dbits)
                                   .arg(enumParity.valueToKey(parity))
                                   .arg(sbits)
                                    );
            ui->comboBoxPort->setEnabled(false);
        }
        else
        {
            ui->debugInfo->setText(QString("Can`t be opened"));
        }
    }
}

void SerialWidget::updatePortInfo()
{
    if(ui->comboBoxPort->isEnabled())
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
}



void SerialWidget::initMetaEnum()
{
    enumBaudrate = QMetaEnum::fromType<QSerialPort::BaudRate>();
    for(int i = 0; i < enumBaudrate.keyCount(); i++)
    {
        ui->comboBoxBaudrate->addItem(QString::number(enumBaudrate.value(i)));
    }
    ui->comboBoxBaudrate->setCurrentText(QString::number(QSerialPort::Baud115200));

    enumToQCombobox<QSerialPort::Parity>(ui->comboBoxParity, enumParity);
    enumToQCombobox<QSerialPort::StopBits>(ui->comboBoxStop, enumStopBits);
    enumToQCombobox<QSerialPort::DataBits>(ui->comboBoxBits, enumDataBits);
    ui->comboBoxBits->setCurrentText(QString(enumDataBits.valueToKey(QSerialPort::Data8)));
}
