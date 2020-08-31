#include "deletecustomer.h"
#include "ui_deletecustomer.h"

#include "mainwindow.h"
#include <QMessageBox>

DeleteCustomer::DeleteCustomer(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DeleteCustomer)
{
    ui->setupUi(this);

    setWindowFlag(Qt::WindowContextHelpButtonHint,false);
}

QString Type, Username, Password;

void DeleteCustomer::receive_details(QString customerType, QString username, QString password)
{
    Type = customerType;
    Username = username;
    Password = password;

    MainWindow connect_database;

    if (!connect_database.sqlOpen())
    {
        qDebug() << "Failed to open the database";
        return;
    }

    connect_database.sqlOpen();

    QSqlQuery qry;

    if (Type == "student")
    {
        qry.prepare("SELECT * FROM Student WHERE Username = '"+Username+"' AND Password = '"+Password+"'");

        if (qry.exec())
        {
            while(qry.next())
            {
                ui->label_showUsername->setText(Username);
                ui->label_showName->setText(qry.value(1).toString());
                ui->label_showCustomerType->setText("Student");

                StudentID = qry.value(0).toInt();
            }
        }
    }

    if (customerType == "staff")
    {
        qry.prepare("SELECT * FROM Staff WHERE Username = '"+Username+"' AND Password = '"+Password+"'");

        if (qry.exec())
        {
            while(qry.next())
            {
                ui->label_showUsername->setText(Username);
                ui->label_showName->setText(qry.value(1).toString());
                ui->label_showCustomerType->setText("Staff");

                StaffID = qry.value(0).toInt();
            }
        }
    }
    connect_database.sqlClose();
}

void DeleteCustomer::on_pushButton_deleteCustomer_clicked()
{
    MainWindow connect_database;

    if (!connect_database.sqlOpen())
    {
        qDebug() << "Failed to open the database";
        return;
    }

    connect_database.sqlOpen();

    QSqlQuery qry;

    if (Type == "student")
    {
        qry.prepare("DELETE FROM Student WHERE Username = '"+Username+"' AND Password = '"+Password+"'");

        if (qry.exec())
        {
            QMessageBox::information(this, tr("Success"), tr("Customer was deleted"));
            this->hide();

            qry.prepare("DELETE FROM Student_Balance WHERE Student_ID = (:studentid)");
            qry.bindValue(":studentid", StudentID);
            qry.exec();

        }
        else
        {
            QMessageBox::critical(this, tr("Error::"), qry.lastError().text());
        }
        connect_database.sqlClose();
    }
    if (Type == "staff")
    {
        qry.prepare("DELETE FROM Staff WHERE Username = '"+Username+"' AND Password = '"+Password+"'");

        if (qry.exec())
        {
            QMessageBox::information(this, tr("Success"), tr("Customer was deleted"));
            this->hide();

            qry.prepare("DELETE FROM Staff_Balance WHERE Staff_ID = (:staffid)");
            qry.bindValue(":staffid", StaffID);
            qry.exec();
        }
        else
        {
            QMessageBox::critical(this, tr("Error::"), qry.lastError().text());
        }
    }
    connect_database.sqlClose();
}

void DeleteCustomer::on_pushButton_cancel_clicked()
{
    this->hide();
}

DeleteCustomer::~DeleteCustomer()
{
    delete ui;
}


