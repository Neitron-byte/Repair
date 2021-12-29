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

    ui->statusbar->addWidget(m_status);

    //Модели - в разработке
    QStringList Model;
    Model << "ES96"<<"A96"<<"A97";
    ui->comboBox_dev->addItems(Model);


    ui->verticalLayout->addWidget(m_console);

    this->setWindowTitle(tr("Repair Starline"));

    ui->plainTextEdit_Comm->setReadOnly(true);

    ui->toolBox_Device->setEnabled(false);

    initActionsConnections();
    initWidgets();

    // Цвет шрифта в пояснениях
    QPalette p = palette();
    p.setColor(QPalette::Text, Qt::red);
    ui->plainTextEdit_Comm->setPalette(p);

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
        showStatusMessage(tr("Подключено к %1 : %2, %3, %4, %5, %6")
                          .arg(m_serial->portName()).arg(m_serial->baudRate()).arg(m_serial->dataBits())
                          .arg(m_serial->parity()).arg(m_serial->stopBits()).arg(m_serial->flowControl()));

       m_console->putData("СOM порт открыт!\r");

       ui->plainTextEdit_Comm->appendPlainText("Для подтверждения корректного подключения перезапустите плату и дождитесь появления в консоли - MCU RESET.\r");
       //ui->plainTextEdit_Comm->appendPlainText("Выберите ошибку -> путем последовательной подачи команд проверяйте соответствующие цепи на ПУ\r");
       //ui->plainTextEdit_Comm->appendPlainText("Сначала выводится команда, на следующей строке - ответ. Если на конце ответа 1 - включение успешно, иначе 0\r");
       ui->toolBox_Device->setEnabled(true);


    } else {
        qDebug() << tr ("Port not open");
        QMessageBox::critical(this, tr("Error"), m_serial->errorString());
    }

}

void MainWindow::clear()
{
    m_console->clear();
    ui->plainTextEdit_Comm->clear();

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
    ui->plainTextEdit_Comm->appendPlainText("Проверьте питание SIM800 pin 34, 35 DD5 - 3,9 В\r");
    ui->plainTextEdit_Comm->appendPlainText("Проверьте наличие сигнала подтверждения pin 42 DD5 - 2,8 - 3,2 В\r");
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

    ui->plainTextEdit_Comm->clear();
    qDebug() << index;
    switch (index) {
    case 0:
        ui->plainTextEdit_Comm->appendPlainText("Напряжение питания - 12В. Проверка KL30.");
        break;
    case 1:        
        ui->plainTextEdit_Comm->appendPlainText("Последовательность включения: Питание GSM -> Питание GPS -> Питание MIC -> Вкл. делитель входа -> Выключить выход CH02_C -> Тест BUZZER.\r");
        ui->plainTextEdit_Comm->appendPlainText("После подачи всех команд проверьте наличие питания: GPS_PWR_C, MICPWR, NetR36_1 - 0,NetR41_2 - ~12В\r");
        break;
    case 2:
        ui->plainTextEdit_Comm->appendPlainText("Последовательность подачи команд: Включение GSM -> Выбор SIM -> Считать ICCID\r");
        break;
    case 3:

        break;
    case 4:
        ui->plainTextEdit_Comm->appendPlainText("Проверка работы твердотельного реле DA22.\r Проверьте замыкание контактов pin4,3 DA22.\r");
        break;
    case 5:

        break;
    case 6:

        break;
    case 7:
        ui->plainTextEdit_Comm->appendPlainText("Проверьте на разъеме XS5 перемычки.\r");
        break;
    default:
        break;
    }
}




void MainWindow::on_pushButton_GSM_ON_2_clicked()
{
    QByteArray cmd (":GSMPWR1\r");    
    writeData(cmd);
    ui->plainTextEdit_Comm->appendPlainText("Проверьте питание SIM800 pin 34, 35 DD5 - 3,9 В\r");
    ui->plainTextEdit_Comm->appendPlainText("Проверьте наличие сигнала подтверждения pin 42 DD5 - 2,8 - 3,2 В\r");
    ui->plainTextEdit_Comm->appendPlainText("Проверьте питание переключателя SIM - pin 16/14 DA18 - 1,8 В\r");

}


