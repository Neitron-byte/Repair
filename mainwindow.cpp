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
#include <QIcon>
#include <QString>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_console(new Console)
    , m_serial(new QSerialPort(this))
    , m_status(new QLabel)
    , m_comments(new comments)

{


    ui->setupUi(this);

    connect(m_serial, &QSerialPort::readyRead, this, &MainWindow::readData);
    connect(m_serial, &QSerialPort::errorOccurred, this, &MainWindow::handleError);
    m_console->setLocalEchoEnabled(true);
    connect(m_console, &Console::getData, this, &MainWindow::writeData);

    //получение доступных COM-портов
    foreach (const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts())
        {
            m_listCom << serialPortInfo.portName();
            //ui->comboBox_dev_COM->addItem(serialPortInfo.portName());
        }

    //добавление в UI портов
    ui->comboBox->addItems(m_listCom);
    //в статус бар
    ui->statusbar->addWidget(m_status);
    //добавление консоли на компоновщик
    ui->verticalLayout->addWidget(m_console);
    //Название
    this->setWindowTitle(tr("Diagnostic Software Starline"));
    //lock ToolBox
    ui->toolBox_Device->setEnabled(false);

    //иниты
    //signal-slot ы
    initActionsConnections();
    //добавление информации на виджеты
    initWidgets();
    //первоначальная инициализация ToolBox и Db
    initToolBox_db();
    //комментарии и Db
    ui->verticalLayout_4->addWidget(m_comments);

    //добавление свойств к виджетам.
    this->setProperty();

    //добавление Checkbox-ов в вектор для проверки
    m_vector_check_starter.push_back(ui->checkBox_23);
    m_vector_check_starter.push_back(ui->checkBox_24);
    m_vector_check_starter.push_back(ui->checkBox_25);
    m_vector_check_starter.push_back(ui->checkBox_31);
    m_vector_check_starter.push_back(ui->checkBox_32);
}

MainWindow::~MainWindow()
{
    delete ui;
    if(m_serial->isOpen()){
        m_serial->close();
    }
    if (m_serial){
        delete m_serial;
    }
    if(m_status){
        delete m_status;
    }
    if (m_console){
        delete m_console;
    }
    if (model){
        delete model;
    }


}

void MainWindow::showStatusMessage(const QString &message)
{
    m_status->setText(message);
}

void MainWindow::openPort()
{
    if (m_serial->open(QIODevice::ReadWrite)){
        qDebug() << tr ("Port open");
        showStatusMessage(tr("Подключено к %1 : %2, %3, %4, %5, %6")
                          .arg(m_serial->portName()).arg(m_serial->baudRate()).arg(m_serial->dataBits())
                          .arg(m_serial->parity()).arg(m_serial->stopBits()).arg(m_serial->flowControl()));

       m_console->putData("СOM порт открыт!\r");             
       ui->toolBox_Device->setEnabled(true);

    } else {
        qDebug() << tr ("Port not open");
        QMessageBox::critical(this, tr("Error"), m_serial->errorString());
    }

}

void MainWindow::clear()
{
    m_console->clear();

}

void MainWindow::readData()
{
    const QByteArray data = m_serial->readAll();
    m_console->putData(data);

}

void MainWindow::writeData(const QByteArray &data)
{
    m_serial->write(data);
}


void MainWindow::on_pushButton_BZ_ON_clicked()
{
    QByteArray qb (":BUZ1\r");
    writeData(qb);

}


void MainWindow::on_pushButton_BZ_OFF_clicked()
{
    QByteArray qb (":BUZ0\r");
    writeData(qb);

}


void MainWindow::on_pushButton_GSM_ON_clicked()
{

    QByteArray cmd (":GSMPWR1\r");
    writeData(cmd);

}


void MainWindow::on_pushButton_GPS_ON_clicked()
{
    QByteArray cmd (":GPSPWR1\r");
    writeData(cmd);
}


void MainWindow::on_pushButton_MIC_PWR_clicked()
{
    QByteArray cmd (":MIC1\r");
    writeData(cmd);
}


void MainWindow::on_pushButton_ON_DIV_clicked()
{
    QByteArray cmd (":ANINDIV1\r");
    writeData(cmd);

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
    m_comments->setCurrentTable(m_test_db_error[index].second.first);

}




void MainWindow::on_pushButton_GSM_ON_2_clicked()
{
    QByteArray cmd (":GSMPWR1\r");    
    writeData(cmd);

}


void MainWindow::on_comboBox_SIM_currentIndexChanged(int index)
{

    if (index == 1){
        QByteArray cmd (":GSMSIM1\r");
        writeData(cmd);
    }
    else if(index == 2) {
        QByteArray cmd (":GSMSIM2\r");
        writeData(cmd);        
    }

}


