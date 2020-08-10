#include "staffcustomeroption.h"
#include "ui_staffcustomeroption.h"

#include <QMessageBox>

StaffCustomerOption::StaffCustomerOption(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StaffCustomerOption)
{
    ui->setupUi(this);
    setWindowFlag(Qt::WindowContextHelpButtonHint,false);

    ui->comboBox_department->addItem("Architecture");
    ui->comboBox_department->addItem("Biochemistry");
    ui->comboBox_department->addItem("Civil Engineering");
    ui->comboBox_department->addItem("Computer Engineering");
    ui->comboBox_department->addItem("Computer Science");
    ui->comboBox_department->addItem("Electrical Engineering");
    ui->comboBox_department->addItem("Environmental Engineering");
    ui->comboBox_department->addItem("Environmental Science");
    ui->comboBox_department->addItem("Geomatics Engineering");
    ui->comboBox_department->addItem("Mechanical Engineering");
    ui->comboBox_department->addItem("Pharmacy");

}

StaffCustomerOption::~StaffCustomerOption()
{
    delete ui;
}

void StaffCustomerOption::on_pushButton_add_staff_clicked()
{
    MainWindow connect_database;

    QString name, department, gender, address, contact, username, password;

    name = ui->lineEdit_name->text();
    department = ui->comboBox_department->currentText();
    address = ui->lineEdit_address->text();
    contact = ui->lineEdit_contact->text();
    username = ui->lineEdit_username->text();
    password = ui->lineEdit_password->text();

    if (ui->radioButton_male->isChecked())
    {
        gender = "Male";
    }
    if (ui->radioButton_female->isChecked())
    {
        gender = "Female";
    }

    if (!connect_database.sqlOpen())
    {
        qDebug() << "Failed to open the database";
        return;
    }

    connect_database.sqlOpen();
    QSqlQuery qry;

    qry.prepare("INSERT INTO Staff(Name, Department, Gender, Address, Contact_No, Username, Password) VALUES(?, ?, ?, ?, ?, ?, ?)");
    qry.addBindValue(name);
    qry.addBindValue(department);
    qry.addBindValue(gender);
    qry.addBindValue(address);
    qry.addBindValue(contact);
    qry.addBindValue(username);
    qry.addBindValue(password);


    if (qry.exec())
    {
        QMessageBox::information(this, tr("Success"), tr("Customer was added"));
        connect_database.sqlClose();
    }
    else
    {
        QMessageBox::critical(this, tr("Error::"), qry.lastError().text());
    }
}


void StaffCustomerOption::on_pushButton_cancel_clicked()
{
    this->hide();
}

