#ifndef ADDCUSTOMEROPTION_H
#define ADDCUSTOMEROPTION_H

#include <QDialog>

namespace Ui {
class StudentStaffOption;
}

class StudentStaffOption : public QDialog
{
    Q_OBJECT

public:
    explicit StudentStaffOption(QWidget *parent = nullptr);
    ~StudentStaffOption();

private slots:
    void on_pushButton_student_clicked();
    void on_pushButton_staff_clicked();

public slots:
//    void receive_addType(QString addType);

private:
    Ui::StudentStaffOption *ui;
};

#endif // ADDCUSTOMEROPTION_H
