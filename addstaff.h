#ifndef STAFFCUSTOMEROPTION_H
#define STAFFCUSTOMEROPTION_H

#include <QDialog>
#include "mainwindow.h"

namespace Ui {
class AddStaff;
}

class AddStaff : public QDialog
{
    Q_OBJECT

public:
    explicit AddStaff(QWidget *parent = nullptr);
    ~AddStaff();

private slots:
    void on_pushButton_add_staff_clicked();

    void on_pushButton_cancel_clicked();

private:
    Ui::AddStaff *ui;
};

#endif // STAFFCUSTOMEROPTION_H
