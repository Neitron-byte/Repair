#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "console.h"
#include <QSerialPortInfo>
#include <QDebug>
#include <QMessageBox>
#include <QToolBox>
#include <QWidget>
#include <QStringList>
#include <QComboBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_console(new Console)
     , m_serial(new QSerialPort(this))
    , m_status(new QLabel)

{


    ui->setupUi(this);

    connect(m_serial, &QSerialPort::readyRead, this, &MainWindow::readData);
    connect(m_serial, &QSerialPort::errorOccurred, this, &MainWindow::handleError);

    //получение доступных COM-портов
    foreach (const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts())
        {
            m_listCom << serialPortInfo.portName();
            //ui->comboBox_dev_COM->addItem(serialPortInfo.portName());
        }

    //добавление в UI портов
    ui->comboBox->addItems(m_listCom);

    ui->statusbar->addWidget(m_status);

    QStringList Model;
    Model << "ES96"<<"A96"<<"A97";

    ui->comboBox_dev->addItems(Model);

    //добавление виджетов на форму
    ui->toolBox_Device->setItemText(0,"E34");
    ui->toolBox_Device->setItemText(1,"E18");

    ui->verticalLayout->addWidget(m_console);

    this->setWindowTitle(tr("Repair"));

    ui->plainTextEdit_Comm->setReadOnly(true);
    ui->comboBox_SIM->addItem(" ");
    ui->comboBox_SIM->addItem("1");
    ui->comboBox_SIM->addItem("2");


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

       m_console->putData("Соединение установлено!\r");
       m_console->putData("Если не появилась надпись \"MCU RESET\" - перезапустите устройство\r");
       m_console->putData("Выберите ошибку -> путем последовательной подачи команд проверяйте соответствующие цепи на ПУ\r");
       m_console->putData("Сначала выводится команда, на следующей строке - ответ. Если на конце 1 - включение успешно, иначе 0\r");


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


void MainWindow::on_pushButton_BZ_ON_clicked()
{
    QByteArray qb (":BUZ1\r");
    m_serial->write(qb);

}


void MainWindow::on_pushButton_BZ_OFF_clicked()
{
    QByteArray qb (":BUZ0\r");
    m_serial->write(qb);

}


void MainWindow::on_pushButton_GSM_ON_clicked()
{

    QByteArray cmd (":GSMPWR1\r");
    m_serial->write(cmd);
}


void MainWindow::on_pushButton_GPS_ON_clicked()
{
    QByteArray cmd (":GPSPWR1\r");
    m_serial->write(cmd);

}


void MainWindow::on_pushButton_MIC_PWR_clicked()
{
    QByteArray cmd (":MIC1\r");
    m_serial->write(cmd);
}


void MainWindow::on_pushButton_ON_DIV_clicked()
{
    QByteArray cmd (":ANINDIV1\r");
    m_serial->write(cmd);

}


void MainWindow::on_pushButton_Open_COM_clicked()
{
    m_serial->setPortName(ui->comboBox->currentText());
    m_serial->setBaudRate(QSerialPort::Baud9600);
    m_serial->setParity(QSerialPort::NoParity);
    m_serial->setDataBits(QSerialPort::Data8);
    m_serial->setStopBits(QSerialPort::OneStop);
    m_serial->setFlowControl(QSerialPort::NoFlowControl);
    openPort();

}


void MainWindow::handleError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError) {
        QMessageBox::critical(this, tr("Critical Error"), m_serial->errorString());
        closeSerialPort();
    }
}

void MainWindow::closeSerialPort()
{
    if (m_serial->isOpen())
        m_serial->close();
    m_console->setEnabled(false);
    showStatusMessage(tr("Disconnected"));
}




void MainWindow::on_toolBox_Device_currentChanged(int index)
{
    ui->plainTextEdit_Comm->clear();
    qDebug() << index;
    switch (index) {
    case 0:
        ui->plainTextEdit_Comm->appendPlainText("Последовательность подачи команд: GSM PWR -> GPS PWR -> MIC PWR -> ON DIV -> OUT CTRL -> BUZZER.\r");
        ui->plainTextEdit_Comm->appendPlainText("После подачи всех команд проверьте наличие питания: GPS_PWR_C, MICPWR, NetR36_1 - 0,NetR41_2 - ~12В\r");
        break;
    case 1:
        ui->plainTextEdit_Comm->appendPlainText("Последовательность подачи команд: GSM PWR -> Sel SIM -> Mode -> GSM ICCID");
        break;
    case 2:
        ui->plainTextEdit_Comm->appendPlainText("Последовательность подачи команд: GSM PWR -> GPS PWR -> MIC PWR -> ON DIV -> OUT CTRL -> BUZZER");
        break;
    default:
        break;
    }
}




void MainWindow::on_pushButton_GSM_ON_2_clicked()
{
    QByteArray cmd (":GSMPWR1\r");
    m_serial->write(cmd);
}


void MainWindow::on_comboBox_SIM_currentIndexChanged(int index)
{

    if (index == 1){
        QByteArray cmd (":GSMSIM1\r");
        m_serial->write(cmd);
    }
    else if(index == 2) {
        QByteArray cmd (":GSMSIM2\r");
        m_serial->write(cmd);
    }

}


void MainWindow::on_pushButton_clicked()
{
    QByteArray cmd (":GSM ICCID?\r");
    m_serial->write(cmd);
}

