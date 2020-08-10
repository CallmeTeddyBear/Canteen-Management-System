#include "studentcustomeroption.h"
#include "ui_studentcustomeroption.h"
#include <QMessageBox>

StudentCustomerOption::StudentCustomerOption(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StudentCustomerOption)
{
    ui->setupUi(this);

    setWindowFlag(Qt::WindowContextHelpButtonHint,false);

    ui->comboBox_faculty->addItem("Architecture");
    ui->comboBox_faculty->addItem("Biochemistry");
    ui->comboBox_faculty->addItem("Civil Engineering");
    ui->comboBox_faculty->addItem("Computer Engineering");
    ui->comboBox_faculty->addItem("Computer Science");
    ui->comboBox_faculty->addItem("Electrical Engineering");
    ui->comboBox_faculty->addItem("Environmental Engineering");
    ui->comboBox_faculty->addItem("Environmental Science");
    ui->comboBox_faculty->addItem("Geomatics Engineering");
    ui->comboBox_faculty->addItem("Mechanical Engineering");
    ui->comboBox_faculty->addItem("Pharmacy");

}

void StudentCustomerOption::on_pushButton_add_student_clicked()
{
    MainWindow connect_database;

    QString name, faculty, gender, address, contact, username, password;

    name = ui->lineEdit_name->text();
    faculty = ui->comboBox_faculty->currentText();
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
    //qry.prepare("INSERT INTO Student(StudentID, Name, Faculty, Gender, Address, Contact_No, Username, Password) VALUES(studentid, '"+name+"', '"+faculty+"', '"+gender+"', '"+address+"', '"+contact+"', '"+username+"', '"+password+"')");

    qry.prepare("INSERT INTO Student(Name, Faculty, Gender, Address, Contact_No, Username, Password) VALUES(?, ?, ?, ?, ?, ?, ?)");
    qry.addBindValue(name);
    qry.addBindValue(faculty);
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

void StudentCustomerOption::on_pushButton_cancel_clicked()
{
    this->hide();
}

StudentCustomerOption::~StudentCustomerOption()
{
    delete ui;
}