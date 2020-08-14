#ifndef LOGOUT_H
#define LOGOUT_H

#include <QDialog>

#include "mainwindow.h"
#include "adminwindow.h"

namespace Ui {
class Logout;
}

class Logout : public QDialog
{
    Q_OBJECT

public:
    explicit Logout(QWidget *parent = nullptr);
    ~Logout();

private slots:
    void on_pushButton_yes_clicked();

    void on_pushButton_2_no_clicked();

private:
    Ui::Logout *ui;
    MainWindow *mainwindow;
    AdminWindow *adminwindow;
};

#endif // LOGOUT_H
