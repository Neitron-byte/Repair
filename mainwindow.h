#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QLabel>
#include <QSqlTableModel>
#include <QTableView>
#include <QVector>
#include <QCheckBox>
#include <QPair>
#include "comments.h"




QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class Console;
class QSqlTableModel;

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
    void writeData(const QByteArray &data);

    void on_pushButton_Open_COM_clicked();

    void handleError(QSerialPort::SerialPortError error);

    void closeSerialPort();

    void on_toolBox_Device_currentChanged(int index);

    void on_pushButton_GSM_ON_2_clicked();

    void on_comboBox_SIM_currentIndexChanged(int index);

    void on_pushButton_clicked();

    void on_pushButton_CH02_OFF_clicked();

    void openPort();

    void clear();

    void on_pushButton_2_clicked();

    void on_pushButton_lin_split_clicked();

    void on_pushButton_lin_split_2_clicked();

    void on_pushButton_lin_split_status_clicked();

    void on_pushButton_lin_split_OFF_exRelay_clicked();

    void on_pushButton_lin_split_on_clicked();

    void on_pushButton_lin_split_off_clicked();

    void on_pushButton_lin_split_off_relay_clicked();

    void on_pushButton_split_status_clicked();

    void on_pushButton_lin_split_ON_exRelay_clicked();

    void on_pushButton_lin_split_on_relay_clicked();

    void abouts();

    void on_pushButton_can_test_on_clicked();

    void on_pushButton_can_test_off_clicked();

    void on_pushButton_can_status_clicked();

    void on_pushButton_can_OFF_exRelay_clicked();

    void on_pushButton_can_ON_exRelay_clicked();

    void on_pushButton_temp_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_gsm_mode_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_ON_clicked();

    void on_pushButton_IN_clicked();    

    void on_pushButton_start_clicked();

    void on_pushButton_stop_clicked();

    void on_pushButton_status_clicked();    

    void on_pushButton_t_mem_ON_clicked();

    void on_pushButton_t_mem_OFF_clicked();

    void on_pushButton_t_mem_stat_clicked();

    void on_pushButton_t_mem_id_clicked();




    void on_pushButton_sn_clicked();

    void on_pushButton_erase_eeprom_clicked();

    void on_pushButton_erase_flash_clicked();

    void on_pushButton_lin_start_clicked();

    void on_pushButton_lin_stop_clicked();

    void on_pushButton_lin_status_clicked();

    void on_pushButton_lin_split2_on_relay_clicked();

    void on_pushButton_lin_split2_off_relay_clicked();

private:

    void showStatusMessage(const QString &message);
    void initActionsConnections();
    void initWidgets();
    void setProperty();
    void initToolBox_db();

    Ui::MainWindow *ui;

    //консоль
    Console *m_console = nullptr;

    // COM порт
    QSerialPort* m_serial;

    //список доступных портов
    QStringList m_listCom;

    //статус подключения к COM
    QLabel *m_status = nullptr;

    //модель таблицы
    QSqlTableModel *model;

    //перечень таблиц
    QStringList m_listTable;

    //Комментарии
    comments* m_comments = nullptr;

    //для стартера
    QVector<QCheckBox*> m_vector_check_starter;


    QVector<QPair<QString,QPair<QString,QString>>> m_test_db_error {
        {"Проверка SN. ",{"t_sn","E0"}},
        {"Тестирование памяти. ", {"t_mem","E2,E15"}},
        {"Тестирование LIN. ",{"t_lin","E3,E5,E37,E39"}},
        {"Тестирование CAN. ",{"t_can","E4,E5"}},
        {"Проверка питания. ", {"t_pwr","E6"}},
        {"Проверка GSM. ",{"t_gsm","E18,E35"}},
        {"Проверка GPS. ",{"t_gps","E19"}},
        {"Проверка ACC. ",{"t_acc","E20"}},
        {"Проверка BLE. ",{"t_ble","E22,E32"}},
        {"Проверка MIC. ",{"t_ai_mic","E34"}},
        {"Проверка температуры. ",{"t_temp","E17"}},
        {"Проверка входов/выходов. ",{"t_io_temp","E8"}},
    };   

};
#endif // MAINWINDOW_H
