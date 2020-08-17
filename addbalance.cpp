#include "addbalance.h"
#include "ui_addbalance.h"
#include "mainwindow.h"

AddBalance::AddBalance(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddBalance)
{
    ui->setupUi(this);

    setWindowFlag(Qt::WindowContextHelpButtonHint,false);
}

void AddBalance::receive_details(QString customerType, QString username, QString password)
{
    MainWindow connect_database;

    if (!connect_database.sqlOpen())
    {
        qDebug() << "Failed to open the database";
        return;
    }

    QSqlQuery qry;

    //connect_database.sqlOpen();

    if (customerType == "student")
    {
        qry.prepare("SELECT * FROM Student WHERE Username = '"+username+"' AND Password = '"+password+"'");

        if (qry.exec())
        {
            int studentID;

            while(qry.next())
            {
                ui->label_showName->setText(qry.value(1).toString());

                studentID = qry.value(0).toInt();
            }

            qry.prepare("SELECT * FROM Student_Balance WHERE Student_ID = (:StudentID)");
            qry.bindValue(":StudentID", studentID);

            if (qry.exec())
            {
                while(qry.next())
                {
                    ui->label_showCurrentBalance->setText(qry.value(2).toString());
                }
            }
        }
    }

    if (customerType == "staff")
    {
        qry.prepare("SELECT * FROM Staff WHERE Username = '"+username+"' AND Password = '"+password+"'");

        if (qry.exec())
        {
            int staffID;
            while(qry.next())
            {
                ui->label_showName->setText(qry.value(1).toString());

                staffID = qry.value(0).toInt();
            }

            qry.prepare("SELECT * FROM Staff_Balance WHERE Staff_ID = (:StaffID)");
            qry.bindValue(":StaffID", staffID);

            if (qry.exec())
            {
                while(qry.next())
                {
                    ui->label_showCurrentBalance->setText(qry.value(2).toString());
                }
            }
        }
    }
    connect_database.sqlClose();
}

void AddBalance::on_pushButton_addBalance_clicked()
{

}

void AddBalance::on_pushButton_cancel_clicked()
{
    this->hide();
}

AddBalance::~AddBalance()
{
    delete ui;
}

