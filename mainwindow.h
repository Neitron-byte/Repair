#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include <QMessageBox>
#include <QToolBox>
#include <QWidget>
#include <QStringList>
#include <QComboBox>
#include <QPlainTextEdit>
#include <QMap>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

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

private:

    void showStatusMessage(const QString &message);
    void showStatusTest(const QString &answer);
    void checkAnswer(const QByteArray &answer);

    void openPort();
    void writeSerial(const QString);
    void readSerial();


    Ui::MainWindow *ui;
    // COM порт
    QSerialPort* m_serial;
    //список доступных портов
    QStringList m_listCom;
    //статус подключения к COM
    QLabel *m_status = nullptr;
    //тайм аут для ожидания данных
    int m_waitTimeout = 0;


};
#endif // MAINWINDOW_H
