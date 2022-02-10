#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QLabel>
#include <QSqlTableModel>
#include <QTableView>
#include <QVector>
#include <QCheckBox>
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

    void on_comboBox_dev_currentIndexChanged(int index);

    void on_pushButton_t_mem_ON_clicked();

    void on_pushButton_t_mem_OFF_clicked();

    void on_pushButton_t_mem_stat_clicked();

    void on_pushButton_t_mem_id_clicked();

signals:

    void initSetTable (int );


private:

    void showStatusMessage(const QString &message);

    void initActionsConnections();

    void initWidgets();

    void addTables();

    void setProperty();

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
    //Список виджетов ошибок
    //QStringList m_errors_widget_name {"t_mem", "t_lin", "t_can", "t_pwr", "t_io_temp", "t_gsm", "t_gps", "t_acc", "t_ble", "t_ai_mic", "t_trx"};
    //для стартера
    QVector<QCheckBox*> m_vector_check_starter;
    //наименование изделий
    QStringList Model {" ", "ES96", "A97"};

    QVector<QPair<QString,QString>> m_test_errorES96 {
        {"t_mem","E1,E2,E15"},
        {"t_lin","E1,E3,E5,E36,E37,E38,E39,E31"},
        {"t_can","E1,E4,E5"},
        {"t_pwr","E6"},
        {"t_io_temp","E8,E17"},
        {"t_gsm","E18,E30,E35"},
        {"t_gps","E19"},
        {"t_acc","E20"},
        {"t_ble","E22,E32"},
        {"t_ai_mic","E34"},
        {"t_trx"," "}

    };

    QVector<QPair<QString,QString>> m_test_errorA97 {
        {"t_mem","E3,E5,E24"},
        {"t_lin","E3,E6,E7,E13,E17"},
        {"t_can","E3,E8,E9,E12"},
        {"t_pwr","E4"},
        {"t_io_temp","E2,E14,E15,E16,E18,E19,E20,E21,E22,E23,E26"},
        {"t_gsm","E25"},
        {"t_gps"," "},
        {"t_acc","E1"},
        {"t_ble","E27,E28,E29"},
        {"t_ai_mic"," "},
        {"t_trx","E10,E11"}

    };

};
#endif // MAINWINDOW_H
