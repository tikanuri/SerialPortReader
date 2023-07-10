#include "serialwidget.h"
#include "ui_serialwidget.h"
#include "utils.h"

SerialWidget::SerialWidget(bool visibleMinusButton, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SerialWidget),
    model(new SerialModel(this)),
    serialPort(this),
    tcpSocket(this),
    comboBoxUpdateEventFilter(this),
    portInfoMap()
{
    ui->setupUi(this);

    //-- Combo Box --
    ui->comboBoxPort->installEventFilter(&comboBoxUpdateEventFilter);
    connect(&comboBoxUpdateEventFilter,&ComboBoxUpdateEventFilter::clicked,this,&SerialWidget::updatePortInfo);
    ui->comboBoxBaudrate->setEditable(true);

    //-- Push Button
    ui->pushButtonMinus->setVisible(visibleMinusButton);
    connect(ui->pushButtonPlus, &QPushButton::clicked, this, [&](){emit clickedPlus();});
    connect(ui->pushButtonMinus, &QPushButton::clicked, this, [&](){emit clickedMinus();});
    connect(ui->pushButtonStart,&QPushButton::clicked,this,&SerialWidget::changeState);
    connect(ui->pushButtonSend, &QPushButton::clicked, this, &SerialWidget::write);
    connect(ui->pushButtonClear, &QPushButton::clicked, model, &SerialModel::clearSerialData);

    //-- Serial Port --
    connect(&serialPort, &QSerialPort::readyRead, this, &SerialWidget::read);

    //-- TCP Socket --
    connect(&tcpSocket, &QTcpSocket::connected, this, &SerialWidget::connectedTcpSocket);
    connect(&tcpSocket, &QTcpSocket::errorOccurred, this, &SerialWidget::tcpSocketError);
    connect(&tcpSocket, &QTcpSocket::readyRead, this, &SerialWidget::read);

    //-- Table View --
    ui->tableView->setModel(model);
    ui->tableView->setShowGrid(false);
    ui->tableView->setWordWrap(true);
    ui->tableView->setColumnHidden(0,true);
    ui->tableView->setColumnHidden(2,true);
    ui->tableView->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);

    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->verticalHeader()->hide();
    ui->tableView->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableView->scrollToTop();

    connect(ui->checkBoxTime, &QCheckBox::stateChanged, this, [&](int state){
        ui->tableView->setColumnHidden(0, !static_cast<bool>(state));
        if(state == Qt::Checked)
            ui->tableView->resizeColumnToContents(0);
    });
    connect(ui->checkBoxHex, &QCheckBox::stateChanged, this, [&](int state){
        ui->tableView->setColumnHidden(2, !static_cast<bool>(state));
        static bool firstUse = true;
        if(firstUse && state == Qt::Checked)
        {
            int size = (ui->tableView->columnWidth(1) + ui->tableView->columnWidth(2))/2;
            ui->tableView->setColumnWidth(1,size);
            ui->tableView->setColumnWidth(2,size);
            firstUse = false;
        }
    });

    updatePortInfo();
    initMetaEnum();
    ui->comboBoxTypeSend->addItems(QStringList() << "Text" << "Hex");
    ui->debugInfo->setText("Close");
}

SerialWidget::~SerialWidget()
{
    delete ui;
    delete model;
}

void SerialWidget::changeState()
{
    switch (ui->tabSwitchInterface->currentIndex()) {
    case 0:
        if(serialPort.isOpen())
        {
            serialPort.close();
            ui->pushButtonStart->setText("Start");
            ui->debugInfo->setText(QString("Close serial port"));
            ui->comboBoxPort->setEnabled(true);
            ui->tabSwitchInterface->setTabEnabled(1,true);
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
                ui->tabSwitchInterface->setTabEnabled(1,false);
            }
            else
            {
                ui->debugInfo->setText(QString("Can`t be opened"));
            }
        }
        break;
    case 1:
        if(tcpSocket.isOpen())
        {
            tcpSocket.close();
            ui->pushButtonStart->setText("Start");
            ui->debugInfo->setText(QString("Close tcp socket"));
            ui->tabSwitchInterface->setTabEnabled(0,true);
        }
        else
        {
            tcpSocket.connectToHost(ui->lineEditHost->text(),ui->lineEditPort->text().toUShort());
            ui->pushButtonStart->setText("Stop");
            ui->debugInfo->setText(QString("Connecting to server..."));
            ui->tabSwitchInterface->setTabEnabled(0,false);
        }
        break;
    default:
        break;
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
            QString key = info.portName();
            if(!info.description().isEmpty())
            {
                key += QString(" [%0]").arg(info.description());
            }
            keys.removeAll(key);
            portInfoMap[key] = info;
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
    QByteArray ba;
    switch (ui->tabSwitchInterface->currentIndex()) {
    case 0:
        ba = serialPort.readAll();
        break;
    case 1:
        ba = tcpSocket.readAll();
    default:
        break;
    }

    if(!ba.isEmpty())
    {
        model->addSerialData(ba);
    }
    if(ui->checkBoxAutoScroll->checkState())
    {
        ui->tableView->scrollToBottom();
    }
}

void SerialWidget::write()
{
    QString str = ui->lineEditSend->text();
    if(!str.isEmpty())
    {
        QByteArray ba;
        switch (ui->comboBoxTypeSend->currentIndex())
        {
        case Text:
            ba = str.toUtf8();
            break;
        case Hex:
            ba = QByteArray::fromHex(str.toUtf8());
            break;
        }


        switch (ui->tabSwitchInterface->currentIndex())
        {
        case 0:
            if(!serialPort.isOpen())
            {
                ui->debugInfo->setText(QString("Port is not open"));
                return;
            }
            serialPort.write(ba);
            break;
        case 1:
            if(!tcpSocket.isOpen())
            {
                ui->debugInfo->setText(QString("Socket is not open"));
                return;
            }
            tcpSocket.write(ba);
            break;

        default:
            break;
        }
    }


}

void SerialWidget::connectedTcpSocket()
{
    ui->debugInfo->setText(QString("Connected to %1:%2")
                            .arg(tcpSocket.peerAddress().toString())
                            .arg(tcpSocket.peerPort()));
}

void SerialWidget::tcpSocketError(QAbstractSocket::SocketError socketError)
{
    //Q_UNUSED(socketError);
    tcpSocket.close();
    ui->pushButtonStart->setText("Start");
    ui->debugInfo->setText(QString("Tcp socket error: %1").arg(socketError));
    ui->tabSwitchInterface->setTabEnabled(0,true);

}
