#ifndef COMMENTS_H
#define COMMENTS_H

#include <QWidget>
#include <QSQLDatabase>
#include <QStringList>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QDebug>


namespace Ui {
class comments;
}

class comments : public QWidget
{
    Q_OBJECT
    void connectionDB();
    void createTables(const QString &);
    void updateTable();

public:
    explicit comments(QWidget *parent = nullptr);
    ~comments();
    QStringList &getTableList();
    void setCurrentTable(const QString &);

public slots:
    //void slotSetTable(const QString &);


private slots:
    void on_pushButton_add_comm_clicked();

private:
    Ui::comments *ui;

    QSqlDatabase m_db;
    QStringList m_listTable;
    QSqlTableModel* m_model;
    QString m_currentTable;


};

#endif // COMMENTS_H
