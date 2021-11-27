#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
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

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openPort()
{
    if (m_serial->open(QIODevice::ReadWrite)){
        qDebug() << tr ("Port open");
    } else {
        qDebug() << tr ("Port not open");
        QMessageBox::critical(this, tr("Error"), m_serial->errorString());
    }

}

void MainWindow::writeSerial(const QString Request)
{
    const QByteArray requestData = Request.toUtf8();
    m_serial->write(requestData);
}

void MainWindow::readSerial()
{
    if (m_serial->waitForBytesWritten()) {
        if (m_serial->waitForReadyRead()) {
            QByteArray responseData = m_serial->readAll();
            const QString response = QString::fromUtf8(responseData);
            qDebug() << response;
        }
        if(m_serial->bytesAvailable() != 0){
            QByteArray responseData = m_serial->readAll();
        }
    }


}


void MainWindow::on_pushButton_BZ_ON_clicked()
{
    const QString RequestONBZ = ":BUZ1\r";
    this->writeSerial(RequestONBZ);
    this->readSerial();

}


void MainWindow::on_pushButton_BZ_OFF_clicked()
{
    const QString RequestOFFBZ = ":BUZ0\r";
    this->writeSerial(RequestOFFBZ);
    this->readSerial();
}


void MainWindow::on_pushButton_GSM_ON_clicked()
{
    const QString RequestONGSM = ":GSMPWR1\r";
    this->writeSerial(RequestONGSM);
    this->readSerial();

}

