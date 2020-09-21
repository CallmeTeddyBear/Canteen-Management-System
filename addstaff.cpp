#include "addstaff.h"
#include "ui_addstaff.h"

#include <QMessageBox>

AddStaff::AddStaff(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddStaff)
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

void AddStaff::on_pushButton_add_staff_clicked()
{
    MainWindow connect_database;

    QString name, department, gender, address, contact, username, raw_password, password;

    name = ui->lineEdit_name->text();
    department = ui->comboBox_department->currentText();
    address = ui->lineEdit_address->text();
    contact = ui->lineEdit_contact->text();
    username = ui->lineEdit_username->text();
    raw_password = ui->lineEdit_password->text();

    QByteArray BA_password = raw_password.toUtf8(); //Changing Raw input of password to byteArray
    password = QByteArray(QCryptographicHash::hash(BA_password, QCryptographicHash::Md5).toHex());

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
        ui->lineEdit_name->setText("");
        ui->comboBox_department->setCurrentIndex(0);
        ui->lineEdit_address->setText("");
        ui->lineEdit_contact->setText("");
        ui->lineEdit_username->setText("");
        ui->lineEdit_password->setText("");

        qry.prepare("INSERT INTO Staff_Balance(Name, Balance) VALUES(?, ?)");
        qry.addBindValue(name);
        qry.addBindValue(0); //sets balance to 0

        qry.exec();
    }
    else
    {
        QMessageBox::critical(this, tr("Error::"), qry.lastError().text());
    }

    connect_database.sqlClose();
}

void AddStaff::on_pushButton_cancel_clicked()
{
    this->hide();
}

AddStaff::~AddStaff()
{
    delete ui;
}
