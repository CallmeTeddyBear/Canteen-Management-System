#ifndef SHOWCUSTOMERDATAOPTION_H
#define SHOWCUSTOMERDATAOPTION_H

#include <QDialog>

#include "mainwindow.h"

namespace Ui {
class ShowCustomerDataOption;
}

class ShowCustomerDataOption : public QDialog
{
    Q_OBJECT

public:
    explicit ShowCustomerDataOption(QWidget *parent = nullptr);
    ~ShowCustomerDataOption();

private slots:
    void on_pushButton_close_clicked();

private:
    Ui::ShowCustomerDataOption *ui;
};

#endif // SHOWCUSTOMERDATAOPTION_H
