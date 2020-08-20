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
    void showTime();

    void on_pushButton_todays_special_clicked();

    void on_pushButton_add_customer_clicked();

    void on_pushButton_showcustomerdata_clicked();

    void on_pushButton_addBalance_clicked();

    void on_pushButton_logout_clicked();

    void on_pushButton_showBalance_clicked();

    void on_pushButton_foodMenu_clicked();

    void on_pushButton_discountOffer_clicked();

    void on_pushButton_deleteCustomer_clicked();

    void on_pushButton_account_settings_clicked();

    void on_pushButton_side_clicked();

    void on_pushButton_side_close_clicked();

signals:
    void send(const QString data);

public slots:
    void receive_admin(QString username, QString password);

private:
    Ui::AdminWindow *ui;
    QTimer *timer_1s;
};

#endif // ADMINWINDOW_H