void MainWindow::on_pushButton_clicked()
{
    QByteArray cmd (":GSM ICCID?\r");
    writeData(cmd);    
}


void MainWindow::on_pushButton_CH02_OFF_clicked()
{
    QByteArray cmd (":OUT CTRL 7-,8-\r");
    writeData(cmd);
}

void MainWindow::initActionsConnections(){
     connect(ui->action_Connect_2, &QAction::triggered, this, &MainWindow::on_pushButton_Open_COM_clicked);
     connect(ui->action_Disconnect_2, &QAction::triggered, this, &MainWindow::closeSerialPort);
     connect(ui->action_Clear_2,&QAction::triggered,this, &MainWindow::clear);
     connect(ui->action_About,&QAction::triggered,this, &MainWindow::abouts);     
}

void MainWindow::initWidgets()
{

    //установка наименвоан виджетов на форму
    ui->comboBox_SIM->addItem(" ");
    ui->comboBox_SIM->addItem("1");
    ui->comboBox_SIM->addItem("2");
}


void MainWindow::setProperty()
{
    //для Com

    //для t_starter
    ui->checkBox_23->setProperty("id",23);
    ui->checkBox_24->setProperty("id",24);
    ui->checkBox_25->setProperty("id",25);
    ui->checkBox_31->setProperty("id",31);
    ui->checkBox_32->setProperty("id",32);


}

void MainWindow::initToolBox_db()
{
    for (int var = 0; var < m_test_db_error.size(); ++var) {
        ui->toolBox_Device->setItemText(var, QString("%1 Ошибки: %2") .arg(m_test_db_error[var].first)
                                        .arg(m_test_db_error[var].second.second));
    }

    int index = ui->toolBox_Device->currentIndex();
    m_comments->setCurrentTable(m_test_db_error[index].second.first);

    ui->toolBox_Device->setCurrentIndex(0);
}

void MainWindow::on_pushButton_2_clicked()
{
    QByteArray cmd (":PWR ?\r");
    writeData(cmd);
}


void MainWindow::on_pushButton_lin_split_clicked()
{
    QByteArray cmd (":LIN LOOP 1\r");
    writeData(cmd);   
}


void MainWindow::on_pushButton_lin_split_2_clicked()
{
    QByteArray cmd (":LIN LOOP 0\r");
    writeData(cmd);
}


void MainWindow::on_pushButton_lin_split_status_clicked()
{
    QByteArray cmd (":LIN ?\r");
    writeData(cmd);

}


void MainWindow::on_pushButton_lin_split_OFF_exRelay_clicked()
{
    QByteArray cmd (":OUT CTRL 43-,44+,45+,46+\r");
    writeData(cmd);    
}


void MainWindow::on_pushButton_lin_split_on_clicked()
{
    QByteArray cmd (":LIN SPLIT 1\r");
    writeData(cmd);
}


void MainWindow::on_pushButton_lin_split_off_clicked()
{
    QByteArray cmd (":LIN SPLIT 0\r");
    writeData(cmd);
}


void MainWindow::on_pushButton_lin_split_off_relay_clicked()
{
    QByteArray cmd (":OUT CTRL 39+\r");
    writeData(cmd);
}


void MainWindow::on_pushButton_split_status_clicked()
{
    QByteArray cmd (":LIN SPLIT ?\r");
    writeData(cmd);
}


void MainWindow::on_pushButton_lin_split_ON_exRelay_clicked()
{
    QByteArray cmd (":OUT CTRL 43+\r");
    writeData(cmd);    
}


void MainWindow::on_pushButton_lin_split_on_relay_clicked()
{
    QByteArray cmd (":OUT CTRL 39-\r");
    writeData(cmd);
}

void MainWindow::abouts()
{

        QMessageBox::about(this, tr("Приложениe диагностики Starline."),
                           tr("Приложение разработано для диагностики изделий Starline: ОБ ES96,S96.\r"
                              "Разработал инженер по тестированию НПО \"Starline\" - Якимов Николай Анатольевич. \r"
                              "E-mail: yakimov.na@starline.ru \r"));

}


void MainWindow::on_pushButton_can_test_on_clicked()
{
    QByteArray cmd (":CAN LOOP 1\r");
    writeData(cmd);
}


void MainWindow::on_pushButton_can_test_off_clicked()
{
    QByteArray cmd (":CAN LOOP 0\r");
    writeData(cmd);    
}


void MainWindow::on_pushButton_can_status_clicked()
{
    QByteArray cmd (":CAN ?\r");
    writeData(cmd);
}


