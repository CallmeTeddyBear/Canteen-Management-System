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

    void receive_items(QString items[12][3], int table_row);
    void showTotal(int);


private:
    Ui::Checkout *ui;
};

#endif // CHECKOUT_H
