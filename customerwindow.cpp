#include "customerwindow.h"
#include "ui_customerwindow.h"

#include "mainwindow.h"

CustomerWindow::CustomerWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CustomerWindow)
{
    ui->setupUi(this);    
}

void CustomerWindow::receive(QString UserType, QString username, QString password)
{

    MainWindow connect_database;

    if (!connect_database.sqlOpen())
    {
        qDebug() << "Failed to open the database";
        return;
    }

    connect_database.sqlOpen();

    QSqlQuery qry;

    if (UserType == "student")
    {
        qry.prepare("SELECT * FROM Student WHERE Username = '"+username+"' AND Password = '"+password+"'");

        if (qry.exec())
        {
            qint8 studentID;

            while(qry.next())
            {
                //count++;
                ui->label_showUsername->setText(username);
                ui->label_showName->setText(qry.value(1).toString());

                studentID = qry.value(0).toInt();
            }

            qry.prepare("SELECT * FROM Student_Balance WHERE Student_ID = (:StudentID)");
            qry.bindValue(":StudentID", studentID);

            if (qry.exec())
            {
                while(qry.next())
                {
                    ui->label_showBalance->setText(qry.value(2).toString());
                }
            }
        }
    }

    if(UserType == "staff")
    {
        qry.prepare("SELECT * FROM Staff WHERE Username = '"+username+"' AND Password = '"+password+"'");
        if (qry.exec())
        {
                    int staffID;
                    while(qry.next())
                    {
                        ui->label_showUsername->setText(username);
                        ui->label_showName->setText(qry.value(1).toString());

                        staffID = qry.value(0).toInt();
                    }

                    qry.prepare("SELECT * FROM Staff_Balance WHERE Staff_ID = (:Staff_ID)");
                    qry.bindValue(":Staff_ID", staffID);

                    if (qry.exec())
                    {
                        qDebug() << "StaffID = " << staffID;
                        while(qry.next())
                        {
                            ui->label_showBalance->setText(qry.value(2).toString());
                        }
                    }
        }
    }
    connect_database.sqlClose();
}

void CustomerWindow::on_pushButton_logout_clicked()
{
    this->hide();
    parentWidget()->show();
}

CustomerWindow::~CustomerWindow()
{
    delete ui;
}
