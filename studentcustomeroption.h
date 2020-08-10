#ifndef STUDENTCUSTOMEROPTION_H
#define STUDENTCUSTOMEROPTION_H

#include <QDialog>
#include "mainwindow.h"

namespace Ui {
class StudentCustomerOption;
}

class StudentCustomerOption : public QDialog
{
    Q_OBJECT

public:
    explicit StudentCustomerOption(QWidget *parent = nullptr);
    ~StudentCustomerOption();

private slots:
    void on_pushButton_add_student_clicked();

    void on_pushButton_cancel_clicked();

private:
    Ui::StudentCustomerOption *ui;
};

#endif // STUDENTCUSTOMEROPTION_H
