#include "addbalance.h"
#include "ui_addbalance.h"
#include "mainwindow.h"

#include <QMessageBox>

AddBalance::AddBalance(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddBalance)
{
    ui->setupUi(this);

    setWindowFlag(Qt::WindowContextHelpButtonHint,false);
}

void AddBalance::receive_details(QString customerType, QString username, QString password)
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

    QSqlQuery qry;

    //connect_database.sqlOpen();

    if (Type == "student")
    {
        qry.prepare("SELECT * FROM Student WHERE Username = '"+Username+"' AND Password = '"+Password+"'");

        if (qry.exec())
        {
            int studentID;

            while(qry.next())
            {
                ui->label_showName->setText(qry.value(1).toString());

                studentID = qry.value(0).toInt();
                ID = studentID;
            }

            qry.prepare("SELECT * FROM Student_Balance WHERE Student_ID = (:StudentID)");
            qry.bindValue(":StudentID", studentID);

            if (qry.exec())
            {
                while(qry.next())
                {
                    currentBalance = qry.value(2).toInt();
                    ui->label_showCurrentBalance->setText(QString::number(currentBalance));
                }
            }
        }
    }

    if (Type == "staff")
    {
        qry.prepare("SELECT * FROM Staff WHERE Username = '"+Username+"' AND Password = '"+Password+"'");

        if (qry.exec())
        {
            int staffID;
            while(qry.next())
            {
                ui->label_showName->setText(qry.value(1).toString());

                staffID = qry.value(0).toInt();
                ID = staffID;
            }

            qry.prepare("SELECT * FROM Staff_Balance WHERE Staff_ID = (:StaffID)");
            qry.bindValue(":StaffID", staffID);

            if (qry.exec())
            {
                while(qry.next())
                {
                    currentBalance = qry.value(2).toInt();
                    ui->label_showCurrentBalance->setText(qry.value(2).toString());
                }
            }
        }
    }
    connect_database.sqlClose();
}

void AddBalance::on_pushButton_addBalance_clicked()
{
    int amount, newBalance;
    amount = ui->lineEdit_addAmount->text().toInt();

    newBalance = amount + currentBalance;
    currentBalance = newBalance;
    qDebug() << "New Amount: " << newBalance;

    MainWindow connect_database;

    if (!connect_database.sqlOpen())
    {
        qDebug() << "Failed to open the database";
        return;
    }

    QSqlQuery qry;
    //connect_database.sqlOpen();

    qDebug() << "Type: " << Type;
    qDebug() << "ID: " << ID;

    if (Type == "student")
    {
        qry.prepare("UPDATE Student_Balance SET Balance = (:balance) WHERE Student_ID = (:ID)");
        qry.bindValue(":balance", newBalance);
        qry.bindValue(":ID", ID);

        if (qry.exec())
        {
                ui->label_showCurrentBalance->setText(QString::number(currentBalance));
                QMessageBox::information(this, "Success", "Amount has been added");
                ui->lineEdit_addAmount->setText("");
        }
        else
        {
            QMessageBox::critical(this, tr("Error::"), qry.lastError().text());
        }
    }

    if (Type == "staff")
    {
        qry.prepare("UPDATE Staff_Balance SET Balance = (:balance) WHERE Staff_ID = (:ID)");
        qry.bindValue(":balance", newBalance);
        qry.bindValue(":ID", ID);

        if (qry.exec())
        {
                ui->label_showCurrentBalance->setText(QString::number(currentBalance));
                QMessageBox::information(this, "Success", "Amount has been added");
                ui->lineEdit_addAmount->setText("");
        }
        else
        {
            QMessageBox::critical(this, tr("Error::"), qry.lastError().text());
        }
    }

}

void AddBalance::on_pushButton_cancel_clicked()
{
    this->hide();
}

AddBalance::~AddBalance()
{
    delete ui;
}

