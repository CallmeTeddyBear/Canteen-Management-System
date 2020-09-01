#ifndef CUSTOMERACCOUNTSETTINGS_H
#define CUSTOMERACCOUNTSETTINGS_H

#include <QDialog>

namespace Ui {
class CustomerAccountSettings;
}

class CustomerAccountSettings : public QDialog
{
    Q_OBJECT

public:
    explicit CustomerAccountSettings(QWidget *parent = nullptr);
    ~CustomerAccountSettings();

private slots:
    void showUserPicture();

    void on_pushButton_cancel_clicked();

    void on_pushButton_profile_clicked();

    void on_pushButton_password_clicked();

    void on_pushButton_changePassword_clicked();

    void on_pushButton_changeUsername_clicked();

public slots:
    void receive(QString Username, QString Usertype);

private:
    Ui::CustomerAccountSettings *ui;
    QString username, usertype;
};

#endif // CUSTOMERACCOUNTSETTINGS_H