void MainWindow::on_comboBox_SIM_currentIndexChanged(int index)
{

    if (index == 1){
        QByteArray cmd (":GSMSIM1\r");
        writeData(cmd);
        ui->plainTextEdit_Comm->appendPlainText("Выбрана SIM-карта (SIM2 на плате). Проверьте сигнал SIM_SEL pin 2 DA18 - 0 В \r");
    }
    else if(index == 2) {
        QByteArray cmd (":GSMSIM2\r");
        writeData(cmd);
        ui->plainTextEdit_Comm->appendPlainText("Выбрана SIM-карта или SIM-чип (SIM1 на плате). Проверьте сигнал SIM_SEL pin 2 DA18 - 3,2 В \r");
    }

}


void MainWindow::on_pushButton_clicked()
{
    QByteArray cmd (":GSM ICCID?\r");
    writeData(cmd);
    ui->plainTextEdit_Comm->appendPlainText("Сверьте полученный ICCID с номером на SIM-карте или SIM-чипе \r");
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

    ui->comboBox_SIM->addItem(" ");
    ui->comboBox_SIM->addItem("1");
    ui->comboBox_SIM->addItem("2");

    //установка наименвоан виджетов на форму
    ui->toolBox_Device->setItemText(0,"E6. Тест питания.");
    ui->toolBox_Device->setItemText(1,"E34. Проверка Buzzer.");
    ui->toolBox_Device->setItemText(2,"E18/E35. Проверка GSM.");
    ui->toolBox_Device->setItemText(3,"E8. Проверка входов/выходов.");
    ui->toolBox_Device->setItemText(4,"E3/E37. Проверка LIN.");
    ui->toolBox_Device->setItemText(5,"E4. Проверка CAN.");
    ui->toolBox_Device->setItemText(6,"E2. Проверка памяти.");
    ui->toolBox_Device->setItemText(7,"E19. Проверка GPS.");
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
    ui->plainTextEdit_Comm->appendPlainText("Передача данных осуществляется по цепи LIN_C -> LIN1_C через внешнее реле. \r");
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
     ui->plainTextEdit_Comm->appendPlainText("Проверьте LIN3_C - 0 В \r");
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
    ui->plainTextEdit_Comm->appendPlainText("Проверьте LIN3_C - 12 В \r");
}


void MainWindow::on_pushButton_lin_split_on_relay_clicked()
{
    QByteArray cmd (":OUT CTRL 39-\r");
    writeData(cmd);
}

void MainWindow::abouts()
{

        QMessageBox::about(this, tr("About Simple Terminal"),
                           tr("The <b>Simple Terminal</b> example demonstrates how to "
                              "use the Qt Serial Port module in modern GUI applications "
                              "using Qt, with a menu bar, toolbars, and a status bar."));

}


void MainWindow::on_pushButton_can_test_on_clicked()
{
    QByteArray cmd (":CAN LOOP 1\r");
    writeData(cmd);
    ui->plainTextEdit_Comm->appendPlainText("Передача данных осуществляется через внешнее реле и через цепи CAN1_H - CAN2_H и CAN1_L - CAN2_L, соединенных друг с другом через терминальный резистор 120 Ом.\r"
"Управление реле осуществляется с помощью LIN4_C.\r");

}


void MainWindow::on_pushButton_can_test_off_clicked()
{
    QByteArray cmd (":CAN LOOP 0\r");
    writeData(cmd);
    ui->plainTextEdit_Comm->appendPlainText("Тест завершен\r");
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
     ui->plainTextEdit_Comm->appendPlainText("\r");
}


void MainWindow::on_pushButton_can_ON_exRelay_clicked()
{
    QByteArray cmd (":OUT CTRL 43+\r");
    writeData(cmd);
    ui->plainTextEdit_Comm->appendPlainText("Команда на замыкание внешнего реле. Проверьте LIN4_C - 12 В\r");
}


void MainWindow::on_pushButton_temp_clicked()
{
    QByteArray cmd (":ETEMP ?\r");
    writeData(cmd);
    ui->plainTextEdit_Comm->appendPlainText("Если в ответе ERR, то проверьте подключение внешнего датчика температуры.\r");
    ui->plainTextEdit_Comm->appendPlainText("Возвращается температура в градусах Цельсия.\r");
}