void MainWindow::on_pushButton_can_OFF_exRelay_clicked()
{
    QByteArray cmd (":OUT CTRL 43-,44+,45+,46+\r");
    writeData(cmd);  
}


void MainWindow::on_pushButton_can_ON_exRelay_clicked()
{
    QByteArray cmd (":OUT CTRL 43+\r");
    writeData(cmd);   
}


void MainWindow::on_pushButton_temp_clicked()
{
    QByteArray cmd (":ETEMP ?\r");
    writeData(cmd);    
}


void MainWindow::on_pushButton_3_clicked()
{
    QByteArray cmd (":GSM?\r");
    writeData(cmd);
}


void MainWindow::on_pushButton_gsm_mode_clicked()
{
    QByteArray cmd (":GSM MODE1\r");
    writeData(cmd);
}


void MainWindow::on_pushButton_4_clicked()
{
    QByteArray cmd (":GSM RESET\r");
    writeData(cmd);
}


void MainWindow::on_pushButton_ON_clicked()
{
    QByteArray cmd (":OUT CTRL ");
    for (int var = 0; var < m_vector_check_starter.size(); ++var) {
        if (m_vector_check_starter[var]->isChecked()){
            qDebug() << "Выбран " << m_vector_check_starter[var]->property("id").toInt();
            cmd += m_vector_check_starter[var]->property("id").toByteArray();
            cmd += "+,";
        } else {
            cmd += m_vector_check_starter[var]->property("id").toByteArray();
            cmd += "-,";
        }

    }
    cmd+="\r";
    writeData(cmd);
}


void MainWindow::on_pushButton_IN_clicked()
{
    QByteArray cmd (":IN?\r");
    writeData(cmd);
}

void MainWindow::on_pushButton_start_clicked()
{
    QByteArray cmd (":BLE START\r");
    writeData(cmd);
}


void MainWindow::on_pushButton_stop_clicked()
{
    QByteArray cmd (":BLE STOP\r");
    writeData(cmd);
}


void MainWindow::on_pushButton_status_clicked()
{
    QByteArray cmd (":BLE ?\r");
    writeData(cmd);
}


void MainWindow::on_pushButton_t_mem_ON_clicked()
{
    QByteArray cmd (":MEM LOOP 1\r");
    writeData(cmd);

}


void MainWindow::on_pushButton_t_mem_OFF_clicked()
{
    QByteArray cmd (":MEM LOOP 0\r");
    writeData(cmd);
}


void MainWindow::on_pushButton_t_mem_stat_clicked()
{
    QByteArray cmd (":MEM ?\r");
    writeData(cmd);
}


void MainWindow::on_pushButton_t_mem_id_clicked()
{
    QByteArray cmd (":JEDEC_FLASH ?\r");
    writeData(cmd);
}


void MainWindow::on_pushButton_sn_clicked()
{
    QByteArray cmd (":SN?\r");
    writeData(cmd);
}


void MainWindow::on_pushButton_erase_eeprom_clicked()
{
    QByteArray cmd (":MEM ERASE 1\r");
    writeData(cmd);
}


void MainWindow::on_pushButton_erase_flash_clicked()
{
    QByteArray cmd (":MEM ERASE 2\r");
    writeData(cmd);
}


void MainWindow::on_pushButton_lin_start_clicked()
{
    if (ui->radioButton_lin_lin1->isChecked() ) {
        QByteArray cmd (":LIN LOOP 1\r");
        writeData(cmd);
    }
    if (ui->radioButton_lin1_lin2->isChecked() ) {
        QByteArray cmd (":LIN SPLIT 1\r");
        writeData(cmd);
    }
    if (ui->radioButton_lin3_lin4->isChecked() ) {
        QByteArray cmd (":LIN SPLIT 2\r");
        writeData(cmd);
    }
}


void MainWindow::on_pushButton_lin_stop_clicked()
{
    if (ui->radioButton_lin_lin1->isChecked() ) {
        QByteArray cmd (":LIN LOOP 0\r");
        writeData(cmd);
    }
    if (ui->radioButton_lin1_lin2->isChecked() || ui->radioButton_lin3_lin4->isChecked() ) {
        QByteArray cmd (":LIN SPLIT 0\r");
        writeData(cmd);
    }

}


void MainWindow::on_pushButton_lin_status_clicked()
{
    if (ui->radioButton_lin_lin1->isChecked() ) {
        QByteArray cmd (":LIN ?\r");
        writeData(cmd);
    }
    if (ui->radioButton_lin1_lin2->isChecked() || ui->radioButton_lin3_lin4->isChecked() ) {
        QByteArray cmd (":LIN SPLIT ?\r");
        writeData(cmd);
    }
}

