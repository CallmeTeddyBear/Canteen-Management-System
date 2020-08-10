#ifndef ADDCUSTOMEROPTION_H
#define ADDCUSTOMEROPTION_H

#include <QDialog>

namespace Ui {
class AddCustomerOption;
}

class AddCustomerOption : public QDialog
{
    Q_OBJECT

public:
    explicit AddCustomerOption(QWidget *parent = nullptr);
    ~AddCustomerOption();

private slots:
    void on_pushButton_student_clicked();

    void on_pushButton_staff_clicked();

private:
    Ui::AddCustomerOption *ui;
};

#endif // ADDCUSTOMEROPTION_H
