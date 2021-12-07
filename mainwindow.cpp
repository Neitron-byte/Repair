#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_status(new QLabel)
{


    ui->setupUi(this);

    //получение доступных COM-портов
    foreach (const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts())
        {
            m_listCom << serialPortInfo.portName();
            //ui->comboBox_dev_COM->addItem(serialPortInfo.portName());
        }

    ui->comboBox->addItems(m_listCom);

    ui->statusbar->addWidget(m_status);

    QStringList Model;
    Model << "ES96"<<"A96"<<"A97";

    ui->comboBox_dev->addItems(Model);

    ui->toolBox_Device->setItemText(0,"E34");
    ui->toolBox_Device->setItemText(1,"E18");   

}

MainWindow::~MainWindow()
{
    delete ui;
    if(m_serial->isOpen()){
        m_serial->close();
    }
    delete m_serial;
    delete m_status;
}

void MainWindow::showStatusMessage(const QString &message)
{
    m_status->setText(message);
}


void MainWindow::openPort()
{
    if (m_serial->open(QIODevice::ReadWrite)){
        qDebug() << tr ("Port open");
        showStatusMessage(tr("Connected to %1 : %2, %3, %4, %5, %6")
                          .arg(m_serial->portName()).arg(m_serial->baudRate()).arg(m_serial->dataBits())
                          .arg(m_serial->parity()).arg(m_serial->stopBits()).arg(m_serial->flowControl()));

    } else {
        qDebug() << tr ("Port not open");
        QMessageBox::critical(this, tr("Error"), m_serial->errorString());
    }

}

void MainWindow::readData()
{
    if (m_serial->waitForReadyRead(1000)){
    const QByteArray data = m_serial->readAll();
    ui->plainTextEdit->appendPlainText(data.data());

    }
}


void MainWindow::writeSerial(const QString Request)
{
    const QByteArray requestData = Request.toUtf8();    
    qDebug() << requestData;
    m_serial->write(requestData);
}

//void MainWindow::readSerial()
//{
//    /*if (m_serial->waitForBytesWritten()) {
//        if (m_serial->waitForReadyRead()) {
//            QByteArray responseData = m_serial->readAll();
//            const QString response = QString::fromUtf8(responseData);
//            qDebug() << response;
//        }
//        if(m_serial->bytesAvailable() != 0){
//            QByteArray responseData = m_serial->readAll();
//        }
//    }*/
//    QByteArray responseData;
//    while(!m_serial->atEnd()){
//        responseData += m_serial->readAll();
//        //const QString response = QString::fromUtf8(responseData);

//        //qDebug() << response;
//    }
//     m_console->putData(responseData);
//    //const QByteArray data = m_serial->readAll();


//}


void MainWindow::on_pushButton_BZ_ON_clicked()
{
    const QString RequestONBZ = ":BUZ1\r";
    this->writeSerial(RequestONBZ);


}


void MainWindow::on_pushButton_BZ_OFF_clicked()
{
    const QString RequestOFFBZ = ":BUZ0\r";
    this->writeSerial(RequestOFFBZ);

}


void MainWindow::on_pushButton_GSM_ON_clicked()
{
    const QString RequestONGSM = ":GSMPWR1\r";
    this->writeSerial(RequestONGSM);


}


void MainWindow::on_pushButton_GPS_ON_clicked()
{
    const QString RequestONGPS = ":GPSPWR1\r";
    this->writeSerial(RequestONGPS);

}


void MainWindow::on_pushButton_MIC_PWR_clicked()
{
    const QString RequestONMIC = ":MIC1\r";
    this->writeSerial(RequestONMIC);

}


void MainWindow::on_pushButton_ON_DIV_clicked()
{
    const QString RequestONDIV = ":ANINDIV1\r";
    this->writeSerial(RequestONDIV);

    const QString RequestOUT = ":OUT CTRL 7-,8-\r";
    this->writeSerial(RequestOUT);

}


void MainWindow::on_pushButton_Open_COM_clicked()
{
    m_serial = new QSerialPort();
    connect(m_serial, &QSerialPort::readyRead, this, &MainWindow::readData);
    m_serial->setPortName(ui->comboBox->currentText());
    m_serial->setBaudRate(QSerialPort::Baud9600);
    m_serial->setParity(QSerialPort::NoParity);
    m_serial->setDataBits(QSerialPort::Data8);
    m_serial->setStopBits(QSerialPort::OneStop);
    m_serial->setFlowControl(QSerialPort::NoFlowControl);
    openPort();

}



