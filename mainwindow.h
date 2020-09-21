#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql>
#include <QtDebug>
#include <QFileInfo>
#include "customerwindow.h"
#include "adminwindow.h"

#include <QDebug>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public:
    QSqlDatabase mydb;

    void sqlClose()
    {
        mydb.close();
        mydb.removeDatabase(QSqlDatabase::defaultConnection);
    }

    bool sqlOpen()
    {
        mydb = QSqlDatabase::QSqlDatabase::addDatabase("QSQLITE");
        mydb.setDatabaseName("C:/Users/User/Desktop/CanteenManagementSystem/canteen_database.db");
//        QString dbPath = QCoreApplication::applicationDirPath() + "/canteen_database.db";
//        mydb.setDatabaseName(dbPath);

        if (!mydb.open())
        {
            qDebug() << "Failed to open the database";
            return false;
        }
        else
        {
            qDebug() << "Database Connected...";
            return true;
        }
    }

signals:
    void send_customer(const QString UserType, const QString username, const QString password);
    void send_admin(const QString username, const QString password);

private slots:
    void on_pushButton_login_clicked();

private:
    Ui::MainWindow *ui;
    AdminWindow *adminwindow;

public:
    CustomerWindow *customerwindow;
};
#endif // MAINWINDOW_H
