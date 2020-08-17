#ifndef ADDBALANCE_H
#define ADDBALANCE_H

#include <QDialog>

namespace Ui {
class AdminCustomerLogin;
}

class AdminCustomerLogin : public QDialog
{
    Q_OBJECT

public:
    explicit AdminCustomerLogin(QWidget *parent = nullptr);
    ~AdminCustomerLogin();

signals:
    void send_details(const QString customerType,const QString username, const QString password);

private slots:
    void on_pushButton_ok_clicked();

    void on_pushButton_cancel_clicked();

public slots:
    void receive(QString data);

private:
    Ui::AdminCustomerLogin *ui;
};

#endif // ADDBALANCE_H
