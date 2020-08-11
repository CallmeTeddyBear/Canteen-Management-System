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

public slots:
    void receive(QString UserType, QString username, QString password);

private slots:
    void on_pushButton_logout_clicked();

private:
    Ui::CustomerWindow *ui;
};

#endif // CUSTOMERWINDOW_H
