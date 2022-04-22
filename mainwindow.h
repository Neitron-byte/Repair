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
#include <QRadioButton>
#include <QVariant>



QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class Console;
class QSqlTableModel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

    void addCheckBoxInVector();
    void RadioButtonIOinInVector();
    void setProperty();
    QVariant CheckEnabledRadioButton();
    void controlIO(QVector<QCheckBox*>&);



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

    void on_pushButton_lin_split_OFF_exRelay_clicked();        

    void on_pushButton_lin_split_off_relay_clicked();    

    void on_pushButton_lin_split_ON_exRelay_clicked();

    void on_pushButton_lin_split_on_relay_clicked();



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

    void on_pushButton_start_gps_clicked();

    void on_pushButton_stop_gps_clicked();

    void on_pushButton_status_gps_clicked();

    void on_pushButton_status_acc_clicked();

    void on_pushButton_itemp_clicked();

public slots:
    void abouts();


private:

    void showStatusMessage(const QString &message);
    void initActionsConnections();
    void initWidgets();

    void initToolBox_db();

    Ui::MainWindow *ui;

    //консоль
    Console *m_console = nullptr;

    // COM порт
    QSerialPort* m_serial = nullptr;

    //список доступных портов
    QStringList m_listCom;

    //статус подключения к COM
    QLabel *m_status = nullptr;

    //перечень таблиц
    QStringList m_listTable;

    //Комментарии
    comments* m_comments = nullptr;

    //RadioButton IO Test
    QVector<QRadioButton*> m_vector_test_IO;
    //для стартера
    QVector<QCheckBox*> m_vector_check_starter;
    //для t_TRUNK
    QVector<QCheckBox*> m_vector_check_trunk;
    //для t_brake
    QVector<QCheckBox*> m_vector_check_brake;
    //для t_hoode
    QVector<QCheckBox*> m_vector_check_hood;

    //для t_hoode
    QVector<QCheckBox*> m_vector_check_IGN_IN;

    //для t_btn
    QVector<QCheckBox*> m_vector_check_t_btn;

    //для t_siren_rpm
    QVector<QCheckBox*> m_vector_check_siren_rpm;


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
