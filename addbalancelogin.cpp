#include "addbalancelogin.h"
#include "ui_addbalancelogin.h"

#include "studentstaffoption.h"
#include "mainwindow.h"
#include "addbalance.h"

#include <QMessageBox>

AddBalanceLogin::AddBalanceLogin(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddBalanceLogin)
{
    ui->setupUi(this);

    setWindowFlag(Qt::WindowContextHelpButtonHint,false);
}

void AddBalanceLogin::on_pushButton_ok_clicked()
{
    QString username, password, customerType;

    username = ui->lineEdit_username->text();
    password = ui->lineEdit_password->text();

    if (ui->radioButton_student->isChecked())
    {
        customerType = "student";
    }
    if (ui->radioButton_staff->isChecked())
    {
        customerType = "staff";
    }

    MainWindow connect_database;

    if (!connect_database.sqlOpen())
    {
        qDebug() << "Failed to open the database";
        return;
    }

    connect_database.sqlOpen();

    QSqlQuery qry;

    if (customerType == "student")
    {
        qry.prepare("SELECT * FROM Student WHERE Username = '"+username+"' and Password = '"+password+"'");

        if (qry.exec())
        {
            int count;
            count = 0;
            while(qry.next())
            {
                count++;
            }
            if (count == 1)
            {
                connect_database.sqlClose();
                this->hide();

                AddBalance *balance = new AddBalance();
                balance->setModal(true);
                connect(this, SIGNAL(send_details(const QString, const QString, const QString)), balance, SLOT(receive_details(const QString, const QString, const QString)));
                emit send_details(customerType, username, password);
                balance->show();
            }
            else
            {
                QMessageBox::warning(this, "Login", "Username or Password is not correct");
            }
        }
    }
    if (customerType == "staff")
    {
        qry.prepare("SELECT * FROM Staff WHERE Username = '"+username+"' and Password = '"+password+"'");

        if (qry.exec())
        {
            int count;
            count = 0;
            while(qry.next())
            {
                count++;
            }
            if (count == 1)
            {
                connect_database.sqlClose();
                this->hide();

                AddBalance *balance = new AddBalance();
                balance->setModal(true);
                connect(this, SIGNAL(send_details(const QString, const QString, const QString)), balance, SLOT(receive_details(const QString, const QString, const QString)));
                emit send_details(customerType, username, password);
                balance->show();
            }
            else
            {
                QMessageBox::warning(this, "Login", "Username or Password is not correct");
            }
        }
    }
}

void AddBalanceLogin::on_pushButton_cancel_clicked()
{
    this->hide();
}

AddBalanceLogin::~AddBalanceLogin()
{
    delete ui;
}
