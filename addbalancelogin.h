#ifndef ADDBALANCE_H
#define ADDBALANCE_H

#include <QDialog>

namespace Ui {
class AddBalanceLogin;
}

class AddBalanceLogin : public QDialog
{
    Q_OBJECT

public:
    explicit AddBalanceLogin(QWidget *parent = nullptr);
    ~AddBalanceLogin();

signals:
    void send_details(const QString customerType,const QString username, const QString password);

private slots:
    void on_pushButton_ok_clicked();

    void on_pushButton_cancel_clicked();

private:
    Ui::AddBalanceLogin *ui;
};

#endif // ADDBALANCE_H
