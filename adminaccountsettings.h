#ifndef ADMINACCOUNTSETTINGS_H
#define ADMINACCOUNTSETTINGS_H

#include <QDialog>

namespace Ui {
class AdminAccountSettings;
}

class AdminAccountSettings : public QDialog
{
    Q_OBJECT

public:
    explicit AdminAccountSettings(QWidget *parent = nullptr);
    ~AdminAccountSettings();

private slots:
    void on_pushButton_2_clicked();

private:
    Ui::AdminAccountSettings *ui;
};

#endif // ADMINACCOUNTSETTINGS_H
