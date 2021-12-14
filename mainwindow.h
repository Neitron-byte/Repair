#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QLabel>



QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class Console;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_pushButton_BZ_ON_clicked();

    void on_pushButton_BZ_OFF_clicked();

    void on_pushButton_GSM_ON_clicked();

    void on_pushButton_GPS_ON_clicked();

    void on_pushButton_MIC_PWR_clicked();

    void on_pushButton_ON_DIV_clicked();

    void readData();

    void on_pushButton_Open_COM_clicked();

    void handleError(QSerialPort::SerialPortError error);

    void closeSerialPort();

    void on_toolBox_Device_currentChanged(int index);



    void on_pushButton_GSM_ON_2_clicked();

    void on_comboBox_SIM_currentIndexChanged(int index);

    void on_pushButton_clicked();

private:

    void showStatusMessage(const QString &message);

    void openPort();




    Ui::MainWindow *ui;
    //консоль
    Console *m_console = nullptr;
    // COM порт
    QSerialPort* m_serial;
    //список доступных портов
    QStringList m_listCom;
    //статус подключения к COM
    QLabel *m_status = nullptr;



};
#endif // MAINWINDOW_H
