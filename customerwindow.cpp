#include "customerwindow.h"
#include "ui_customerwindow.h"

#include "mainwindow.h"

CustomerWindow::CustomerWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CustomerWindow)
{
    ui->setupUi(this);    
}

void CustomerWindow::receive(QString username, QString password)
{

    MainWindow connect_database;

    if (!connect_database.sqlOpen())
    {
        qDebug() << "Failed to open the database";
        return;
    }

    connect_database.sqlOpen();

    QSqlQuery qry;

    qry.prepare("SELECT * FROM Student WHERE Username = '"+username+"' AND Password = '"+password+"'");

    if (qry.exec())
    {
        qint8 studentID;
        while(qry.next())
        {
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
    connect_database.sqlClose();
}

CustomerWindow::~CustomerWindow()
{
    delete ui;
}

void CustomerWindow::on_pushButton_logout_clicked()
{
    this->hide();
    parentWidget()->show();
}
