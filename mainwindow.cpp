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

    ui->lineEdit_username->setPlaceholderText(" Enter Username");
    ui->lineEdit_password->setPlaceholderText(" Enter Password");

    if(!sqlOpen())
    {
        QMessageBox::warning(this, "ERROR", "Failed to open the database");
    }
 }

void MainWindow::on_pushButton_login_clicked()
{
    QString UserType;
    QString username = ui->lineEdit_username->text(); //variable declaration username and password
    QString password = ui->lineEdit_password->text(); //assigning text from user to variables

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
            sqlClose(); //closes the database - it is needed to open new window
            this->hide();

            UserType = "student";
            customerwindow = new CustomerWindow(this);
            connect(this, SIGNAL(send(const QString, const QString, const QString)), customerwindow, SLOT(receive(const QString, const QString, const QString)));
            emit send(UserType, username, password);
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
                    this->hide();

                    UserType = "staff";
                    customerwindow = new CustomerWindow(this);
                    connect(this, SIGNAL(send(const QString, const QString, const QString)), customerwindow, SLOT(receive(const QString, const QString, const QString)));
                    emit send(UserType, username, password);
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
                            sqlClose(); //closes the database - it is needed to open new window
                            this->hide();

                            UserType = "admin";
                            adminwindow = new AdminWindow(this);//this means this main window or this class which acts as a parent
                            connect(this, SIGNAL(send(const QString, const QString)), adminwindow, SLOT(receive(const QString, const QString)));
                            emit send(UserType, username, password);
                            adminwindow->show();

                        }
                        else
                        {
                        QMessageBox::warning(this, "Login", "Username and Password is not correct");
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


