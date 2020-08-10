#ifndef STAFFCUSTOMEROPTION_H
#define STAFFCUSTOMEROPTION_H

#include <QDialog>
#include "mainwindow.h"

namespace Ui {
class StaffCustomerOption;
}

class StaffCustomerOption : public QDialog
{
    Q_OBJECT

public:
    explicit StaffCustomerOption(QWidget *parent = nullptr);
    ~StaffCustomerOption();

private slots:
    void on_pushButton_add_staff_clicked();

    void on_pushButton_cancel_clicked();

private:
    Ui::StaffCustomerOption *ui;
};

#endif // STAFFCUSTOMEROPTION_H
