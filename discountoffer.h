#ifndef DISCOUNTOFFER_H
#define DISCOUNTOFFER_H

#include <QDialog>

namespace Ui {
class DiscountOffer;
}

class DiscountOffer : public QDialog
{
    Q_OBJECT

public:
    explicit DiscountOffer(QWidget *parent = nullptr);
    ~DiscountOffer();

private slots:
    void on_pushButton_cancel_clicked();

    void on_pushButton_select_clicked();

    void on_pushButton_addDiscountoffer_clicked();

    void on_pushButton_removeDiscountoffer_clicked();

    void on_pushButton_addOffer_clicked();

private:
    Ui::DiscountOffer *ui;
    int foodID;
    QString foodTitle;
    int discountPrice;
    int actualPrice;
};

#endif // DISCOUNTOFFER_H
