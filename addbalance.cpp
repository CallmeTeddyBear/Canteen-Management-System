#include "addbalance.h"
#include "ui_addbalance.h"

#include "studentstaffoption.h"

AddBalance::AddBalance(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddBalance)
{
    ui->setupUi(this);


}

AddBalance::~AddBalance()
{
    delete ui;
}
