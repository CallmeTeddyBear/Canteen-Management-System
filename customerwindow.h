#ifndef CUSTOMERWINDOW_H
#define CUSTOMERWINDOW_H

#include <QMainWindow>


namespace Ui {
class CustomerWindow;
}

class CustomerWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit CustomerWindow(QWidget *parent = nullptr);
    ~CustomerWindow();

signals:
    void send(const QString Username, const QString Usertype);

public slots:
    void receive_customer(QString UserType, QString username, QString password);

private slots:
    void on_pushButton_logout_clicked();

    void on_pushButton_dinner_clicked();

    void on_pushButton_lunch_clicked();

    void on_pushButton_drinks_clicked();

    void on_pushButton_breakfast_clicked();

    void on_pushButton_settings_clicked();

    void showTodaysSpecial_gif();

    void showTodaysSpecial();

    void showDiscountoffer();

    void showList();

    void showBreakfast();

    void on_pushButton_momo_clicked();

    void on_pushButton_toast_clicked();

    void on_pushButton_riceset_clicked();

    void on_pushButton_discardAll_clicked();

    void on_pushButton_blackCoffee_clicked();

    void on_pushButton_checkout_clicked();

private:
    Ui::CustomerWindow *ui;
    QString Username;
    QString Usertype;
    int table_row;
};

#endif // CUSTOMERWINDOW_H
