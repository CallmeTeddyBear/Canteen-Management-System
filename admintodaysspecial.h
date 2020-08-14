#ifndef ADMINTODAYSSPECIAL_H
#define ADMINTODAYSSPECIAL_H

#include <QDialog>
#include "mainwindow.h"
#include "todaysspecialpopup.h"

namespace Ui {
class AdminTodaysSpecial;
}

class AdminTodaysSpecial : public QDialog
{
    Q_OBJECT

public:
    explicit AdminTodaysSpecial(QWidget *parent = nullptr);
    ~AdminTodaysSpecial();

signals:
    void send(const QString todays_special);

private slots:
    void on_pushButton_cancel_clicked();

    void on_pushButton_ok_clicked();

private:
    Ui::AdminTodaysSpecial *ui;
    TodaysSpecialPopUp *popup;
};

#endif // ADMINTODAYSSPECIAL_H
