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
    void on_pushButton_cancel_clicked();

private:
    Ui::CustomerAccountSettings *ui;
};

#endif // CUSTOMERACCOUNTSETTINGS_H
