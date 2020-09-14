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
    QString raw_password, currentPassword, raw_newPassword, raw_newPassword2, newPassword;

    raw_password = ui->lineEdit_password->text();

    QByteArray BA_password = raw_password.toUtf8(); //Changing Raw input of password to byteArray
    currentPassword = QByteArray(QCryptographicHash::hash(BA_password, QCryptographicHash::Md5).toHex());

    raw_newPassword = ui->lineEdit_newPassword->text();
    raw_newPassword2 = ui->lineEdit_newPassword2->text();

    QByteArray BA_newPassword = raw_newPassword.toUtf8(); //Changing Raw input of password to byteArray
    newPassword = QByteArray(QCryptographicHash::hash(BA_newPassword, QCryptographicHash::Md5).toHex());

    MainWindow connect_database;

    if (!connect_database.sqlOpen())
    {
        qDebug() << "Failed to open the database";
        return;
    }

    connect_database.sqlOpen();

    QSqlQuery qry;

    qry.prepare("SELECT * FROM Admin WHERE Password = '"+currentPassword+"'");

    if(qry.exec())
    {
        int count = 0;
        while(qry.next())
        {
            count++;
        }
        if (count == 1)
        {
            if (raw_newPassword == raw_newPassword2)
            {
                if(raw_newPassword != "")
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



