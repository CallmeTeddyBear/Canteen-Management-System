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

private:
    Ui::DeleteCustomer *ui;
};

#endif // DELETECUSTOMER_H
