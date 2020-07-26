#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QPixmap>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QPixmap pix("C:/Users/User/Documents/Draft/KU_logo.png");
    ui->label_ku_logo->setPixmap(pix.scaled(200,200));

    QSqlDatabase mydb = QSqlDatabase::QSqlDatabase::addDatabase("QSQLITE");
    mydb.setDatabaseName("")
 }

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_login_clicked()
{
    QString username = ui->lineEdit_username->text(); //variable declaration username and password
    QString password = ui->lineEdit_password->text(); //assigning text from user to variables

    if (username == "test" && password == "test")
    {
        hide();
        secondWindow = new SecondWindow(this);
        secondWindow->show();
    }
    else
    {
        QMessageBox::warning(this, "Login", "Username and Password is not correct");
    }
}
