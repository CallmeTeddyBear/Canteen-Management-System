#include "customeraccountsettings.h"
#include "ui_customeraccountsettings.h"

#include "mainwindow.h"
#include <QMessageBox>

CustomerAccountSettings::CustomerAccountSettings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CustomerAccountSettings)
{
    ui->setupUi(this);
    setWindowFlag(Qt::WindowContextHelpButtonHint,false);

    ui->stackedWidget->setCurrentIndex(0);
}

void CustomerAccountSettings::receive(QString Username, QString Usertype)
{
    ui->lineEdit_username->setText(Username);
    username = Username;
    usertype = Usertype;

    showUserPicture();
}

void CustomerAccountSettings::showUserPicture()
{
    MainWindow connect_database;

    if (!connect_database.sqlOpen())
    {
        qDebug() << "Failed to open the database";
        return;
    }

    connect_database.sqlOpen();

    QSqlQuery qry;

    if (usertype == "student")
    {
        qry.prepare("SELECT * FROM Student WHERE Username = '"+username+"'" );

        qry.exec();
        qry.first();
        QByteArray outByteArray = qry.value(8).toByteArray();
        QPixmap outPixmap = QPixmap();
        outPixmap.loadFromData(outByteArray);
        ui->label_userPicture->setPixmap(outPixmap.scaled(120,120));

        connect_database.close();
    }
    if (usertype == "staff")
    {
        qry.prepare("SELECT * FROM Staff WHERE Username = '"+username+"'" );

        qry.exec();
        qry.first();
        QByteArray outByteArray = qry.value(8).toByteArray();
        QPixmap outPixmap = QPixmap();
        outPixmap.loadFromData(outByteArray);
        ui->label_userPicture->setPixmap(outPixmap.scaled(120,120));

        connect_database.close();
    }
}

void CustomerAccountSettings::on_pushButton_changeUsername_clicked()
{
    QString newUsername = ui->lineEdit_username->text();

    MainWindow connect_database;

    if (!connect_database.sqlOpen())
    {
        qDebug() << "Failed to open the database";
        return;
    }

    connect_database.sqlOpen();

    QSqlQuery qry;

    if(usertype == "student")
    {
        qry.prepare("SELECT * FROM Student WHERE Username = '"+username+"'" );

        if(qry.exec())
        {
            int count = 0;
            while(qry.next())
            {
                count++;
            }
            if (count == 1)
            {
                if(newUsername != "")
                {
                    qry.prepare("UPDATE Student SET Username = '"+newUsername+"' WHERE Username = '"+username+"'");
                    if(qry.exec())
                    {
                        QMessageBox::information(this, "Success", "Username is changed");
                        this->hide();
                    }
                    else
                    {
                        QMessageBox::critical(this, tr("Error::"), qry.lastError().text());
                    }
                }
                else
                {
                    QMessageBox::warning(this, "", "Invalid Username");
                }
            }
        }
        connect_database.sqlClose();
    }
}

void CustomerAccountSettings::on_pushButton_changePassword_clicked()
{
    QString currentpassword, newPassword, newPassword2;

    currentpassword = ui->lineEdit_currentPassword->text();
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

    if(usertype == "student")
    {
        qry.prepare("SELECT * FROM Student WHERE Password = '"+currentpassword+"' AND Username = '"+username+"'" );

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
                        qry.prepare("UPDATE Student SET Password = '"+newPassword+"' WHERE Password = '"+currentpassword+"' AND Username = '"+username+"'");
                        if(qry.exec())
                        {
                            QMessageBox::information(this, "Success", "Password is changed");
                            ui->lineEdit_currentPassword->setText("");
                            ui->lineEdit_newPassword->setText("");
                            ui->lineEdit_newPassword2->setText("");
                            this->hide();
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
    if(usertype == "staff")
    {
        qry.prepare("SELECT * FROM Staff WHERE Password = '"+currentpassword+"' AND Username = '"+username+"'" );

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
                        qry.prepare("UPDATE Staff SET Password = '"+newPassword+"' WHERE Password = '"+currentpassword+"' AND Username = '"+username+"'");
                        if(qry.exec())
                        {
                            QMessageBox::information(this, "Success", "Password is changed");
                            ui->lineEdit_currentPassword->setText("");
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
}

void CustomerAccountSettings::on_pushButton_profile_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);

    showUserPicture();

}

void CustomerAccountSettings::on_pushButton_password_clicked()
{
     ui->stackedWidget->setCurrentIndex(1);
}

void CustomerAccountSettings::on_pushButton_cancel_clicked()
{
    this->hide();
}

CustomerAccountSettings::~CustomerAccountSettings()
{
    delete ui;
}
