#ifndef ADMINWINDOW_H
#define ADMINWINDOW_H

#include <QMainWindow>

namespace Ui {
class AdminWindow;
}

class AdminWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit AdminWindow(QWidget *parent = nullptr);
    ~AdminWindow();

private slots:
    void on_pushButton_todays_special_clicked();

    void on_pushButton_add_customer_clicked();

    void on_pushButton_showcustomerdata_clicked();

    void on_pushButton_addBalance_clicked();

    void on_pushButton_logout_clicked();

public slots:
    void receive(QString username, QString password);

private:
    Ui::AdminWindow *ui;
    //MainWindow *mainwindow;
};

#endif // ADMINWINDOW_H
