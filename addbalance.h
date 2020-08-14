#ifndef ADDBALANCE_H
#define ADDBALANCE_H

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

private:
    Ui::AddBalance *ui;
};

#endif // ADDBALANCE_H
