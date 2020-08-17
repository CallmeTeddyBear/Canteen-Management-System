#ifndef SHOWFOODMENU_H
#define SHOWFOODMENU_H

#include <QDialog>

namespace Ui {
class ShowFoodMenu;
}

class ShowFoodMenu : public QDialog
{
    Q_OBJECT

public:
    explicit ShowFoodMenu(QWidget *parent = nullptr);
    ~ShowFoodMenu();

private slots:
    void on_pushButton_close_clicked();

    void on_pushButton_addFood_clicked();

    void on_pushButton_addItem_clicked();

    void on_pushButton_home_clicked();

    void on_pushButton_deleteFood_clicked();

    void on_pushButton_delete_clicked();

private:
    Ui::ShowFoodMenu *ui;
};

#endif // SHOWFOODMENU_H
