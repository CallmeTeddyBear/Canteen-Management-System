#include "customeraccountsettings.h"
#include "ui_customeraccountsettings.h"

CustomerAccountSettings::CustomerAccountSettings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CustomerAccountSettings)
{
    ui->setupUi(this);
    setWindowFlag(Qt::WindowContextHelpButtonHint,false);
}

void CustomerAccountSettings::on_pushButton_cancel_clicked()
{
    this->hide();
}

CustomerAccountSettings::~CustomerAccountSettings()
{
    delete ui;
}

