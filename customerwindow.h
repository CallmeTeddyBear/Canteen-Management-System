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

    void on_pushButton_Sandwich_clicked();

    void on_pushButton_Toast_clicked();

    void on_pushButton_discardAll_clicked();

    void on_pushButton_blackCoffee_clicked();

    void on_pushButton_checkout_clicked();

    void showBreakfastThumbnails();

    void showLunchThumbnails();

    void showDinnerThumbnails();

    void showDrinksThumbnails();

    void on_pushButton_Muffin_clicked();

    void on_pushButton_Doughnut_clicked();

    void on_pushButton_Croissant_clicked();

    void on_pushButton_Cereal_clicked();

    void on_pushButton_Boiled_Egg_clicked();

    void on_pushButton_Jerry_clicked();

    void on_pushButton_Puri_Tarkari_clicked();

    void on_pushButton_Bacon_clicked();

    void on_pushButton_Oatmeal_clicked();

    void on_pushButton_Pancake_clicked();

    void on_pushButton_MoMo_clicked();

    void showOverflow();

    void on_pushButton_Chowmein_clicked();

    void on_pushButton_Chicken_Biryani_clicked();

    void on_pushButton_Mutton_Biryani_clicked();

    void on_pushButton_Red_Sauce_Pasta_clicked();

    void on_pushButton_White_Sauce_Pasta_clicked();

    void on_pushButton_Spaghetti_clicked();

    void on_pushButton_Pizza_clicked();

    void on_pushButton_Aloo_Chop_clicked();

    void on_pushButton_Naan_clicked();

    void on_pushButton_Chat_clicked();

    void on_pushButton_Sausage_clicked();

    void on_pushButton_Rice_Set_clicked();

    void on_pushButton_Roti_clicked();

    void on_pushButton_Chicken_Curry_clicked();

    void on_pushButton_Mutton_Curry_clicked();

    void on_pushButton_Egg_Curry_clicked();

    void on_pushButton_Chicken_Roast_clicked();

    void on_pushButton_Fish_Fry_clicked();

    void on_pushButton_Paneer_Chilli_clicked();

    void on_pushButton_Daal_Fry_clicked();

    void on_pushButton_Chana_Masala_clicked();

    void on_pushButton_Rajma_Masala_clicked();

    void on_pushButton_Papad_clicked();

private:
    Ui::CustomerWindow *ui;
    QString Username;
    QString Usertype;
    int table_row;

    int toastcount = 0;
    int sandwichcount = 0;
    int muffincount = 0;
    int doughnutcount = 0;
    int croissantcount = 0;
    int cerealcount = 0;
    int boiled_eggcount = 0;
    int jerrycount = 0;
    int puri_tarkaricount= 0;
    int baconcount = 0;
    int oatmealcount = 0;
    int pancakecount = 0;

    int momocount = 0;
    int chowmeincount = 0;
    int chicken_biryanicount = 0;
    int mutton_biryanicount = 0;
    int red_sauce_pastacount = 0;
    int white_sauce_pastacount = 0;
    int spaghetticount = 0;
    int pizzacount = 0;
    int aloo_chopcount = 0;
    int naancount = 0;
    int chatcount = 0;
    int sausagecount = 0;

    int rice_setcount = 0;
    int roticount = 0;
    int chicken_currycount = 0;
    int mutton_currycount = 0;
    int egg_currycount = 0;
    int chicken_roastcount = 0;
    int fish_frycount = 0;
    int paneer_chillicount = 0;
    int daal_frycount = 0;
    int chana_masalacount = 0;
    int rajma_masalacount = 0;
    int papadcount = 0;

    int black_teacount = 0;
    int black_coffeecount = 0;
    int milk_teacount = 0;
    int milk_coffeecount = 0;
    int pepsicount = 0;
    int fantacount = 0;
    int spritecount = 0;
    int orange_juicecount = 0;
    int redbullcount = 0;
    int hot_lemoncount = 0;
    int milkshakecount = 0;
    int hot_chocolatecount = 0;
};

#endif // CUSTOMERWINDOW_H
