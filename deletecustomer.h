#ifndef DELETECUSTOMER_H
#define DELETECUSTOMER_H

#include <QDialog>

namespace Ui {
class DeleteCustomer;
}

class DeleteCustomer : public QDialog
{
    Q_OBJECT

public:
    explicit DeleteCustomer(QWidget *parent = nullptr);
    ~DeleteCustomer();

public slots:
    void receive_details(QString customerType, QString username, QString password);

private slots:
    void on_pushButton_cancel_clicked();

    void on_pushButton_deleteCustomer_clicked();

private:
    Ui::DeleteCustomer *ui;
    int StudentID;
    int StaffID;
};

#endif // DELETECUSTOMER_H
