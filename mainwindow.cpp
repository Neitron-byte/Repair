#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_console(new Console)
{
    ui->setupUi(this);
    m_serial = new QSerialPort(this);
    m_serial->setPortName("COM4");
    m_serial->setBaudRate(QSerialPort::Baud9600);
    m_serial->setParity(QSerialPort::NoParity);
    m_serial->setDataBits(QSerialPort::Data8);
    m_serial->setStopBits(QSerialPort::OneStop);
    m_serial->setFlowControl(QSerialPort::NoFlowControl);
    openPort();

    //m_console->setEnabled(false);
    //ui->widget_Console->set
    //setCentralWidget(m_console);

    QStringList Model;
    Model << "ES96"<<"A96"<<"A97";

    ui->comboBox_dev->addItems(Model);

    ui->toolBox_Device->setItemText(0,"E34");
    ui->toolBox_Device->setItemText(1,"E18");

    m_console->setEnabled(false);
    connect(m_serial, &QSerialPort::readyRead, this, &MainWindow::readData);




}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openPort()
{
    if (m_serial->open(QIODevice::ReadWrite)){
        qDebug() << tr ("Port open");
        m_console->setEnabled(true);

    } else {
        qDebug() << tr ("Port not open");
        QMessageBox::critical(this, tr("Error"), m_serial->errorString());
    }

}

void MainWindow::readData()
{
    const QByteArray data = m_serial->readAll();
    m_console->putData(data);
}


void MainWindow::writeSerial(const QString Request)
{
    const QByteArray requestData = Request.toUtf8();
    m_console->putData(requestData);
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
    //this->readSerial();

}


void MainWindow::on_pushButton_BZ_OFF_clicked()
{
    const QString RequestOFFBZ = ":BUZ0\r";
    this->writeSerial(RequestOFFBZ);
    //this->readSerial();
}


void MainWindow::on_pushButton_GSM_ON_clicked()
{
    const QString RequestONGSM = ":GSMPWR1\r";
    this->writeSerial(RequestONGSM);
    //this->readSerial();

}


void MainWindow::on_pushButton_GPS_ON_clicked()
{
    const QString RequestONGPS = ":GPSPWR1\r";
    this->writeSerial(RequestONGPS);
    //this->readSerial();
}


void MainWindow::on_pushButton_MIC_PWR_clicked()
{
    const QString RequestONMIC = ":MIC1\r";
    this->writeSerial(RequestONMIC);
    //this->readSerial();
}


void MainWindow::on_pushButton_ON_DIV_clicked()
{
    const QString RequestONDIV = ":ANINDIV1\r";
    this->writeSerial(RequestONDIV);
    //this->readSerial();
    const QString RequestOUT = ":OUT CTRL 7-,8-\r";
    this->writeSerial(RequestOUT);
    //this->readSerial();
}

