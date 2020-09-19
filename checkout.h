#ifndef CHECKOUT_H
#define CHECKOUT_H

#include <QDialog>

namespace Ui {
class Checkout;
}

class Checkout : public QDialog
{
    Q_OBJECT

public:
    explicit Checkout(QWidget *parent = nullptr);
    ~Checkout();

private slots:
    void on_pushButton_cancel_clicked();

    void on_pushButton_checkout_clicked();

public:

    void receive_items(QString items[12][3], int table_row, QString Usertype, int userbalance, int ID);
    void showTotal(int);


private:
    Ui::Checkout *ui;
    int total_price;
    QString UserType;
    int UserBalance;
    int UserID;
};

#endif // CHECKOUT_H
