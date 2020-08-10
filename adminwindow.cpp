#include "adminwindow.h"
#include "ui_adminwindow.h"

#include "addcustomeroption.h"
#include "showcustomerdataoption.h"
#include "admintodaysspecial.h"

#include <QDebug>

AdminWindow::AdminWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AdminWindow)
{
    ui->setupUi(this);

}

void AdminWindow::receive(QString username, QString password)
{

    MainWindow connect_database;

    if (!connect_database.sqlOpen())
    {
        qDebug() << "Failed to open the database";
        return;
    }

    connect_database.sqlOpen();

    QSqlQuery qry;

    qry.prepare("SELECT * FROM Admin WHERE Username = '"+username+"' AND Password = '"+password+"'");

    if (qry.exec())
    {
        while(qry.next())
        {
            ui->label_username->setText(username);
            ui->label_showName->setText(qry.value(1).toString());
        }
        connect_database.sqlClose();
    }
}

AdminWindow::~AdminWindow()
{
    delete ui;
}

void AdminWindow::on_pushButton_todays_special_clicked()
{
    AdminTodaysSpecial todayspecial;
    todayspecial.setModal(true);
    todayspecial.exec();
}

void AdminWindow::on_pushButton_add_customer_clicked()
{
    AddCustomerOption customerOption;
    customerOption.setModal(true);
    customerOption.exec();
}


void AdminWindow::on_pushButton_showcustomerdata_clicked()
{
//    QString addoption()
//    {
//        return "add_Student";
//    }
    ShowCustomerDataOption showDataOption;
    showDataOption.setModal(true);
    showDataOption.exec();
}

void AdminWindow::on_pushButton_addBalance_clicked()
{

}

void AdminWindow::on_pushButton_logout_clicked()
{
    this->hide();
    parentWidget()->show();
}
