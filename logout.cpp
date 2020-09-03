#include "logout.h"
#include "ui_logout.h"

#include "mainwindow.h"
#include "customerwindow.h"
//#include <QCloseEvent>

Logout::Logout(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Logout)
{
    ui->setupUi(this);

    setWindowFlag(Qt::WindowContextHelpButtonHint,false);
    setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::CustomizeWindowHint);
}

void Logout::on_pushButton_yes_clicked()
{
    this->hide();
//    mainwindow = new MainWindow();
//    mainwindow->show();
//    CustomerWindow destroy;
//    destroy.~CustomerWindow();
////    adminwindow->close();



}

void Logout::on_pushButton_2_no_clicked()
{
    this->hide();
}

Logout::~Logout()
{
    delete ui;
}

