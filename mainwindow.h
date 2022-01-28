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

private:

    void showStatusMessage(const QString &message);

    void initActionsConnections();

    void initWidgets();

    void addTables();

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
    QStringList m_errors_widget_name {"E6", "E34", "E18_35", "E8", "E3_37", "E4", "E2", "E19"};
    //для стартера
    QVector<QCheckBox*> m_vector_check_starter;

};
#endif // MAINWINDOW_H
