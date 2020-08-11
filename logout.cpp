#include "logout.h"
#include "ui_logout.h"

//#include "mainwindow.h"

Logout::Logout(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Logout)
{
    setWindowFlag(Qt::WindowContextHelpButtonHint,false);
    ui->setupUi(this);
}

Logout::~Logout()
{
    delete ui;
}

void Logout::on_pushButton_yes_clicked()
{
    this->hide();
//    adminwindow->close();
//    mainwindow = new MainWindow();
//    mainwindow->show();
}

void Logout::on_pushButton_2_no_clicked()
{
    this->hide();
}
