#include "adminaccountsettings.h"
#include "ui_adminaccountsettings.h"

AdminAccountSettings::AdminAccountSettings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AdminAccountSettings)
{
    ui->setupUi(this);
    setWindowFlag(Qt::WindowContextHelpButtonHint,false);
}

AdminAccountSettings::~AdminAccountSettings()
{
    delete ui;
}

void AdminAccountSettings::on_pushButton_2_clicked()
{
    this->hide();
}
