#ifndef BALANCE_H
#define BALANCE_H

#include <QDialog>

namespace Ui {
class AddBalance;
}

class AddBalance : public QDialog
{
    Q_OBJECT

public:
    explicit AddBalance(QWidget *parent = nullptr);
    ~AddBalance();

private slots:
    void on_pushButton_cancel_clicked();

    void on_pushButton_addBalance_clicked();

public slots:
    void receive_details(QString customerType, QString username, QString password);

private:
    Ui::AddBalance *ui;
};

#endif // BALANCE_H
