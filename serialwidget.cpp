#include "serialwidget.h"
#include "ui_serialwidget.h"
#include "utils.h"

SerialWidget::SerialWidget(bool visibleMinusButton, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SerialWidget),
    view(new SerialView(this)),
    model(new SerialModel(this)),
    serialPort(this),
    comboBoxUpdateEventFilter(this),
    portInfoMap()
{
    ui->setupUi(this);

    view->setObjectName("SerialView");
    view->setModel(model);
    ui->verticalLayout->insertWidget(2,view);


    ui->pushButtonMinus->setVisible(visibleMinusButton);
    qDebug() << "Constructor:  " << (uint64_t)this;

    connect(ui->pushButtonPlus, &QPushButton::clicked, this, [&](){emit clickedPlus();});
    connect(ui->pushButtonMinus, &QPushButton::clicked, this, [&](){emit clickedMinus();});

    //comboBox
    ui->comboBoxPort->installEventFilter(&comboBoxUpdateEventFilter);
    connect(&comboBoxUpdateEventFilter,&ComboBoxUpdateEventFilter::clicked,this,&SerialWidget::updatePortInfo);
    ui->comboBoxBaudrate->setEditable(true);

    connect(ui->pushButtonStart,&QPushButton::clicked,this,&SerialWidget::changeState);
    //connect(&serialPort, &QSerialPort::readyRead, this, &SerialWidget::read);
    connect(ui->pushButtonSend, &QPushButton::clicked, this, &SerialWidget::read);
    connect(ui->pushButtonClear, &QPushButton::clicked, model, &SerialModel::clearSerialData);
//    connect(ui->checkBoxTime, &QCheckBox::stateChanged, this, [&](int state){
//        ui->tableView->setColumnHidden(0, !static_cast<bool>(state));
//        ui->tableView->resizeColumnToContents(0);
//    });
//    connect(ui->checkBoxHex, &QCheckBox::stateChanged, this, [&](int state){
//        ui->tableView->setColumnHidden(2, !static_cast<bool>(state));
//    });

//    ui->tableView->setModel(model);
//    ui->tableView->setShowGrid(false);
//    ui->tableView->setWordWrap(false);
//    ui->tableView->horizontalHeader()->setStretchLastSection(true);
//    ui->tableView->setColumnHidden(0,true);
//    ui->tableView->setColumnHidden(2,true);
//    ui->tableView->verticalHeader()->hide();
//    ui->tableView->horizontalHeader()->hide();

    updatePortInfo();
    initMetaEnum();
}

SerialWidget::~SerialWidget()
{
    qDebug() << "Destructor:  " << (uint64_t)this;
    delete ui;
    delete model;
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
            ui->debugInfo->setText(QString("Open: %0 %1; %2 %3 %4")
                                   .arg(ui->comboBoxPort->currentText())
                                   .arg(ui->comboBoxBaudrate->currentText())
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

QPushButton *SerialWidget::getPlusButton()
{
    return ui->pushButtonPlus;
}

QPushButton *SerialWidget::getMinusButton()
{
    return ui->pushButtonMinus;
}

void SerialWidget::read()
{
    //QByteArray ba = serialPort.readAll();
    if(!ui->lineEditSend->text().isEmpty())
    {
        model->addSerialData(QDateTime::currentDateTime(), ui->lineEditSend->text().toUtf8());
    }
}
