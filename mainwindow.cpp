#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QPixmap>

#include "customerwindow.h"
#include "adminwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QPixmap pix("C:/Users/User/Desktop/CanteenManagementSystem/KU_logo.png");
    ui->label_ku_logo->setPixmap(pix.scaled(200,200));

    ui->lineEdit_username->setPlaceholderText(" Username");
    ui->lineEdit_password->setPlaceholderText(" Password");

    if(!sqlOpen())
    {
        QMessageBox::critical(this, "ERROR", "Failed to open the database");
    }
 }

void MainWindow::on_pushButton_login_clicked()
{
    QString UserType;
    QString username = ui->lineEdit_username->text();
    QString raw_password = ui->lineEdit_password->text(); //assigning text from user to variables

    // PassWord Hash //
    QByteArray BA_password = raw_password.toUtf8(); //Changing Raw input of password to byteArray
    QString password = QByteArray(QCryptographicHash::hash(BA_password, QCryptographicHash::Md5).toHex()); //Converting to Hash

    sqlOpen();
    QSqlQuery qry;
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
            sqlClose();

            ui->lineEdit_username->setText("");
            ui->lineEdit_password->setText("");

            this->hide();

            UserType = "student";
            customerwindow = new CustomerWindow(this);
            connect(this, SIGNAL(send_customer(const QString, const QString, const QString)), customerwindow, SLOT(receive_customer(const QString, const QString, const QString)));
            emit send_customer(UserType, username, password);
            customerwindow->show();


        }
        else
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
                    sqlClose();

                    ui->lineEdit_username->setText("");
                    ui->lineEdit_password->setText("");
                    this->hide();

                    UserType = "staff";
                    customerwindow = new CustomerWindow(this);
                    connect(this, SIGNAL(send_customer(const QString, const QString, const QString)), customerwindow, SLOT(receive_customer(const QString, const QString, const QString)));
                    emit send_customer(UserType, username, password);
                    customerwindow->show();
                }
                else
                {
                    qry.prepare("SELECT * FROM Admin WHERE Username = '"+username+"' and Password = '"+password+"'");
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
                            sqlClose();

                            ui->lineEdit_username->setText("");
                            ui->lineEdit_password->setText("");
                            this->hide();

                            adminwindow = new AdminWindow(this);//this means this main window or this class which acts as a parent
                            connect(this, SIGNAL(send_admin(const QString, const QString)), adminwindow, SLOT(receive_admin(const QString, const QString)));
                            emit send_admin(username, password);
                            adminwindow->show();

                        }
                        else
                        {
                            QMessageBox::warning(this, "Login", "Username or Password is not correct");
                        }
                    }
                }
            }
        }
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}


