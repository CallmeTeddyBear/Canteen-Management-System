#include "studentstaffoption.h"
#include "ui_studentstaffoption.h"

#include "addstudent.h"
#include "addstaff.h"

StudentStaffOption::StudentStaffOption(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StudentStaffOption)
{
    ui->setupUi(this);
    setWindowFlag(Qt::WindowContextHelpButtonHint,false);
}

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

