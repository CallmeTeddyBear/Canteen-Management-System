#include "deletecustomer.h"
#include "ui_deletecustomer.h"

DeleteCustomer::DeleteCustomer(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DeleteCustomer)
{
    ui->setupUi(this);

    setWindowFlag(Qt::WindowContextHelpButtonHint,false);
}

DeleteCustomer::~DeleteCustomer()
{
    delete ui;
}
