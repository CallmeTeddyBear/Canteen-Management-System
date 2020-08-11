#ifndef STUDENTCUSTOMEROPTION_H
#define STUDENTCUSTOMEROPTION_H

#include <QDialog>
#include "mainwindow.h"

namespace Ui {
class AddStudent;
}

class AddStudent : public QDialog
{
    Q_OBJECT

public:
    explicit AddStudent(QWidget *parent = nullptr);
    ~AddStudent();

private slots:
    void on_pushButton_add_student_clicked();

    void on_pushButton_cancel_clicked();

private:
    Ui::AddStudent *ui;
};

#endif // STUDENTCUSTOMEROPTION_H
