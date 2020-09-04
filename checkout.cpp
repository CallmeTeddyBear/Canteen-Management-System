#include "checkout.h"
#include "ui_checkout.h"

Checkout::Checkout(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Checkout)
{
    ui->setupUi(this);

    setWindowFlag(Qt::WindowContextHelpButtonHint,false);
}

Checkout::~Checkout()
{
    delete ui;
}

void Checkout::on_pushButton_cancel_clicked()
{
    this->hide();
}
