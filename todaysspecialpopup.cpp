#include "todaysspecialpopup.h"
#include "ui_todaysspecialpopup.h"

#include "mainwindow.h"

TodaysSpecialPopUp::TodaysSpecialPopUp(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TodaysSpecialPopUp)
{
    ui->setupUi(this);

    setWindowFlag(Qt::WindowContextHelpButtonHint,false);
}

void TodaysSpecialPopUp::receive(QString popup)
{
    ui->label_showtodaysspecial->setText(popup);
}

TodaysSpecialPopUp::~TodaysSpecialPopUp()
{
    delete ui;
}
