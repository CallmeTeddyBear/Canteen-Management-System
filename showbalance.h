#ifndef SHOWBALANCE_H
#define SHOWBALANCE_H

#include <QDialog>

namespace Ui {
class ShowBalance;
}

class ShowBalance : public QDialog
{
    Q_OBJECT

public:
    explicit ShowBalance(QWidget *parent = nullptr);
    ~ShowBalance();

private slots:
    void on_pushButton_close_clicked();

private:
    Ui::ShowBalance *ui;
};

#endif // SHOWBALANCE_H
