#include "studentstaffoption.h"
#include "ui_studentstaffoption.h"

#include "addstudent.h"
#include "addstaff.h"
#include "admincustomerlogin.h"

StudentStaffOption::StudentStaffOption(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StudentStaffOption)
{
    ui->setupUi(this);

    setWindowFlag(Qt::WindowContextHelpButtonHint,false);
    setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::CustomizeWindowHint);
}

//QString type;

//void StudentStaffOption::receive_addType(QString addType)
//{
//    type = addType;
//}

void StudentStaffOption::on_pushButton_student_clicked()
{
    this->hide();

    AddStudent student;
    student.setModal(true);
    student.exec();
}

void StudentStaffOption::on_pushButton_staff_clicked()
{
    this->hide();

    AddStaff staff;
    staff.setModal(true);
    staff.exec();

}

StudentStaffOption::~StudentStaffOption()
{
    delete ui;
}
