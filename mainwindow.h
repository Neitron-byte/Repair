#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QDebug>
#include <QMessageBox>

#define CR "\r"


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

private:
    void openPort();
    void writeSerial(const QString);
    void readSerial();

    Ui::MainWindow *ui;
    QSerialPort* m_serial;

};
#endif // MAINWINDOW_H
