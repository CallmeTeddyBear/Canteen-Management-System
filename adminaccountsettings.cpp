#include "adminaccountsettings.h"
#include "ui_adminaccountsettings.h"

#include "mainwindow.h"
#include <QMessageBox>

AdminAccountSettings::AdminAccountSettings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AdminAccountSettings)
{
    ui->setupUi(this);
    setWindowFlag(Qt::WindowContextHelpButtonHint,false);
}

void AdminAccountSettings::on_pushButton_changePassword_clicked()
{
    QString password, newPassword, newPassword2;

    password = ui->lineEdit_password->text();
    newPassword = ui->lineEdit_newPassword->text();
    newPassword2 = ui->lineEdit_newPassword2->text();

    MainWindow connect_database;

    if (!connect_database.sqlOpen())
    {
        qDebug() << "Failed to open the database";
        return;
    }

    connect_database.sqlOpen();

    QSqlQuery qry;

    qry.prepare("SELECT * FROM Admin WHERE Password = '"+password+"'");

    if(qry.exec())
    {
        int count = 0;
        while(qry.next())
        {
            count++;
        }
        if (count == 1)
        {
            if (newPassword == newPassword2)
            {
                if(newPassword != "")
                {
                    qry.prepare("UPDATE Admin SET Password = '"+newPassword+"'");
                    if(qry.exec())
                    {
                        QMessageBox::information(this, "Success", "Password is changed");
                        ui->lineEdit_password->setText("");
                        ui->lineEdit_newPassword->setText("");
                        ui->lineEdit_newPassword2->setText("");
                    }
                    else
                    {
                        QMessageBox::critical(this, tr("Error::"), qry.lastError().text());
                    }
                }
                else
                {
                    QMessageBox::warning(this, "", "Invalid Password");
                }
            }
            else
            {
                QMessageBox::warning(this, "", "New Password does not match");
            }
        }
        else
        {
            QMessageBox::warning(this, "", "Wrong Password");
        }
    }

    connect_database.sqlClose();
}

void AdminAccountSettings::on_pushButton_cancel_clicked()
{
    this->hide();
}

AdminAccountSettings::~AdminAccountSettings()
{
    delete ui;
}



