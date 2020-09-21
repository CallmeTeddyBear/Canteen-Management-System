#include "customerwindow.h"
#include "ui_customerwindow.h"

#include "mainwindow.h"
#include "todaysspecialpopup.h"
#include "customeraccountsettings.h"
#include "checkout.h"
#include "logout.h"
#include <QMessageBox>
#include <QTimer>
#include <QMovie>
#include <QSize>
#include <QPainter>

#include <QStandardItemModel>

CustomerWindow::CustomerWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CustomerWindow)
{
    ui->setupUi(this);

    setWindowState(Qt::WindowMaximized);

//    TodaysSpecialPopUp *popup = new TodaysSpecialPopUp();
//    popup->setModal(true);
//    QTimer::singleShot(1000, popup, SLOT(show()));

    ui->stackedWidget->setCurrentIndex(0);

    showBreakfastThumbnails();
    showLunchThumbnails();
    showDinnerThumbnails();
    showDrinksThumbnails();

    showTodaysSpecial_gif();
    showTodaysSpecial();
    showDiscountoffer();
    showList();

    total_price = 0;
    ui->label_total->hide();
    ui->label_showTotal->hide();
}

void CustomerWindow::receive_customer(QString UserType, QString username, QString password)
{
    Username = username;
    Usertype = UserType;

    MainWindow connect_database;

    if (!connect_database.sqlOpen())
    {
        qDebug() << "Failed to open the database";
        return;
    }

    connect_database.sqlOpen();

    QSqlQuery qry;

    if (UserType == "student")
    {
        qry.prepare("SELECT * FROM Student WHERE Username = '"+username+"' AND Password = '"+password+"'");

        if (qry.exec())
        {
            int studentID;

            while(qry.next())
            {
                ui->label_showUsername->setText(username);
                ui->label_showName->setText(qry.value(1).toString());

                QByteArray outByteArray = qry.value(8).toByteArray();
                QPixmap outPixmap = QPixmap();
                outPixmap.loadFromData(outByteArray);
                ui->label_userPicture->setPixmap(outPixmap.scaled(210,210));

                studentID = qry.value(0).toInt();
                ID = studentID;
            }

            qry.prepare("SELECT * FROM Student_Balance WHERE Student_ID = (:StudentID)");
            qry.bindValue(":StudentID", studentID);

            if (qry.exec())
            {
                while(qry.next())
                {
                    userbalance = qry.value(2).toInt();
                    ui->label_showBalance->setText(qry.value(2).toString());
                }
            }
        }
    }

    if(UserType == "staff")
    {
        qry.prepare("SELECT * FROM Staff WHERE Username = '"+username+"' AND Password = '"+password+"'");

        if (qry.exec())
        {
            int staffID;
            while(qry.next())
            {
                ui->label_showUsername->setText(username);
                ui->label_showName->setText(qry.value(1).toString());

                qry.first();
                QByteArray outByteArray = qry.value(8).toByteArray();
                QPixmap outPixmap = QPixmap();
                outPixmap.loadFromData(outByteArray);
                ui->label_userPicture->setPixmap(outPixmap.scaled(210,210));

                staffID = qry.value(0).toInt();
                ID = staffID;
            }

            qry.prepare("SELECT * FROM Staff_Balance WHERE Staff_ID = (:Staff_ID)");
            qry.bindValue(":Staff_ID", staffID);

            if (qry.exec())
            {
                while(qry.next())
                {
                    userbalance = qry.value(2).toInt();
                    ui->label_showBalance->setText(qry.value(2).toString());
                }
            }
        }
    }
    connect_database.sqlClose();
}

void CustomerWindow::showList()
{
    QStringList header;
    header << "Food" << "Qty." << "Price";
    ui->tableWidget->setColumnCount(3);
    ui->tableWidget->setHorizontalHeaderLabels(header);
    ui->tableWidget->setColumnWidth(0, 200);
    ui->tableWidget->setColumnWidth(1, 50);
    ui->tableWidget->setColumnWidth(2, 125);
}


void CustomerWindow::showTodaysSpecial_gif()
{
    QMovie *movie = new QMovie("C:/Users/User/Desktop/CanteenManagementSystem/todays_special.gif");
    movie->setScaledSize(QSize().scaled(150, 150, Qt::KeepAspectRatio));
    ui->label_gif->setMovie(movie);
    movie->start();
    connect(movie, SIGNAL(finished()), movie, SLOT(start()));
}

void CustomerWindow::showTodaysSpecial()
{
    MainWindow connect_database;

    if (!connect_database.sqlOpen())
    {
        qDebug() << "Failed to open the database";
        return;
    }

    connect_database.sqlOpen();

    QSqlQuery qry;

    qry.prepare("SELECT * FROM TodaysSpecial");

    if (qry.exec())
    {
        while(qry.next())
        {
            ui->label_showTodaysspecial->setText(qry.value(0).toString());
        }
        connect_database.sqlClose();
    }

}

void CustomerWindow::showDiscountoffer()
{
    MainWindow connect_database;

    if (!connect_database.sqlOpen())
    {
        qDebug() << "Failed to open the database";
        return;
    }

    connect_database.sqlOpen();

    QSqlQuery qry;

    qry.prepare("SELECT * FROM Discount_Offer");

    qry.exec();
    qry.last();

    QString foodname = qry.value(1).toString();
    QString discount_price = qry.value(2).toString();
    ui->label_showdiscountoffer->setText("Get " + foodname + " at just Rs. " + discount_price);

    connect_database.sqlClose();

}

void CustomerWindow::on_pushButton_breakfast_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void CustomerWindow::on_pushButton_lunch_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void CustomerWindow::on_pushButton_dinner_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

void CustomerWindow::on_pushButton_drinks_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}

void CustomerWindow::on_pushButton_logout_clicked()
{
//    CustomerWindow destroy;
//    destroy.~CustomerWindow();
//    MainWindow mainwin;
//    mainwin.show();


    this->hide();
    parentWidget()->show();
      /* Logout logout;
       logout.setModal(true);
       logout.exec()*/;
}

void CustomerWindow::on_pushButton_settings_clicked()
{
    CustomerAccountSettings *settings = new CustomerAccountSettings(this);
    connect(this, SIGNAL(send(const QString, const QString)), settings, SLOT(receive(const QString, const QString)));
    emit send(Username, Usertype);
    settings->setModal(true);
    settings->show();
}

CustomerWindow::~CustomerWindow()
{
    delete ui;
}

void CustomerWindow::on_pushButton_discardAll_clicked()
{
    ui->tableWidget->setRowCount(0);

    toastcount = 0; sandwichcount = 0; muffincount = 0; doughnutcount = 0;
    croissantcount = 0; cerealcount = 0; boiled_eggcount = 0; jerrycount = 0;
    puri_tarkaricount= 0; baconcount = 0; oatmealcount = 0; pancakecount = 0;

    momocount = 0;
    chowmeincount = 0;
    chicken_biryanicount = 0;
    mutton_biryanicount = 0;
    red_sauce_pastacount = 0;
    white_sauce_pastacount = 0;
    spaghetticount = 0;
    pizzacount = 0;
    aloo_chopcount = 0;
    naancount = 0;
    chatcount = 0;
    sausagecount = 0;

    rice_setcount = 0;
    roticount = 0;
    chicken_currycount = 0;
    mutton_currycount = 0;
    egg_currycount = 0;
    chicken_roastcount = 0;
    fish_frycount = 0;
    paneer_chillicount = 0;
    daal_frycount = 0;
    chana_masalacount = 0;
    rajma_masalacount = 0;
    papadcount = 0;

    black_teacount = 0;
    black_coffeecount = 0;
    milk_teacount = 0;
    milk_coffeecount = 0;
    pepsicount = 0;
    fantacount = 0;
    spritecount = 0;
    orange_juicecount = 0;
    redbullcount = 0;
    hot_lemoncount = 0;
    milkshakecount = 0;
    hot_chocolatecount = 0;

    total_price = 0;
    ui->label_total->hide();
    ui->label_showTotal->hide();
}

void CustomerWindow::on_pushButton_blackCoffee_clicked()
{
    ui->tableWidget->insertRow(ui->tableWidget->rowCount());
    int tablerow = ui->tableWidget->rowCount() - 1;
    ui->tableWidget->setItem(tablerow, 0, new QTableWidgetItem("Black Coffee"));
    ui->tableWidget->setItem(tablerow, 1, new QTableWidgetItem(QString::number(1)));
    ui->tableWidget->setItem(tablerow, 2, new QTableWidgetItem(QString::number(25)));
}

void CustomerWindow::on_pushButton_checkout_clicked()
{
    QString items[12][3];
    table_row = ui->tableWidget->rowCount();

    for(int row = 0; row <= (table_row - 1); row++)
    {
        for(int column = 0; column <= 2; column++)
        {
            items[row][column] = ui->tableWidget->item(row, column)->text();
        }
    }

    for(int row = 0; row <= (table_row - 1); row++)
    {
        QString price_string = ui->tableWidget->item(row, 2)->text();

        int price = price_string.toInt();
        total_price = total_price + price;
    }

    if (userbalance >= total_price)
    {
        Checkout checkout;
        checkout.setModal(true);
        checkout.receive_items(items, table_row, Usertype, userbalance, ID);
        checkout.exec();
    }
    else
    {
        QMessageBox msg(this);
        msg.setStyleSheet("border-image: none");
        msg.setIcon(QMessageBox::Icon::Warning);
        msg.setText("You have insufficient balance");
        msg.exec();
    }
}

void CustomerWindow::showBreakfastThumbnails()
{
    MainWindow connect_database;

    connect_database.sqlOpen();

    QSqlQuery qry;

    QString breakfast_foodTitle;

    //Toast
    breakfast_foodTitle = "Toast";
    qry.prepare("SELECT * FROM Food_Item WHERE Food_Title = '"+breakfast_foodTitle+"'");

    if (qry.exec())
    {
        while(qry.next())
        {
            QByteArray outByteArray = qry.value(4).toByteArray();
            QPixmap outPixmap = QPixmap();
            outPixmap.loadFromData(outByteArray);
            ui->pushButton_Toast->setIcon(outPixmap);
            ui->pushButton_Toast->setIconSize(QSize(183, 183));
        }
    }

    //Sandwich
    breakfast_foodTitle = "Sandwich";
    qry.prepare("SELECT * FROM Food_Item WHERE Food_Title = '"+breakfast_foodTitle+"'");

    if (qry.exec())
    {
        while(qry.next())
        {
            QByteArray outByteArray = qry.value(4).toByteArray();
            QPixmap outPixmap = QPixmap();
            outPixmap.loadFromData(outByteArray);
            ui->pushButton_Sandwich->setIcon(outPixmap);
            ui->pushButton_Sandwich->setIconSize(QSize(183, 183));
        }
    }

    //Muffin
    breakfast_foodTitle = "Muffin";
    qry.prepare("SELECT * FROM Food_Item WHERE Food_Title = '"+breakfast_foodTitle+"'");

    if (qry.exec())
    {
        while(qry.next())
        {
            QByteArray outByteArray = qry.value(4).toByteArray();
            QPixmap outPixmap = QPixmap();
            outPixmap.loadFromData(outByteArray);
            ui->pushButton_Muffin->setIcon(outPixmap);
            ui->pushButton_Muffin->setIconSize(QSize(183, 183));
        }
    }

    //Doughnut
    breakfast_foodTitle = "Doughnut";
    qry.prepare("SELECT * FROM Food_Item WHERE Food_Title = '"+breakfast_foodTitle+"'");

    if (qry.exec())
    {
        while(qry.next())
        {
            QByteArray outByteArray = qry.value(4).toByteArray();
            QPixmap outPixmap = QPixmap();
            outPixmap.loadFromData(outByteArray);
            ui->pushButton_Doughnut->setIcon(outPixmap);
            ui->pushButton_Doughnut->setIconSize(QSize(183, 183));
        }
    }

    //Croissant
    breakfast_foodTitle = "Croissant";
    qry.prepare("SELECT * FROM Food_Item WHERE Food_Title = '"+breakfast_foodTitle+"'");

    if (qry.exec())
    {
        while(qry.next())
        {
            QByteArray outByteArray = qry.value(4).toByteArray();
            QPixmap outPixmap = QPixmap();
            outPixmap.loadFromData(outByteArray);
            ui->pushButton_Croissant->setIcon(outPixmap);
            ui->pushButton_Croissant->setIconSize(QSize(183, 183));
        }
    }

    //Cereal
    breakfast_foodTitle = "Cereal";
    qry.prepare("SELECT * FROM Food_Item WHERE Food_Title = '"+breakfast_foodTitle+"'");

    if (qry.exec())
    {
        while(qry.next())
        {
            QByteArray outByteArray = qry.value(4).toByteArray();
            QPixmap outPixmap = QPixmap();
            outPixmap.loadFromData(outByteArray);
            ui->pushButton_Cereal->setIcon(outPixmap);
            ui->pushButton_Cereal->setIconSize(QSize(183, 183));
        }
    }

    //Boiled Egg
    breakfast_foodTitle = "Boiled Egg";
    qry.prepare("SELECT * FROM Food_Item WHERE Food_Title = '"+breakfast_foodTitle+"'");

    if (qry.exec())
    {
        while(qry.next())
        {
            QByteArray outByteArray = qry.value(4).toByteArray();
            QPixmap outPixmap = QPixmap();
            outPixmap.loadFromData(outByteArray);
            ui->pushButton_Boiled_Egg->setIcon(outPixmap);
            ui->pushButton_Boiled_Egg->setIconSize(QSize(183, 183));
        }
    }

    //Jerry
    breakfast_foodTitle = "Jerry";
    qry.prepare("SELECT * FROM Food_Item WHERE Food_Title = '"+breakfast_foodTitle+"'");

    if (qry.exec())
    {
        while(qry.next())
        {
            QByteArray outByteArray = qry.value(4).toByteArray();
            QPixmap outPixmap = QPixmap();
            outPixmap.loadFromData(outByteArray);
            ui->pushButton_Jerry->setIcon(outPixmap);
            ui->pushButton_Jerry->setIconSize(QSize(183, 183));
        }
    }

    //Puri Tarkari
    breakfast_foodTitle = "Puri Tarkari";
    qry.prepare("SELECT * FROM Food_Item WHERE Food_Title = '"+breakfast_foodTitle+"'");

    if (qry.exec())
    {
        while(qry.next())
        {
            QByteArray outByteArray = qry.value(4).toByteArray();
            QPixmap outPixmap = QPixmap();
            outPixmap.loadFromData(outByteArray);
            ui->pushButton_Puri_Tarkari->setIcon(outPixmap);
            ui->pushButton_Puri_Tarkari->setIconSize(QSize(183, 183));
        }
    }

    //Bacon
    breakfast_foodTitle = "Bacon";
    qry.prepare("SELECT * FROM Food_Item WHERE Food_Title = '"+breakfast_foodTitle+"'");

    if (qry.exec())
    {
        while(qry.next())
        {
            QByteArray outByteArray = qry.value(4).toByteArray();
            QPixmap outPixmap = QPixmap();
            outPixmap.loadFromData(outByteArray);
            ui->pushButton_Bacon->setIcon(outPixmap);
            ui->pushButton_Bacon->setIconSize(QSize(183, 183));
        }
    }

    //Oatmeal
    breakfast_foodTitle = "Oatmeal";
    qry.prepare("SELECT * FROM Food_Item WHERE Food_Title = '"+breakfast_foodTitle+"'");

    if (qry.exec())
    {
        while(qry.next())
        {
            QByteArray outByteArray = qry.value(4).toByteArray();
            QPixmap outPixmap = QPixmap();
            outPixmap.loadFromData(outByteArray);
            ui->pushButton_Oatmeal->setIcon(outPixmap);
            ui->pushButton_Oatmeal->setIconSize(QSize(183, 183));
        }
    }

    //Pancake
    breakfast_foodTitle = "Pancake";
    qry.prepare("SELECT * FROM Food_Item WHERE Food_Title = '"+breakfast_foodTitle+"'");

    if (qry.exec())
    {
        while(qry.next())
        {
            QByteArray outByteArray = qry.value(4).toByteArray();
            QPixmap outPixmap = QPixmap();
            outPixmap.loadFromData(outByteArray);
            ui->pushButton_Pancake->setIcon(outPixmap);
            ui->pushButton_Pancake->setIconSize(QSize(183, 183));
        }
    }

    connect_database.sqlClose();
}

void CustomerWindow::showLunchThumbnails()
{
    MainWindow connect_database;

    connect_database.sqlOpen();

    QSqlQuery qry;

    QString lunch_foodTitle;

    //MoMo
    lunch_foodTitle = "MoMo";
    qry.prepare("SELECT * FROM Food_Item WHERE Food_Title = '"+lunch_foodTitle+"'");

    if (qry.exec())
    {
        while(qry.next())
        {
            QByteArray outByteArray = qry.value(4).toByteArray();
            QPixmap outPixmap = QPixmap();
            outPixmap.loadFromData(outByteArray);
            ui->pushButton_MoMo->setIcon(outPixmap);
            ui->pushButton_MoMo->setIconSize(QSize(183, 183));
        }
    }

    //Chowmein
    lunch_foodTitle = "Chowmein";
    qry.prepare("SELECT * FROM Food_Item WHERE Food_Title = '"+lunch_foodTitle+"'");

    if (qry.exec())
    {
        while(qry.next())
        {
            QByteArray outByteArray = qry.value(4).toByteArray();
            QPixmap outPixmap = QPixmap();
            outPixmap.loadFromData(outByteArray);
            ui->pushButton_Chowmein->setIcon(outPixmap);
            ui->pushButton_Chowmein->setIconSize(QSize(183, 183));
        }
    }

    //Chicken Biryani
    lunch_foodTitle = "Chicken Biryani";
    qry.prepare("SELECT * FROM Food_Item WHERE Food_Title = '"+lunch_foodTitle+"'");

    if (qry.exec())
    {
        while(qry.next())
        {
            QByteArray outByteArray = qry.value(4).toByteArray();
            QPixmap outPixmap = QPixmap();
            outPixmap.loadFromData(outByteArray);
            ui->pushButton_Chicken_Biryani->setIcon(outPixmap);
            ui->pushButton_Chicken_Biryani->setIconSize(QSize(183, 183));
        }
    }

    //Mutton Biryani
    lunch_foodTitle = "Mutton Biryani";
    qry.prepare("SELECT * FROM Food_Item WHERE Food_Title = '"+lunch_foodTitle+"'");

    if (qry.exec())
    {
        while(qry.next())
        {
            QByteArray outByteArray = qry.value(4).toByteArray();
            QPixmap outPixmap = QPixmap();
            outPixmap.loadFromData(outByteArray);
            ui->pushButton_Mutton_Biryani->setIcon(outPixmap);
            ui->pushButton_Mutton_Biryani->setIconSize(QSize(183, 183));
        }
    }

    //Red Sauce Pasta
    lunch_foodTitle = "Red Sauce Pasta";
    qry.prepare("SELECT * FROM Food_Item WHERE Food_Title = '"+lunch_foodTitle+"'");

    if (qry.exec())
    {
        while(qry.next())
        {
            QByteArray outByteArray = qry.value(4).toByteArray();
            QPixmap outPixmap = QPixmap();
            outPixmap.loadFromData(outByteArray);
            ui->pushButton_Red_Sauce_Pasta->setIcon(outPixmap);
            ui->pushButton_Red_Sauce_Pasta->setIconSize(QSize(183, 183));
        }
    }

    //White Sauce Pasta
    lunch_foodTitle = "White Sauce Pasta";
    qry.prepare("SELECT * FROM Food_Item WHERE Food_Title = '"+lunch_foodTitle+"'");

    if (qry.exec())
    {
        while(qry.next())
        {
            QByteArray outByteArray = qry.value(4).toByteArray();
            QPixmap outPixmap = QPixmap();
            outPixmap.loadFromData(outByteArray);
            ui->pushButton_White_Sauce_Pasta->setIcon(outPixmap);
            ui->pushButton_White_Sauce_Pasta->setIconSize(QSize(183, 183));
        }
    }

    //Spaghetti
    lunch_foodTitle = "Spaghetti";
    qry.prepare("SELECT * FROM Food_Item WHERE Food_Title = '"+lunch_foodTitle+"'");

    if (qry.exec())
    {
        while(qry.next())
        {
            QByteArray outByteArray = qry.value(4).toByteArray();
            QPixmap outPixmap = QPixmap();
            outPixmap.loadFromData(outByteArray);
            ui->pushButton_Spaghetti->setIcon(outPixmap);
            ui->pushButton_Spaghetti->setIconSize(QSize(183, 183));
        }
    }

    //Aloo Chop
    lunch_foodTitle = "Aloo Chop";
    qry.prepare("SELECT * FROM Food_Item WHERE Food_Title = '"+lunch_foodTitle+"'");

    if (qry.exec())
    {
        while(qry.next())
        {
            QByteArray outByteArray = qry.value(4).toByteArray();
            QPixmap outPixmap = QPixmap();
            outPixmap.loadFromData(outByteArray);
            ui->pushButton_Aloo_Chop->setIcon(outPixmap);
            ui->pushButton_Aloo_Chop->setIconSize(QSize(183, 183));
        }
    }

    //Pizza
    lunch_foodTitle = "Pizza";
    qry.prepare("SELECT * FROM Food_Item WHERE Food_Title = '"+lunch_foodTitle+"'");

    if (qry.exec())
    {
        while(qry.next())
        {
            QByteArray outByteArray = qry.value(4).toByteArray();
            QPixmap outPixmap = QPixmap();
            outPixmap.loadFromData(outByteArray);
            ui->pushButton_Pizza->setIcon(outPixmap);
            ui->pushButton_Pizza->setIconSize(QSize(183, 183));
        }
    }

    //Naan
    lunch_foodTitle = "Naan";
    qry.prepare("SELECT * FROM Food_Item WHERE Food_Title = '"+lunch_foodTitle+"'");

    if (qry.exec())
    {
        while(qry.next())
        {
            QByteArray outByteArray = qry.value(4).toByteArray();
            QPixmap outPixmap = QPixmap();
            outPixmap.loadFromData(outByteArray);
            ui->pushButton_Naan->setIcon(outPixmap);
            ui->pushButton_Naan->setIconSize(QSize(183, 183));
        }
    }

    //Chat
    lunch_foodTitle = "Chat";
    qry.prepare("SELECT * FROM Food_Item WHERE Food_Title = '"+lunch_foodTitle+"'");

    if (qry.exec())
    {
        while(qry.next())
        {
            QByteArray outByteArray = qry.value(4).toByteArray();
            QPixmap outPixmap = QPixmap();
            outPixmap.loadFromData(outByteArray);
            ui->pushButton_Chat->setIcon(outPixmap);
            ui->pushButton_Chat->setIconSize(QSize(183, 183));
        }
    }

    //Sausage
    lunch_foodTitle = "Sausage";
    qry.prepare("SELECT * FROM Food_Item WHERE Food_Title = '"+lunch_foodTitle+"'");

    if (qry.exec())
    {
        while(qry.next())
        {
            QByteArray outByteArray = qry.value(4).toByteArray();
            QPixmap outPixmap = QPixmap();
            outPixmap.loadFromData(outByteArray);
            ui->pushButton_Sausage->setIcon(outPixmap);
            ui->pushButton_Sausage->setIconSize(QSize(183, 183));
        }
    }

    connect_database.sqlClose();
}

void CustomerWindow::showDinnerThumbnails()
{
    MainWindow connect_database;

    connect_database.sqlOpen();

    QSqlQuery qry;

    QString dinner_foodTitle;

    //Rice Set
    dinner_foodTitle = "Rice Set";
    qry.prepare("SELECT * FROM Food_Item WHERE Food_Title = '"+dinner_foodTitle+"'");

    if (qry.exec())
    {
        while(qry.next())
        {
            QByteArray outByteArray = qry.value(4).toByteArray();
            QPixmap outPixmap = QPixmap();
            outPixmap.loadFromData(outByteArray);
            ui->pushButton_Rice_Set->setIcon(outPixmap);
            ui->pushButton_Rice_Set->setIconSize(QSize(183, 183));
        }
    }

    //Roti
    dinner_foodTitle = "Roti";
    qry.prepare("SELECT * FROM Food_Item WHERE Food_Title = '"+dinner_foodTitle+"'");

    if (qry.exec())
    {
        while(qry.next())
        {
            QByteArray outByteArray = qry.value(4).toByteArray();
            QPixmap outPixmap = QPixmap();
            outPixmap.loadFromData(outByteArray);
            ui->pushButton_Roti->setIcon(outPixmap);
            ui->pushButton_Roti->setIconSize(QSize(183, 183));
        }
    }

    //Chicken Curry
    dinner_foodTitle = "Chicken Curry";
    qry.prepare("SELECT * FROM Food_Item WHERE Food_Title = '"+dinner_foodTitle+"'");

    if (qry.exec())
    {
        while(qry.next())
        {
            QByteArray outByteArray = qry.value(4).toByteArray();
            QPixmap outPixmap = QPixmap();
            outPixmap.loadFromData(outByteArray);
            ui->pushButton_Chicken_Curry->setIcon(outPixmap);
            ui->pushButton_Chicken_Curry->setIconSize(QSize(183, 183));
        }
    }

    //Mutton Curry
    dinner_foodTitle = "Mutton Curry";
    qry.prepare("SELECT * FROM Food_Item WHERE Food_Title = '"+dinner_foodTitle+"'");

    if (qry.exec())
    {
        while(qry.next())
        {
            QByteArray outByteArray = qry.value(4).toByteArray();
            QPixmap outPixmap = QPixmap();
            outPixmap.loadFromData(outByteArray);
            ui->pushButton_Mutton_Curry->setIcon(outPixmap);
            ui->pushButton_Mutton_Curry->setIconSize(QSize(183, 183));
        }
    }

    //Egg Curry
    dinner_foodTitle = "Egg Curry";
    qry.prepare("SELECT * FROM Food_Item WHERE Food_Title = '"+dinner_foodTitle+"'");

    if (qry.exec())
    {
        while(qry.next())
        {
            QByteArray outByteArray = qry.value(4).toByteArray();
            QPixmap outPixmap = QPixmap();
            outPixmap.loadFromData(outByteArray);
            ui->pushButton_Egg_Curry->setIcon(outPixmap);
            ui->pushButton_Egg_Curry->setIconSize(QSize(183, 183));
        }
    }

    //Chicken Roast
    dinner_foodTitle = "Chicken Roast";
    qry.prepare("SELECT * FROM Food_Item WHERE Food_Title = '"+dinner_foodTitle+"'");

    if (qry.exec())
    {
        while(qry.next())
        {
            QByteArray outByteArray = qry.value(4).toByteArray();
            QPixmap outPixmap = QPixmap();
            outPixmap.loadFromData(outByteArray);
            ui->pushButton_Chicken_Roast->setIcon(outPixmap);
            ui->pushButton_Chicken_Roast->setIconSize(QSize(183, 183));
        }
    }

    //Fish Fry
    dinner_foodTitle = "Fish Fry";
    qry.prepare("SELECT * FROM Food_Item WHERE Food_Title = '"+dinner_foodTitle+"'");

    if (qry.exec())
    {
        while(qry.next())
        {
            QByteArray outByteArray = qry.value(4).toByteArray();
            QPixmap outPixmap = QPixmap();
            outPixmap.loadFromData(outByteArray);
            ui->pushButton_Fish_Fry->setIcon(outPixmap);
            ui->pushButton_Fish_Fry->setIconSize(QSize(183, 183));
        }
    }

    //Paneer_Chilli
    dinner_foodTitle = "Paneer Chilli";
    qry.prepare("SELECT * FROM Food_Item WHERE Food_Title = '"+dinner_foodTitle+"'");

    if (qry.exec())
    {
        while(qry.next())
        {
            QByteArray outByteArray = qry.value(4).toByteArray();
            QPixmap outPixmap = QPixmap();
            outPixmap.loadFromData(outByteArray);
            ui->pushButton_Paneer_Chilli->setIcon(outPixmap);
            ui->pushButton_Paneer_Chilli->setIconSize(QSize(183, 183));
        }
    }

    //Daal Fry
    dinner_foodTitle = "Daal Fry";
    qry.prepare("SELECT * FROM Food_Item WHERE Food_Title = '"+dinner_foodTitle+"'");

    if (qry.exec())
    {
        while(qry.next())
        {
            QByteArray outByteArray = qry.value(4).toByteArray();
            QPixmap outPixmap = QPixmap();
            outPixmap.loadFromData(outByteArray);
            ui->pushButton_Daal_Fry->setIcon(outPixmap);
            ui->pushButton_Daal_Fry->setIconSize(QSize(183, 183));
        }
    }

    //Chana Masala
    dinner_foodTitle = "Chana Masala";
    qry.prepare("SELECT * FROM Food_Item WHERE Food_Title = '"+dinner_foodTitle+"'");

    if (qry.exec())
    {
        while(qry.next())
        {
            QByteArray outByteArray = qry.value(4).toByteArray();
            QPixmap outPixmap = QPixmap();
            outPixmap.loadFromData(outByteArray);
            ui->pushButton_Chana_Masala->setIcon(outPixmap);
            ui->pushButton_Chana_Masala->setIconSize(QSize(183, 183));
        }
    }

    //Rajma Masala
    dinner_foodTitle = "Rajma Masala";
    qry.prepare("SELECT * FROM Food_Item WHERE Food_Title = '"+dinner_foodTitle+"'");

    if (qry.exec())
    {
        while(qry.next())
        {
            QByteArray outByteArray = qry.value(4).toByteArray();
            QPixmap outPixmap = QPixmap();
            outPixmap.loadFromData(outByteArray);
            ui->pushButton_Rajma_Masala->setIcon(outPixmap);
            ui->pushButton_Rajma_Masala->setIconSize(QSize(183, 183));
        }
    }

    //Papad
    dinner_foodTitle = "Papad";
    qry.prepare("SELECT * FROM Food_Item WHERE Food_Title = '"+dinner_foodTitle+"'");

    if (qry.exec())
    {
        while(qry.next())
        {
            QByteArray outByteArray = qry.value(4).toByteArray();
            QPixmap outPixmap = QPixmap();
            outPixmap.loadFromData(outByteArray);
            ui->pushButton_Papad->setIcon(outPixmap);
            ui->pushButton_Papad->setIconSize(QSize(183, 183));
        }
    }

    connect_database.sqlClose();
}

void CustomerWindow::showDrinksThumbnails()
{
    MainWindow connect_database;

    connect_database.sqlOpen();

    QSqlQuery qry;

    QString drinks_foodTitle;

    //Black Tea
    drinks_foodTitle = "Black Tea";
    qry.prepare("SELECT * FROM Food_Item WHERE Food_Title = '"+drinks_foodTitle+"'");

    if (qry.exec())
    {
        while(qry.next())
        {
            QByteArray outByteArray = qry.value(4).toByteArray();
            QPixmap outPixmap = QPixmap();
            outPixmap.loadFromData(outByteArray);
            ui->pushButton_Black_Tea->setIcon(outPixmap);
            ui->pushButton_Black_Tea->setIconSize(QSize(183, 183));
        }
    }

    //Black Coffee
    drinks_foodTitle = "Black Coffee";
    qry.prepare("SELECT * FROM Food_Item WHERE Food_Title = '"+drinks_foodTitle+"'");

    if (qry.exec())
    {
        while(qry.next())
        {
            QByteArray outByteArray = qry.value(4).toByteArray();
            QPixmap outPixmap = QPixmap();
            outPixmap.loadFromData(outByteArray);
            ui->pushButton_Black_Coffee->setIcon(outPixmap);
            ui->pushButton_Black_Coffee->setIconSize(QSize(183, 183));
        }
    }

    //Milk Tea
    drinks_foodTitle = "Milk Tea";
    qry.prepare("SELECT * FROM Food_Item WHERE Food_Title = '"+drinks_foodTitle+"'");

    if (qry.exec())
    {
        while(qry.next())
        {
            QByteArray outByteArray = qry.value(4).toByteArray();
            QPixmap outPixmap = QPixmap();
            outPixmap.loadFromData(outByteArray);
            ui->pushButton_Milk_Tea->setIcon(outPixmap);
            ui->pushButton_Milk_Tea->setIconSize(QSize(183, 183));
        }
    }

    //Milk Coffee
    drinks_foodTitle = "Milk Coffee";
    qry.prepare("SELECT * FROM Food_Item WHERE Food_Title = '"+drinks_foodTitle+"'");

    if (qry.exec())
    {
        while(qry.next())
        {
            QByteArray outByteArray = qry.value(4).toByteArray();
            QPixmap outPixmap = QPixmap();
            outPixmap.loadFromData(outByteArray);
            ui->pushButton_Milk_Coffee->setIcon(outPixmap);
            ui->pushButton_Milk_Coffee->setIconSize(QSize(183, 183));
        }
    }

    //Pepsi
    drinks_foodTitle = "Pepsi";
    qry.prepare("SELECT * FROM Food_Item WHERE Food_Title = '"+drinks_foodTitle+"'");

    if (qry.exec())
    {
        while(qry.next())
        {
            QByteArray outByteArray = qry.value(4).toByteArray();
            QPixmap outPixmap = QPixmap();
            outPixmap.loadFromData(outByteArray);
            ui->pushButton_Pepsi->setIcon(outPixmap);
            ui->pushButton_Pepsi->setIconSize(QSize(183, 183));
        }
    }

    //Fanta
    drinks_foodTitle = "Fanta";
    qry.prepare("SELECT * FROM Food_Item WHERE Food_Title = '"+drinks_foodTitle+"'");

    if (qry.exec())
    {
        while(qry.next())
        {
            QByteArray outByteArray = qry.value(4).toByteArray();
            QPixmap outPixmap = QPixmap();
            outPixmap.loadFromData(outByteArray);
            ui->pushButton_Fanta->setIcon(outPixmap);
            ui->pushButton_Fanta->setIconSize(QSize(183, 183));
        }
    }

    //Sprite
    drinks_foodTitle = "Sprite";
    qry.prepare("SELECT * FROM Food_Item WHERE Food_Title = '"+drinks_foodTitle+"'");

    if (qry.exec())
    {
        while(qry.next())
        {
            QByteArray outByteArray = qry.value(4).toByteArray();
            QPixmap outPixmap = QPixmap();
            outPixmap.loadFromData(outByteArray);
            ui->pushButton_Sprite->setIcon(outPixmap);
            ui->pushButton_Sprite->setIconSize(QSize(183, 183));
        }
    }

    //Orange Juice
    drinks_foodTitle = "Orange Juice";
    qry.prepare("SELECT * FROM Food_Item WHERE Food_Title = '"+drinks_foodTitle+"'");

    if (qry.exec())
    {
        while(qry.next())
        {
            QByteArray outByteArray = qry.value(4).toByteArray();
            QPixmap outPixmap = QPixmap();
            outPixmap.loadFromData(outByteArray);
            ui->pushButton_Orange_Juice->setIcon(outPixmap);
            ui->pushButton_Orange_Juice->setIconSize(QSize(183, 183));
        }
    }

    //Redbull
    drinks_foodTitle = "Redbull";
    qry.prepare("SELECT * FROM Food_Item WHERE Food_Title = '"+drinks_foodTitle+"'");

    if (qry.exec())
    {
        while(qry.next())
        {
            QByteArray outByteArray = qry.value(4).toByteArray();
            QPixmap outPixmap = QPixmap();
            outPixmap.loadFromData(outByteArray);
            ui->pushButton_Redbull->setIcon(outPixmap);
            ui->pushButton_Redbull->setIconSize(QSize(183, 183));
        }
    }

    //Hot Lemon
    drinks_foodTitle = "Hot Lemon";
    qry.prepare("SELECT * FROM Food_Item WHERE Food_Title = '"+drinks_foodTitle+"'");

    if (qry.exec())
    {
        while(qry.next())
        {
            QByteArray outByteArray = qry.value(4).toByteArray();
            QPixmap outPixmap = QPixmap();
            outPixmap.loadFromData(outByteArray);
            ui->pushButton_Hot_Lemon->setIcon(outPixmap);
            ui->pushButton_Hot_Lemon->setIconSize(QSize(183, 183));
        }
    }

    //Milkshake
    drinks_foodTitle = "Milkshake";
    qry.prepare("SELECT * FROM Food_Item WHERE Food_Title = '"+drinks_foodTitle+"'");

    if (qry.exec())
    {
        while(qry.next())
        {
            QByteArray outByteArray = qry.value(4).toByteArray();
            QPixmap outPixmap = QPixmap();
            outPixmap.loadFromData(outByteArray);
            ui->pushButton_Milkshake->setIcon(outPixmap);
            ui->pushButton_Milkshake->setIconSize(QSize(183, 183));
        }
    }

    //Hot Chocolate
    drinks_foodTitle = "Hot Chocolate";
    qry.prepare("SELECT * FROM Food_Item WHERE Food_Title = '"+drinks_foodTitle+"'");

    if (qry.exec())
    {
        while(qry.next())
        {
            QByteArray outByteArray = qry.value(4).toByteArray();
            QPixmap outPixmap = QPixmap();
            outPixmap.loadFromData(outByteArray);
            ui->pushButton_Hot_Chocolate->setIcon(outPixmap);
            ui->pushButton_Hot_Chocolate->setIconSize(QSize(183, 183));
        }
    }

    connect_database.sqlClose();
}

void CustomerWindow::showOverflow()
{
    QMessageBox msg(this);
    msg.setStyleSheet("border-image: none");
    msg.setWindowTitle(QLatin1String("Error"));
    msg.setIcon(QMessageBox::Icon::Warning);
    msg.setText("You cannot select more that 12 items");
    msg.exec();
}

void CustomerWindow::showTotalFoodAmount()
{
    ui->label_total->show();
    ui->label_showTotal->show();

    table_row = ui->tableWidget->rowCount();
    total_price = 0;
    qDebug() << "No. of rows = " << table_row;
    for(int row = 0; row <= (table_row - 1); row++)
    {
        QString food_price_string = ui->tableWidget->item(row, 2)->text();
        int food_price = food_price_string.toInt();

        total_price = total_price + food_price;
    }
    ui->label_showTotal->setText(QString::number(total_price));
}


///////////////////////////////Codes for Thumbnails/////////////////////////////////////

void CustomerWindow::on_pushButton_Toast_clicked()
{
    int price = 20;
    toastcount++;

    if (toastcount == 1)
    {
        int tablerow = ui->tableWidget->rowCount();
        if(tablerow <= 11)
        {
            ui->tableWidget->insertRow(ui->tableWidget->rowCount());
            int tablerow = ui->tableWidget->rowCount() - 1;

            ui->tableWidget->setItem(tablerow, 0, new QTableWidgetItem("Toast"));
            ui->tableWidget->setItem(tablerow, 1, new QTableWidgetItem(QString::number(toastcount)));
            ui->tableWidget->setItem(tablerow, 2, new QTableWidgetItem(QString::number(price)));
        }
        else
        {
           showOverflow();
        }
    }
    else
    {
        QString item;
        int tablerow = 0;
        do
        {
            int column = 0;
            QString item;
            item = ui->tableWidget->item(tablerow, column)->text();
            if (item == "Toast")
            {
                ui->tableWidget->setItem(tablerow, 1, new QTableWidgetItem(QString::number(toastcount)));
                ui->tableWidget->setItem(tablerow, 2, new QTableWidgetItem(QString::number(price * toastcount)));
                break;
            }
            tablerow++;
        }
        while (tablerow <= 11);
    }
    showTotalFoodAmount();
}

void CustomerWindow::on_pushButton_Sandwich_clicked()
{
    int price = 40;
    sandwichcount++;

    if (sandwichcount == 1)
    {
        int tablerow = ui->tableWidget->rowCount();
        if(tablerow <= 11)
        {
            ui->tableWidget->insertRow(ui->tableWidget->rowCount());
            int tablerow = ui->tableWidget->rowCount() - 1;

            ui->tableWidget->setItem(tablerow, 0, new QTableWidgetItem("Sandwich"));
            ui->tableWidget->setItem(tablerow, 1, new QTableWidgetItem(QString::number(sandwichcount)));
            ui->tableWidget->setItem(tablerow, 2, new QTableWidgetItem(QString::number(price)));
        }
        else
        {
           showOverflow();
        }
    }
    else
    {
        QString item;
        int tablerow = 0;
        do
        {
            int column = 0;
            QString item;
            item = ui->tableWidget->item(tablerow, column)->text();
            if (item == "Sandwich")
            {
                ui->tableWidget->setItem(tablerow, 1, new QTableWidgetItem(QString::number(sandwichcount)));
                ui->tableWidget->setItem(tablerow, 2, new QTableWidgetItem(QString::number(price * sandwichcount)));
                break;
            }
            tablerow++;
        }
        while (tablerow <= 11);
    }
    showTotalFoodAmount();
}

void CustomerWindow::on_pushButton_Muffin_clicked()
{
    int price = 25;
    muffincount++;

    if (muffincount == 1)
    {
        int tablerow = ui->tableWidget->rowCount();
        if(tablerow <= 11)
        {
            ui->tableWidget->insertRow(ui->tableWidget->rowCount());
            int tablerow = ui->tableWidget->rowCount() - 1;

            ui->tableWidget->setItem(tablerow, 0, new QTableWidgetItem("Muffin"));
            ui->tableWidget->setItem(tablerow, 1, new QTableWidgetItem(QString::number(muffincount)));
            ui->tableWidget->setItem(tablerow, 2, new QTableWidgetItem(QString::number(price)));
        }
        else
        {
           showOverflow();
        }
    }
    else
    {
        QString item;
        int tablerow = 0;
        do
        {
            int column = 0;
            QString item;
            item = ui->tableWidget->item(tablerow, column)->text();
            if (item == "Muffin")
            {
                ui->tableWidget->setItem(tablerow, 1, new QTableWidgetItem(QString::number(muffincount)));
                ui->tableWidget->setItem(tablerow, 2, new QTableWidgetItem(QString::number(price * muffincount)));
                break;
            }
            tablerow++;
        }
        while (tablerow <= 11);
    }
    showTotalFoodAmount();
}

void CustomerWindow::on_pushButton_Doughnut_clicked()
{
    int price = 25;
    doughnutcount++;

    if (doughnutcount == 1)
    {
        int tablerow = ui->tableWidget->rowCount();
        if(tablerow <= 11)
        {
            ui->tableWidget->insertRow(ui->tableWidget->rowCount());
            int tablerow = ui->tableWidget->rowCount() - 1;

            ui->tableWidget->setItem(tablerow, 0, new QTableWidgetItem("Doughnut"));
            ui->tableWidget->setItem(tablerow, 1, new QTableWidgetItem(QString::number(doughnutcount)));
            ui->tableWidget->setItem(tablerow, 2, new QTableWidgetItem(QString::number(price)));
        }
        else
        {
           showOverflow();
        }
    }
    else
    {
        QString item;
        int tablerow = 0;
        do
        {
            int column = 0;
            QString item;
            item = ui->tableWidget->item(tablerow, column)->text();
            if (item == "Doughnut")
            {
                ui->tableWidget->setItem(tablerow, 1, new QTableWidgetItem(QString::number(doughnutcount)));
                ui->tableWidget->setItem(tablerow, 2, new QTableWidgetItem(QString::number(price * doughnutcount)));
                break;
            }
            tablerow++;
        }
        while (tablerow <= 11);
    }
    showTotalFoodAmount();
}

void CustomerWindow::on_pushButton_Croissant_clicked()
{
    int price = 25;
    croissantcount++;

    if (croissantcount == 1)
    {
        int tablerow = ui->tableWidget->rowCount();
        if(tablerow <= 11)
        {
            ui->tableWidget->insertRow(ui->tableWidget->rowCount());
            int tablerow = ui->tableWidget->rowCount() - 1;

            ui->tableWidget->setItem(tablerow, 0, new QTableWidgetItem("Croissant"));
            ui->tableWidget->setItem(tablerow, 1, new QTableWidgetItem(QString::number(croissantcount)));
            ui->tableWidget->setItem(tablerow, 2, new QTableWidgetItem(QString::number(price)));
        }
        else
        {
           showOverflow();
        }
    }
    else
    {
        QString item;
        int tablerow = 0;
        do
        {
            int column = 0;
            QString item;
            item = ui->tableWidget->item(tablerow, column)->text();
            if (item == "Croissant")
            {
                ui->tableWidget->setItem(tablerow, 1, new QTableWidgetItem(QString::number(croissantcount)));
                ui->tableWidget->setItem(tablerow, 2, new QTableWidgetItem(QString::number(price * croissantcount)));
                break;
            }
            tablerow++;
        }
        while (tablerow <= 11);
    }
    showTotalFoodAmount();
}

void CustomerWindow::on_pushButton_Cereal_clicked()
{
    int price = 25;
    cerealcount++;

    if (cerealcount == 1)
    {
        int tablerow = ui->tableWidget->rowCount();
        if(tablerow <= 11)
        {
            ui->tableWidget->insertRow(ui->tableWidget->rowCount());
            int tablerow = ui->tableWidget->rowCount() - 1;

            ui->tableWidget->setItem(tablerow, 0, new QTableWidgetItem("Cereal"));
            ui->tableWidget->setItem(tablerow, 1, new QTableWidgetItem(QString::number(cerealcount)));
            ui->tableWidget->setItem(tablerow, 2, new QTableWidgetItem(QString::number(price)));
        }
        else
        {
           showOverflow();
        }
    }
    else
    {
        QString item;
        int tablerow = 0;
        do
        {
            int column = 0;
            QString item;
            item = ui->tableWidget->item(tablerow, column)->text();
            if (item == "Cereal")
            {
                ui->tableWidget->setItem(tablerow, 1, new QTableWidgetItem(QString::number(cerealcount)));
                ui->tableWidget->setItem(tablerow, 2, new QTableWidgetItem(QString::number(price * cerealcount)));
                break;
            }
            tablerow++;
        }
        while (tablerow <= 11);
    }
    showTotalFoodAmount();
}

void CustomerWindow::on_pushButton_Boiled_Egg_clicked()
{
    int price = 25;
    boiled_eggcount++;

    if (boiled_eggcount == 1)
    {
        int tablerow = ui->tableWidget->rowCount();
        if(tablerow <= 11)
        {
            ui->tableWidget->insertRow(ui->tableWidget->rowCount());
            int tablerow = ui->tableWidget->rowCount() - 1;

            ui->tableWidget->setItem(tablerow, 0, new QTableWidgetItem("Boiled Egg"));
            ui->tableWidget->setItem(tablerow, 1, new QTableWidgetItem(QString::number(boiled_eggcount)));
            ui->tableWidget->setItem(tablerow, 2, new QTableWidgetItem(QString::number(price)));
        }
        else
        {
           showOverflow();
        }
    }
    else
    {
        QString item;
        int tablerow = 0;
        do
        {
            int column = 0;
            QString item;
            item = ui->tableWidget->item(tablerow, column)->text();
            if (item == "Boiled Egg")
            {
                ui->tableWidget->setItem(tablerow, 1, new QTableWidgetItem(QString::number(boiled_eggcount)));
                ui->tableWidget->setItem(tablerow, 2, new QTableWidgetItem(QString::number(price * boiled_eggcount)));
                break;
            }
            tablerow++;
        }
        while (tablerow <= 11);
    }
    showTotalFoodAmount();
}

void CustomerWindow::on_pushButton_Jerry_clicked()
{
    int price = 25;
    jerrycount++;

    if (jerrycount == 1)
    {
        int tablerow = ui->tableWidget->rowCount();
        if(tablerow <= 11)
        {
            ui->tableWidget->insertRow(ui->tableWidget->rowCount());
            int tablerow = ui->tableWidget->rowCount() - 1;

            ui->tableWidget->setItem(tablerow, 0, new QTableWidgetItem("Jerry"));
            ui->tableWidget->setItem(tablerow, 1, new QTableWidgetItem(QString::number(jerrycount)));
            ui->tableWidget->setItem(tablerow, 2, new QTableWidgetItem(QString::number(price)));
        }
        else
        {
           showOverflow();
        }
    }
    else
    {
        QString item;
        int tablerow = 0;
        do
        {
            int column = 0;
            QString item;
            item = ui->tableWidget->item(tablerow, column)->text();
            if (item == "Jerry")
            {
                ui->tableWidget->setItem(tablerow, 1, new QTableWidgetItem(QString::number(jerrycount)));
                ui->tableWidget->setItem(tablerow, 2, new QTableWidgetItem(QString::number(price * jerrycount)));
                break;
            }
            tablerow++;
        }
        while (tablerow <= 11);
    }
    showTotalFoodAmount();
}

void CustomerWindow::on_pushButton_Puri_Tarkari_clicked()
{
    int price = 25;
    puri_tarkaricount++;

    if (puri_tarkaricount == 1)
    {
        int tablerow = ui->tableWidget->rowCount();
        if(tablerow <= 11)
        {
            ui->tableWidget->insertRow(ui->tableWidget->rowCount());
            int tablerow = ui->tableWidget->rowCount() - 1;

            ui->tableWidget->setItem(tablerow, 0, new QTableWidgetItem("Puri Tarkari"));
            ui->tableWidget->setItem(tablerow, 1, new QTableWidgetItem(QString::number(puri_tarkaricount)));
            ui->tableWidget->setItem(tablerow, 2, new QTableWidgetItem(QString::number(price)));
        }
        else
        {
           showOverflow();
        }
    }
    else
    {
        QString item;
        int tablerow = 0;
        do
        {
            int column = 0;
            QString item;
            item = ui->tableWidget->item(tablerow, column)->text();
            if (item == "Puri Tarkari")
            {
                ui->tableWidget->setItem(tablerow, 1, new QTableWidgetItem(QString::number(puri_tarkaricount)));
                ui->tableWidget->setItem(tablerow, 2, new QTableWidgetItem(QString::number(price * puri_tarkaricount)));
                break;
            }
            tablerow++;
        }
        while (tablerow <= 11);
    }
    showTotalFoodAmount();
}

void CustomerWindow::on_pushButton_Bacon_clicked()
{
    int price = 25;
    baconcount++;

    if (baconcount == 1)
    {
        int tablerow = ui->tableWidget->rowCount();
        if(tablerow <= 11)
        {
            ui->tableWidget->insertRow(ui->tableWidget->rowCount());
            int tablerow = ui->tableWidget->rowCount() - 1;

            ui->tableWidget->setItem(tablerow, 0, new QTableWidgetItem("Bacon"));
            ui->tableWidget->setItem(tablerow, 1, new QTableWidgetItem(QString::number(baconcount)));
            ui->tableWidget->setItem(tablerow, 2, new QTableWidgetItem(QString::number(price)));
        }
        else
        {
           showOverflow();
        }
    }
    else
    {
        QString item;
        int tablerow = 0;
        do
        {
            int column = 0;
            QString item;
            item = ui->tableWidget->item(tablerow, column)->text();
            if (item == "Bacon")
            {
                ui->tableWidget->setItem(tablerow, 1, new QTableWidgetItem(QString::number(baconcount)));
                ui->tableWidget->setItem(tablerow, 2, new QTableWidgetItem(QString::number(price * baconcount)));
                break;
            }
            tablerow++;
        }
        while (tablerow <= 11);
    }
    showTotalFoodAmount();
}

void CustomerWindow::on_pushButton_Oatmeal_clicked()
{
    int price = 25;
    oatmealcount++;

    if (oatmealcount == 1)
    {
        int tablerow = ui->tableWidget->rowCount();
        if(tablerow <= 11)
        {
            ui->tableWidget->insertRow(ui->tableWidget->rowCount());
            int tablerow = ui->tableWidget->rowCount() - 1;

            ui->tableWidget->setItem(tablerow, 0, new QTableWidgetItem("Oatmeal"));
            ui->tableWidget->setItem(tablerow, 1, new QTableWidgetItem(QString::number(oatmealcount)));
            ui->tableWidget->setItem(tablerow, 2, new QTableWidgetItem(QString::number(price)));
        }
        else
        {
           showOverflow();
        }
    }
    else
    {
        QString item;
        int tablerow = 0;
        do
        {
            int column = 0;
            QString item;
            item = ui->tableWidget->item(tablerow, column)->text();
            if (item == "Oatmeal")
            {
                ui->tableWidget->setItem(tablerow, 1, new QTableWidgetItem(QString::number(oatmealcount)));
                ui->tableWidget->setItem(tablerow, 2, new QTableWidgetItem(QString::number(price * oatmealcount)));
                break;
            }
            tablerow++;
        }
        while (tablerow <= 11);
    }
    showTotalFoodAmount();
}

void CustomerWindow::on_pushButton_Pancake_clicked()
{
    int price = 25;
    pancakecount++;

    if (pancakecount == 1)
    {
        int tablerow = ui->tableWidget->rowCount();
        if(tablerow <= 11)
        {
            ui->tableWidget->insertRow(ui->tableWidget->rowCount());
            int tablerow = ui->tableWidget->rowCount() - 1;

            ui->tableWidget->setItem(tablerow, 0, new QTableWidgetItem("Pancake"));
            ui->tableWidget->setItem(tablerow, 1, new QTableWidgetItem(QString::number(pancakecount)));
            ui->tableWidget->setItem(tablerow, 2, new QTableWidgetItem(QString::number(price)));
        }
        else
        {
           showOverflow();
        }
    }
    else
    {
        QString item;
        int tablerow = 0;
        do
        {
            int column = 0;
            QString item;
            item = ui->tableWidget->item(tablerow, column)->text();
            if (item == "Pancake")
            {
                ui->tableWidget->setItem(tablerow, 1, new QTableWidgetItem(QString::number(pancakecount)));
                ui->tableWidget->setItem(tablerow, 2, new QTableWidgetItem(QString::number(price * pancakecount)));
                break;
            }
            tablerow++;
        }
        while (tablerow <= 11);
    }
    showTotalFoodAmount();
}

void CustomerWindow::on_pushButton_MoMo_clicked()
{
    int price = 25;
    momocount++;

    if (momocount == 1)
    {
        int tablerow = ui->tableWidget->rowCount();
        if(tablerow <= 11)
        {
            ui->tableWidget->insertRow(ui->tableWidget->rowCount());
            int tablerow = ui->tableWidget->rowCount() - 1;

            ui->tableWidget->setItem(tablerow, 0, new QTableWidgetItem("MoMo"));
            ui->tableWidget->setItem(tablerow, 1, new QTableWidgetItem(QString::number(momocount)));
            ui->tableWidget->setItem(tablerow, 2, new QTableWidgetItem(QString::number(price)));
        }
        else
        {
           showOverflow();
        }
    }
    else
    {
        QString item;
        int tablerow = 0;
        do
        {
            int column = 0;
            QString item;
            item = ui->tableWidget->item(tablerow, column)->text();
            if (item == "MoMo")
            {
                ui->tableWidget->setItem(tablerow, 1, new QTableWidgetItem(QString::number(momocount)));
                ui->tableWidget->setItem(tablerow, 2, new QTableWidgetItem(QString::number(price * momocount)));
                break;
            }
            tablerow++;
        }
        while (tablerow <= 11);
    }
    showTotalFoodAmount();
}

void CustomerWindow::on_pushButton_Chowmein_clicked()
{
    int price = 25;
    chowmeincount++;

    if (chowmeincount == 1)
    {
        int tablerow = ui->tableWidget->rowCount();
        if(tablerow <= 11)
        {
            ui->tableWidget->insertRow(ui->tableWidget->rowCount());
            int tablerow = ui->tableWidget->rowCount() - 1;

            ui->tableWidget->setItem(tablerow, 0, new QTableWidgetItem("Chowmein"));
            ui->tableWidget->setItem(tablerow, 1, new QTableWidgetItem(QString::number(chowmeincount)));
            ui->tableWidget->setItem(tablerow, 2, new QTableWidgetItem(QString::number(price)));
        }
        else
        {
           showOverflow();
        }
    }
    else
    {
        QString item;
        int tablerow = 0;
        do
        {
            int column = 0;
            QString item;
            item = ui->tableWidget->item(tablerow, column)->text();
            if (item == "Chowmein")
            {
                ui->tableWidget->setItem(tablerow, 1, new QTableWidgetItem(QString::number(chowmeincount)));
                ui->tableWidget->setItem(tablerow, 2, new QTableWidgetItem(QString::number(price * chowmeincount)));
                break;
            }
            tablerow++;
        }
        while (tablerow <= 11);
    }
    showTotalFoodAmount();
}

void CustomerWindow::on_pushButton_Chicken_Biryani_clicked()
{
    int price = 25;
    chicken_biryanicount++;

    if (chicken_biryanicount == 1)
    {
        int tablerow = ui->tableWidget->rowCount();
        if(tablerow <= 11)
        {
            ui->tableWidget->insertRow(ui->tableWidget->rowCount());
            int tablerow = ui->tableWidget->rowCount() - 1;

            ui->tableWidget->setItem(tablerow, 0, new QTableWidgetItem("Chicken Biryani"));
            ui->tableWidget->setItem(tablerow, 1, new QTableWidgetItem(QString::number(chicken_biryanicount)));
            ui->tableWidget->setItem(tablerow, 2, new QTableWidgetItem(QString::number(price)));
        }
        else
        {
           showOverflow();
        }
    }
    else
    {
        QString item;
        int tablerow = 0;
        do
        {
            int column = 0;
            QString item;
            item = ui->tableWidget->item(tablerow, column)->text();
            if (item == "Chicken Biryani")
            {
                ui->tableWidget->setItem(tablerow, 1, new QTableWidgetItem(QString::number(chicken_biryanicount)));
                ui->tableWidget->setItem(tablerow, 2, new QTableWidgetItem(QString::number(price * chicken_biryanicount)));
                break;
            }
            tablerow++;
        }
        while (tablerow <= 11);
    }
    showTotalFoodAmount();
}

void CustomerWindow::on_pushButton_Mutton_Biryani_clicked()
{
    int price = 25;
    mutton_biryanicount++;

    if (mutton_biryanicount == 1)
    {
        int tablerow = ui->tableWidget->rowCount();
        if(tablerow <= 11)
        {
            ui->tableWidget->insertRow(ui->tableWidget->rowCount());
            int tablerow = ui->tableWidget->rowCount() - 1;

            ui->tableWidget->setItem(tablerow, 0, new QTableWidgetItem("Mutton Biryani"));
            ui->tableWidget->setItem(tablerow, 1, new QTableWidgetItem(QString::number(mutton_biryanicount)));
            ui->tableWidget->setItem(tablerow, 2, new QTableWidgetItem(QString::number(price)));
        }
        else
        {
           showOverflow();
        }
    }
    else
    {
        QString item;
        int tablerow = 0;
        do
        {
            int column = 0;
            QString item;
            item = ui->tableWidget->item(tablerow, column)->text();
            if (item == "Mutton Biryani")
            {
                ui->tableWidget->setItem(tablerow, 1, new QTableWidgetItem(QString::number(mutton_biryanicount)));
                ui->tableWidget->setItem(tablerow, 2, new QTableWidgetItem(QString::number(price * mutton_biryanicount)));
                break;
            }
            tablerow++;
        }
        while (tablerow <= 11);
    }
    showTotalFoodAmount();
}

void CustomerWindow::on_pushButton_Red_Sauce_Pasta_clicked()
{
    int price = 25;
    red_sauce_pastacount++;

    if (red_sauce_pastacount == 1)
    {
        int tablerow = ui->tableWidget->rowCount();
        if(tablerow <= 11)
        {
            ui->tableWidget->insertRow(ui->tableWidget->rowCount());
            int tablerow = ui->tableWidget->rowCount() - 1;

            ui->tableWidget->setItem(tablerow, 0, new QTableWidgetItem("Red Sauce Pasta"));
            ui->tableWidget->setItem(tablerow, 1, new QTableWidgetItem(QString::number(red_sauce_pastacount)));
            ui->tableWidget->setItem(tablerow, 2, new QTableWidgetItem(QString::number(price)));
        }
        else
        {
           showOverflow();
        }
    }
    else
    {
        QString item;
        int tablerow = 0;
        do
        {
            int column = 0;
            QString item;
            item = ui->tableWidget->item(tablerow, column)->text();
            if (item == "Red Sauce Pata")
            {
                ui->tableWidget->setItem(tablerow, 1, new QTableWidgetItem(QString::number(red_sauce_pastacount)));
                ui->tableWidget->setItem(tablerow, 2, new QTableWidgetItem(QString::number(price * red_sauce_pastacount)));
                break;
            }
            tablerow++;
        }
        while (tablerow <= 11);
    }
    showTotalFoodAmount();
}

void CustomerWindow::on_pushButton_White_Sauce_Pasta_clicked()
{
    int price = 25;
    white_sauce_pastacount++;

    if (white_sauce_pastacount == 1)
    {
        int tablerow = ui->tableWidget->rowCount();
        if(tablerow <= 11)
        {
            ui->tableWidget->insertRow(ui->tableWidget->rowCount());
            int tablerow = ui->tableWidget->rowCount() - 1;

            ui->tableWidget->setItem(tablerow, 0, new QTableWidgetItem("White Sauce Pasta"));
            ui->tableWidget->setItem(tablerow, 1, new QTableWidgetItem(QString::number(white_sauce_pastacount)));
            ui->tableWidget->setItem(tablerow, 2, new QTableWidgetItem(QString::number(price)));
        }
        else
        {
           showOverflow();
        }
    }
    else
    {
        QString item;
        int tablerow = 0;
        do
        {
            int column = 0;
            QString item;
            item = ui->tableWidget->item(tablerow, column)->text();
            if (item == "White Sauce Pasta")
            {
                ui->tableWidget->setItem(tablerow, 1, new QTableWidgetItem(QString::number(white_sauce_pastacount)));
                ui->tableWidget->setItem(tablerow, 2, new QTableWidgetItem(QString::number(price * white_sauce_pastacount)));
                break;
            }
            tablerow++;
        }
        while (tablerow <= 11);
    }
    showTotalFoodAmount();
}

void CustomerWindow::on_pushButton_Spaghetti_clicked()
{
    int price = 25;
    spaghetticount++;

    if (spaghetticount == 1)
    {
        int tablerow = ui->tableWidget->rowCount();
        if(tablerow <= 11)
        {
            ui->tableWidget->insertRow(ui->tableWidget->rowCount());
            int tablerow = ui->tableWidget->rowCount() - 1;

            ui->tableWidget->setItem(tablerow, 0, new QTableWidgetItem("Spaghetti"));
            ui->tableWidget->setItem(tablerow, 1, new QTableWidgetItem(QString::number(spaghetticount)));
            ui->tableWidget->setItem(tablerow, 2, new QTableWidgetItem(QString::number(price)));
        }
        else
        {
           showOverflow();
        }
    }
    else
    {
        QString item;
        int tablerow = 0;
        do
        {
            int column = 0;
            QString item;
            item = ui->tableWidget->item(tablerow, column)->text();
            if (item == "Spaghetti")
            {
                ui->tableWidget->setItem(tablerow, 1, new QTableWidgetItem(QString::number(spaghetticount)));
                ui->tableWidget->setItem(tablerow, 2, new QTableWidgetItem(QString::number(price * spaghetticount)));
                break;
            }
            tablerow++;
        }
        while (tablerow <= 11);
    }
    showTotalFoodAmount();
}

void CustomerWindow::on_pushButton_Pizza_clicked()
{
    int price = 25;
    pizzacount++;

    if (pizzacount == 1)
    {
        int tablerow = ui->tableWidget->rowCount();
        if(tablerow <= 11)
        {
            ui->tableWidget->insertRow(ui->tableWidget->rowCount());
            int tablerow = ui->tableWidget->rowCount() - 1;

            ui->tableWidget->setItem(tablerow, 0, new QTableWidgetItem("Pizza"));
            ui->tableWidget->setItem(tablerow, 1, new QTableWidgetItem(QString::number(pizzacount)));
            ui->tableWidget->setItem(tablerow, 2, new QTableWidgetItem(QString::number(price)));
        }
        else
        {
           showOverflow();
        }
    }
    else
    {
        QString item;
        int tablerow = 0;
        do
        {
            int column = 0;
            QString item;
            item = ui->tableWidget->item(tablerow, column)->text();
            if (item == "Pizza")
            {
                ui->tableWidget->setItem(tablerow, 1, new QTableWidgetItem(QString::number(pizzacount)));
                ui->tableWidget->setItem(tablerow, 2, new QTableWidgetItem(QString::number(price * pizzacount)));
                break;
            }
            tablerow++;
        }
        while (tablerow <= 11);
    }
    showTotalFoodAmount();
}

void CustomerWindow::on_pushButton_Aloo_Chop_clicked()
{
    int price = 25;
    aloo_chopcount++;

    if (aloo_chopcount == 1)
    {
        int tablerow = ui->tableWidget->rowCount();
        if(tablerow <= 11)
        {
            ui->tableWidget->insertRow(ui->tableWidget->rowCount());
            int tablerow = ui->tableWidget->rowCount() - 1;

            ui->tableWidget->setItem(tablerow, 0, new QTableWidgetItem("Aloo Chop"));
            ui->tableWidget->setItem(tablerow, 1, new QTableWidgetItem(QString::number(aloo_chopcount)));
            ui->tableWidget->setItem(tablerow, 2, new QTableWidgetItem(QString::number(price)));
        }
        else
        {
           showOverflow();
        }
    }
    else
    {
        QString item;
        int tablerow = 0;
        do
        {
            int column = 0;
            QString item;
            item = ui->tableWidget->item(tablerow, column)->text();
            if (item == "Aloo Chop")
            {
                ui->tableWidget->setItem(tablerow, 1, new QTableWidgetItem(QString::number(aloo_chopcount)));
                ui->tableWidget->setItem(tablerow, 2, new QTableWidgetItem(QString::number(price * aloo_chopcount)));
                break;
            }
            tablerow++;
        }
        while (tablerow <= 11);
    }
    showTotalFoodAmount();
}

void CustomerWindow::on_pushButton_Naan_clicked()
{
    int price = 25;
    naancount++;

    if (naancount == 1)
    {
        int tablerow = ui->tableWidget->rowCount();
        if(tablerow <= 11)
        {
            ui->tableWidget->insertRow(ui->tableWidget->rowCount());
            int tablerow = ui->tableWidget->rowCount() - 1;

            ui->tableWidget->setItem(tablerow, 0, new QTableWidgetItem("Naan"));
            ui->tableWidget->setItem(tablerow, 1, new QTableWidgetItem(QString::number(naancount)));
            ui->tableWidget->setItem(tablerow, 2, new QTableWidgetItem(QString::number(price)));
        }
        else
        {
           showOverflow();
        }
    }
    else
    {
        QString item;
        int tablerow = 0;
        do
        {
            int column = 0;
            QString item;
            item = ui->tableWidget->item(tablerow, column)->text();
            if (item == "Naan")
            {
                ui->tableWidget->setItem(tablerow, 1, new QTableWidgetItem(QString::number(naancount)));
                ui->tableWidget->setItem(tablerow, 2, new QTableWidgetItem(QString::number(price * naancount)));
                break;
            }
            tablerow++;
        }
        while (tablerow <= 11);
    }
    showTotalFoodAmount();
}

void CustomerWindow::on_pushButton_Chat_clicked()
{
    int price = 25;
    chatcount++;

    if (chatcount == 1)
    {
        int tablerow = ui->tableWidget->rowCount();
        if(tablerow <= 11)
        {
            ui->tableWidget->insertRow(ui->tableWidget->rowCount());
            int tablerow = ui->tableWidget->rowCount() - 1;

            ui->tableWidget->setItem(tablerow, 0, new QTableWidgetItem("Chat"));
            ui->tableWidget->setItem(tablerow, 1, new QTableWidgetItem(QString::number(chatcount)));
            ui->tableWidget->setItem(tablerow, 2, new QTableWidgetItem(QString::number(price)));
        }
        else
        {
           showOverflow();
        }
    }
    else
    {
        QString item;
        int tablerow = 0;
        do
        {
            int column = 0;
            QString item;
            item = ui->tableWidget->item(tablerow, column)->text();
            if (item == "Chat")
            {
                ui->tableWidget->setItem(tablerow, 1, new QTableWidgetItem(QString::number(chatcount)));
                ui->tableWidget->setItem(tablerow, 2, new QTableWidgetItem(QString::number(price * chatcount)));
                break;
            }
            tablerow++;
        }
        while (tablerow <= 11);
    }
    showTotalFoodAmount();
}

void CustomerWindow::on_pushButton_Sausage_clicked()
{
    int price = 25;
    sausagecount++;

    if (sausagecount == 1)
    {
        int tablerow = ui->tableWidget->rowCount();
        if(tablerow <= 11)
        {
            ui->tableWidget->insertRow(ui->tableWidget->rowCount());
            int tablerow = ui->tableWidget->rowCount() - 1;

            ui->tableWidget->setItem(tablerow, 0, new QTableWidgetItem("Sausage"));
            ui->tableWidget->setItem(tablerow, 1, new QTableWidgetItem(QString::number(sausagecount)));
            ui->tableWidget->setItem(tablerow, 2, new QTableWidgetItem(QString::number(price)));
        }
        else
        {
           showOverflow();
        }
    }
    else
    {
        QString item;
        int tablerow = 0;
        do
        {
            int column = 0;
            QString item;
            item = ui->tableWidget->item(tablerow, column)->text();
            if (item == "Sausage")
            {
                ui->tableWidget->setItem(tablerow, 1, new QTableWidgetItem(QString::number(sausagecount)));
                ui->tableWidget->setItem(tablerow, 2, new QTableWidgetItem(QString::number(price * sausagecount)));
                break;
            }
            tablerow++;
        }
        while (tablerow <= 11);
    }
    showTotalFoodAmount();
}

void CustomerWindow::on_pushButton_Rice_Set_clicked()
{
    int price = 25;
    rice_setcount++;

    if (rice_setcount == 1)
    {
        int tablerow = ui->tableWidget->rowCount();
        if(tablerow <= 11)
        {
            ui->tableWidget->insertRow(ui->tableWidget->rowCount());
            int tablerow = ui->tableWidget->rowCount() - 1;

            ui->tableWidget->setItem(tablerow, 0, new QTableWidgetItem("Rice Set"));
            ui->tableWidget->setItem(tablerow, 1, new QTableWidgetItem(QString::number(rice_setcount)));
            ui->tableWidget->setItem(tablerow, 2, new QTableWidgetItem(QString::number(price)));
        }
        else
        {
           showOverflow();
        }
    }
    else
    {
        QString item;
        int tablerow = 0;
        do
        {
            int column = 0;
            QString item;
            item = ui->tableWidget->item(tablerow, column)->text();
            if (item == "Rice Set")
            {
                ui->tableWidget->setItem(tablerow, 1, new QTableWidgetItem(QString::number(rice_setcount)));
                ui->tableWidget->setItem(tablerow, 2, new QTableWidgetItem(QString::number(price * rice_setcount)));
                break;
            }
            tablerow++;
        }
        while (tablerow <= 11);
    }
    showTotalFoodAmount();
}

void CustomerWindow::on_pushButton_Roti_clicked()
{
    int price = 25;
    roticount++;

    if (roticount == 1)
    {
        int tablerow = ui->tableWidget->rowCount();
        if(tablerow <= 11)
        {
            ui->tableWidget->insertRow(ui->tableWidget->rowCount());
            int tablerow = ui->tableWidget->rowCount() - 1;

            ui->tableWidget->setItem(tablerow, 0, new QTableWidgetItem("Roti"));
            ui->tableWidget->setItem(tablerow, 1, new QTableWidgetItem(QString::number(roticount)));
            ui->tableWidget->setItem(tablerow, 2, new QTableWidgetItem(QString::number(price)));
        }
        else
        {
           showOverflow();
        }
    }
    else
    {
        QString item;
        int tablerow = 0;
        do
        {
            int column = 0;
            QString item;
            item = ui->tableWidget->item(tablerow, column)->text();
            if (item == "Roti")
            {
                ui->tableWidget->setItem(tablerow, 1, new QTableWidgetItem(QString::number(roticount)));
                ui->tableWidget->setItem(tablerow, 2, new QTableWidgetItem(QString::number(price * roticount)));
                break;
            }
            tablerow++;
        }
        while (tablerow <= 11);
    }
    showTotalFoodAmount();
}

void CustomerWindow::on_pushButton_Chicken_Curry_clicked()
{
    int price = 25;
    chicken_currycount++;

    if (chicken_currycount == 1)
    {
        int tablerow = ui->tableWidget->rowCount();
        if(tablerow <= 11)
        {
            ui->tableWidget->insertRow(ui->tableWidget->rowCount());
            int tablerow = ui->tableWidget->rowCount() - 1;

            ui->tableWidget->setItem(tablerow, 0, new QTableWidgetItem("Chicken Curry"));
            ui->tableWidget->setItem(tablerow, 1, new QTableWidgetItem(QString::number(chicken_currycount)));
            ui->tableWidget->setItem(tablerow, 2, new QTableWidgetItem(QString::number(price)));
        }
        else
        {
           showOverflow();
        }
    }
    else
    {
        QString item;
        int tablerow = 0;
        do
        {
            int column = 0;
            QString item;
            item = ui->tableWidget->item(tablerow, column)->text();
            if (item == "Chicken Curry")
            {
                ui->tableWidget->setItem(tablerow, 1, new QTableWidgetItem(QString::number(chicken_currycount)));
                ui->tableWidget->setItem(tablerow, 2, new QTableWidgetItem(QString::number(price * chicken_currycount)));
                break;
            }
            tablerow++;
        }
        while (tablerow <= 11);
    }
    showTotalFoodAmount();
}

void CustomerWindow::on_pushButton_Mutton_Curry_clicked()
{
    int price = 25;
    mutton_currycount++;

    if (mutton_currycount == 1)
    {
        int tablerow = ui->tableWidget->rowCount();
        if(tablerow <= 11)
        {
            ui->tableWidget->insertRow(ui->tableWidget->rowCount());
            int tablerow = ui->tableWidget->rowCount() - 1;

            ui->tableWidget->setItem(tablerow, 0, new QTableWidgetItem("Mutton Curry"));
            ui->tableWidget->setItem(tablerow, 1, new QTableWidgetItem(QString::number(mutton_currycount)));
            ui->tableWidget->setItem(tablerow, 2, new QTableWidgetItem(QString::number(price)));
        }
        else
        {
           showOverflow();
        }
    }
    else
    {
        QString item;
        int tablerow = 0;
        do
        {
            int column = 0;
            QString item;
            item = ui->tableWidget->item(tablerow, column)->text();
            if (item == "Mutton Curry")
            {
                ui->tableWidget->setItem(tablerow, 1, new QTableWidgetItem(QString::number(mutton_currycount)));
                ui->tableWidget->setItem(tablerow, 2, new QTableWidgetItem(QString::number(price * mutton_currycount)));
                break;
            }
            tablerow++;
        }
        while (tablerow <= 11);
    }
    showTotalFoodAmount();
}

void CustomerWindow::on_pushButton_Egg_Curry_clicked()
{
    int price = 25;
    egg_currycount++;

    if (egg_currycount == 1)
    {
        int tablerow = ui->tableWidget->rowCount();
        if(tablerow <= 11)
        {
            ui->tableWidget->insertRow(ui->tableWidget->rowCount());
            int tablerow = ui->tableWidget->rowCount() - 1;

            ui->tableWidget->setItem(tablerow, 0, new QTableWidgetItem("Egg Curry"));
            ui->tableWidget->setItem(tablerow, 1, new QTableWidgetItem(QString::number(egg_currycount)));
            ui->tableWidget->setItem(tablerow, 2, new QTableWidgetItem(QString::number(price)));
        }
        else
        {
           showOverflow();
        }
    }
    else
    {
        QString item;
        int tablerow = 0;
        do
        {
            int column = 0;
            QString item;
            item = ui->tableWidget->item(tablerow, column)->text();
            if (item == "Egg Curry")
            {
                ui->tableWidget->setItem(tablerow, 1, new QTableWidgetItem(QString::number(egg_currycount)));
                ui->tableWidget->setItem(tablerow, 2, new QTableWidgetItem(QString::number(price * egg_currycount)));
                break;
            }
            tablerow++;
        }
        while (tablerow <= 11);
    }
    showTotalFoodAmount();
}

void CustomerWindow::on_pushButton_Chicken_Roast_clicked()
{
    int price = 25;
    chicken_roastcount++;

    if (chicken_roastcount == 1)
    {
        int tablerow = ui->tableWidget->rowCount();
        if(tablerow <= 11)
        {
            ui->tableWidget->insertRow(ui->tableWidget->rowCount());
            int tablerow = ui->tableWidget->rowCount() - 1;

            ui->tableWidget->setItem(tablerow, 0, new QTableWidgetItem("Chicken Roast"));
            ui->tableWidget->setItem(tablerow, 1, new QTableWidgetItem(QString::number(chicken_roastcount)));
            ui->tableWidget->setItem(tablerow, 2, new QTableWidgetItem(QString::number(price)));
        }
        else
        {
           showOverflow();
        }
    }
    else
    {
        QString item;
        int tablerow = 0;
        do
        {
            int column = 0;
            QString item;
            item = ui->tableWidget->item(tablerow, column)->text();
            if (item == "Chicken Roast")
            {
                ui->tableWidget->setItem(tablerow, 1, new QTableWidgetItem(QString::number(chicken_roastcount)));
                ui->tableWidget->setItem(tablerow, 2, new QTableWidgetItem(QString::number(price * chicken_roastcount)));
                break;
            }
            tablerow++;
        }
        while (tablerow <= 11);
    }
    showTotalFoodAmount();
}

void CustomerWindow::on_pushButton_Fish_Fry_clicked()
{
    int price = 25;
    fish_frycount++;

    if (fish_frycount == 1)
    {
        int tablerow = ui->tableWidget->rowCount();
        if(tablerow <= 11)
        {
            ui->tableWidget->insertRow(ui->tableWidget->rowCount());
            int tablerow = ui->tableWidget->rowCount() - 1;

            ui->tableWidget->setItem(tablerow, 0, new QTableWidgetItem("Fish Fry"));
            ui->tableWidget->setItem(tablerow, 1, new QTableWidgetItem(QString::number(fish_frycount)));
            ui->tableWidget->setItem(tablerow, 2, new QTableWidgetItem(QString::number(price)));
        }
        else
        {
           showOverflow();
        }
    }
    else
    {
        QString item;
        int tablerow = 0;
        do
        {
            int column = 0;
            QString item;
            item = ui->tableWidget->item(tablerow, column)->text();
            if (item == "Fish Fry")
            {
                ui->tableWidget->setItem(tablerow, 1, new QTableWidgetItem(QString::number(fish_frycount)));
                ui->tableWidget->setItem(tablerow, 2, new QTableWidgetItem(QString::number(price * fish_frycount)));
                break;
            }
            tablerow++;
        }
        while (tablerow <= 11);
    }
    showTotalFoodAmount();
}

void CustomerWindow::on_pushButton_Paneer_Chilli_clicked()
{
    int price = 25;
    paneer_chillicount++;

    if (paneer_chillicount == 1)
    {
        int tablerow = ui->tableWidget->rowCount();
        if(tablerow <= 11)
        {
            ui->tableWidget->insertRow(ui->tableWidget->rowCount());
            int tablerow = ui->tableWidget->rowCount() - 1;

            ui->tableWidget->setItem(tablerow, 0, new QTableWidgetItem("Paneer Chilli"));
            ui->tableWidget->setItem(tablerow, 1, new QTableWidgetItem(QString::number(paneer_chillicount)));
            ui->tableWidget->setItem(tablerow, 2, new QTableWidgetItem(QString::number(price)));
        }
        else
        {
           showOverflow();
        }
    }
    else
    {
        QString item;
        int tablerow = 0;
        do
        {
            int column = 0;
            QString item;
            item = ui->tableWidget->item(tablerow, column)->text();
            if (item == "Paneer Chilli")
            {
                ui->tableWidget->setItem(tablerow, 1, new QTableWidgetItem(QString::number(paneer_chillicount)));
                ui->tableWidget->setItem(tablerow, 2, new QTableWidgetItem(QString::number(price * paneer_chillicount)));
                break;
            }
            tablerow++;
        }
        while (tablerow <= 11);
    }
    showTotalFoodAmount();
}

void CustomerWindow::on_pushButton_Daal_Fry_clicked()
{
    int price = 25;
    daal_frycount++;

    if (daal_frycount == 1)
    {
        int tablerow = ui->tableWidget->rowCount();
        if(tablerow <= 11)
        {
            ui->tableWidget->insertRow(ui->tableWidget->rowCount());
            int tablerow = ui->tableWidget->rowCount() - 1;

            ui->tableWidget->setItem(tablerow, 0, new QTableWidgetItem("Daal Fry"));
            ui->tableWidget->setItem(tablerow, 1, new QTableWidgetItem(QString::number(daal_frycount)));
            ui->tableWidget->setItem(tablerow, 2, new QTableWidgetItem(QString::number(price)));
        }
        else
        {
           showOverflow();
        }
    }
    else
    {
        QString item;
        int tablerow = 0;
        do
        {
            int column = 0;
            QString item;
            item = ui->tableWidget->item(tablerow, column)->text();
            if (item == "Daal Fry")
            {
                ui->tableWidget->setItem(tablerow, 1, new QTableWidgetItem(QString::number(daal_frycount)));
                ui->tableWidget->setItem(tablerow, 2, new QTableWidgetItem(QString::number(price * daal_frycount)));
                break;
            }
            tablerow++;
        }
        while (tablerow <= 11);
    }
    showTotalFoodAmount();
}

void CustomerWindow::on_pushButton_Chana_Masala_clicked()
{
    int price = 25;
    chana_masalacount++;

    if (chana_masalacount == 1)
    {
        int tablerow = ui->tableWidget->rowCount();
        if(tablerow <= 11)
        {
            ui->tableWidget->insertRow(ui->tableWidget->rowCount());
            int tablerow = ui->tableWidget->rowCount() - 1;

            ui->tableWidget->setItem(tablerow, 0, new QTableWidgetItem("Chana Masala"));
            ui->tableWidget->setItem(tablerow, 1, new QTableWidgetItem(QString::number(chana_masalacount)));
            ui->tableWidget->setItem(tablerow, 2, new QTableWidgetItem(QString::number(price)));
        }
        else
        {
           showOverflow();
        }
    }
    else
    {
        QString item;
        int tablerow = 0;
        do
        {
            int column = 0;
            QString item;
            item = ui->tableWidget->item(tablerow, column)->text();
            if (item == "Chana Masala")
            {
                ui->tableWidget->setItem(tablerow, 1, new QTableWidgetItem(QString::number(chana_masalacount)));
                ui->tableWidget->setItem(tablerow, 2, new QTableWidgetItem(QString::number(price * chana_masalacount)));
                break;
            }
            tablerow++;
        }
        while (tablerow <= 11);
    }
    showTotalFoodAmount();
}

void CustomerWindow::on_pushButton_Rajma_Masala_clicked()
{
    int price = 25;
    rajma_masalacount++;

    if (rajma_masalacount == 1)
    {
        int tablerow = ui->tableWidget->rowCount();
        if(tablerow <= 11)
        {
            ui->tableWidget->insertRow(ui->tableWidget->rowCount());
            int tablerow = ui->tableWidget->rowCount() - 1;

            ui->tableWidget->setItem(tablerow, 0, new QTableWidgetItem("Rajma Masala"));
            ui->tableWidget->setItem(tablerow, 1, new QTableWidgetItem(QString::number(rajma_masalacount)));
            ui->tableWidget->setItem(tablerow, 2, new QTableWidgetItem(QString::number(price)));
        }
        else
        {
           showOverflow();
        }
    }
    else
    {
        QString item;
        int tablerow = 0;
        do
        {
            int column = 0;
            QString item;
            item = ui->tableWidget->item(tablerow, column)->text();
            if (item == "Rajma Masala")
            {
                ui->tableWidget->setItem(tablerow, 1, new QTableWidgetItem(QString::number(rajma_masalacount)));
                ui->tableWidget->setItem(tablerow, 2, new QTableWidgetItem(QString::number(price * rajma_masalacount)));
                break;
            }
            tablerow++;
        }
        while (tablerow <= 11);
    }
    showTotalFoodAmount();
}

void CustomerWindow::on_pushButton_Papad_clicked()
{
    int price = 25;
    papadcount++;

    if (papadcount == 1)
    {
        int tablerow = ui->tableWidget->rowCount();
        if(tablerow <= 11)
        {
            ui->tableWidget->insertRow(ui->tableWidget->rowCount());
            int tablerow = ui->tableWidget->rowCount() - 1;

            ui->tableWidget->setItem(tablerow, 0, new QTableWidgetItem("Papad"));
            ui->tableWidget->setItem(tablerow, 1, new QTableWidgetItem(QString::number(papadcount)));
            ui->tableWidget->setItem(tablerow, 2, new QTableWidgetItem(QString::number(price)));
        }
        else
        {
           showOverflow();
        }
    }
    else
    {
        QString item;
        int tablerow = 0;
        do
        {
            int column = 0;
            QString item;
            item = ui->tableWidget->item(tablerow, column)->text();
            if (item == "Papad")
            {
                ui->tableWidget->setItem(tablerow, 1, new QTableWidgetItem(QString::number(papadcount)));
                ui->tableWidget->setItem(tablerow, 2, new QTableWidgetItem(QString::number(price * papadcount)));
                break;
            }
            tablerow++;
        }
        while (tablerow <= 11);
    }
    showTotalFoodAmount();
}

void CustomerWindow::on_pushButton_Black_Tea_clicked()
{
    int price = 25;
    black_teacount++;

    if (black_teacount == 1)
    {
        int tablerow = ui->tableWidget->rowCount();
        if(tablerow <= 11)
        {
            ui->tableWidget->insertRow(ui->tableWidget->rowCount());
            int tablerow = ui->tableWidget->rowCount() - 1;

            ui->tableWidget->setItem(tablerow, 0, new QTableWidgetItem("Black Tea"));
            ui->tableWidget->setItem(tablerow, 1, new QTableWidgetItem(QString::number(black_teacount)));
            ui->tableWidget->setItem(tablerow, 2, new QTableWidgetItem(QString::number(price)));
        }
        else
        {
           showOverflow();
        }
    }
    else
    {
        QString item;
        int tablerow = 0;
        do
        {
            int column = 0;
            QString item;
            item = ui->tableWidget->item(tablerow, column)->text();
            if (item == "Black Tea")
            {
                ui->tableWidget->setItem(tablerow, 1, new QTableWidgetItem(QString::number(black_teacount)));
                ui->tableWidget->setItem(tablerow, 2, new QTableWidgetItem(QString::number(price * black_teacount)));
                break;
            }
            tablerow++;
        }
        while (tablerow <= 11);
    }
    showTotalFoodAmount();
}

void CustomerWindow::on_pushButton_Black_Coffee_clicked()
{
    int price = 25;
    black_coffeecount++;

    if (black_coffeecount == 1)
    {
        int tablerow = ui->tableWidget->rowCount();
        if(tablerow <= 11)
        {
            ui->tableWidget->insertRow(ui->tableWidget->rowCount());
            int tablerow = ui->tableWidget->rowCount() - 1;

            ui->tableWidget->setItem(tablerow, 0, new QTableWidgetItem("Black Coffee"));
            ui->tableWidget->setItem(tablerow, 1, new QTableWidgetItem(QString::number(black_coffeecount)));
            ui->tableWidget->setItem(tablerow, 2, new QTableWidgetItem(QString::number(price)));
        }
        else
        {
           showOverflow();
        }
    }
    else
    {
        QString item;
        int tablerow = 0;
        do
        {
            int column = 0;
            QString item;
            item = ui->tableWidget->item(tablerow, column)->text();
            if (item == "Black Coffee")
            {
                ui->tableWidget->setItem(tablerow, 1, new QTableWidgetItem(QString::number(black_coffeecount)));
                ui->tableWidget->setItem(tablerow, 2, new QTableWidgetItem(QString::number(price * black_coffeecount)));
                break;
            }
            tablerow++;
        }
        while (tablerow <= 11);
    }
    showTotalFoodAmount();
}

void CustomerWindow::on_pushButton_Milk_Tea_clicked()
{
    int price = 25;
    milk_teacount++;

    if (milk_teacount == 1)
    {
        int tablerow = ui->tableWidget->rowCount();
        if(tablerow <= 11)
        {
            ui->tableWidget->insertRow(ui->tableWidget->rowCount());
            int tablerow = ui->tableWidget->rowCount() - 1;

            ui->tableWidget->setItem(tablerow, 0, new QTableWidgetItem("Milk Tea"));
            ui->tableWidget->setItem(tablerow, 1, new QTableWidgetItem(QString::number(milk_teacount)));
            ui->tableWidget->setItem(tablerow, 2, new QTableWidgetItem(QString::number(price)));
        }
        else
        {
           showOverflow();
        }
    }
    else
    {
        QString item;
        int tablerow = 0;
        do
        {
            int column = 0;
            QString item;
            item = ui->tableWidget->item(tablerow, column)->text();
            if (item == "Milk Tea")
            {
                ui->tableWidget->setItem(tablerow, 1, new QTableWidgetItem(QString::number(milk_teacount)));
                ui->tableWidget->setItem(tablerow, 2, new QTableWidgetItem(QString::number(price * milk_teacount)));
                break;
            }
            tablerow++;
        }
        while (tablerow <= 11);
    }
    showTotalFoodAmount();
}

void CustomerWindow::on_pushButton_Milk_Coffee_clicked()
{
    int price = 25;
    milk_coffeecount++;

    if (milk_coffeecount == 1)
    {
        int tablerow = ui->tableWidget->rowCount();
        if(tablerow <= 11)
        {
            ui->tableWidget->insertRow(ui->tableWidget->rowCount());
            int tablerow = ui->tableWidget->rowCount() - 1;

            ui->tableWidget->setItem(tablerow, 0, new QTableWidgetItem("Milk Coffee"));
            ui->tableWidget->setItem(tablerow, 1, new QTableWidgetItem(QString::number(milk_coffeecount)));
            ui->tableWidget->setItem(tablerow, 2, new QTableWidgetItem(QString::number(price)));
        }
        else
        {
           showOverflow();
        }
    }
    else
    {
        QString item;
        int tablerow = 0;
        do
        {
            int column = 0;
            QString item;
            item = ui->tableWidget->item(tablerow, column)->text();
            if (item == "Milk Coffee")
            {
                ui->tableWidget->setItem(tablerow, 1, new QTableWidgetItem(QString::number(milk_coffeecount)));
                ui->tableWidget->setItem(tablerow, 2, new QTableWidgetItem(QString::number(price * milk_coffeecount)));
                break;
            }
            tablerow++;
        }
        while (tablerow <= 11);
    }
    showTotalFoodAmount();
}

void CustomerWindow::on_pushButton_Pepsi_clicked()
{
    int price = 25;
    pepsicount++;

    if (pepsicount == 1)
    {
        int tablerow = ui->tableWidget->rowCount();
        if(tablerow <= 11)
        {
            ui->tableWidget->insertRow(ui->tableWidget->rowCount());
            int tablerow = ui->tableWidget->rowCount() - 1;

            ui->tableWidget->setItem(tablerow, 0, new QTableWidgetItem("Pepsi"));
            ui->tableWidget->setItem(tablerow, 1, new QTableWidgetItem(QString::number(pepsicount)));
            ui->tableWidget->setItem(tablerow, 2, new QTableWidgetItem(QString::number(price)));
        }
        else
        {
           showOverflow();
        }
    }
    else
    {
        QString item;
        int tablerow = 0;
        do
        {
            int column = 0;
            QString item;
            item = ui->tableWidget->item(tablerow, column)->text();
            if (item == "Pepsi")
            {
                ui->tableWidget->setItem(tablerow, 1, new QTableWidgetItem(QString::number(pepsicount)));
                ui->tableWidget->setItem(tablerow, 2, new QTableWidgetItem(QString::number(price * pepsicount)));
                break;
            }
            tablerow++;
        }
        while (tablerow <= 11);
    }
    showTotalFoodAmount();
}

void CustomerWindow::on_pushButton_Fanta_clicked()
{
    int price = 25;
    fantacount++;

    if (fantacount == 1)
    {
        int tablerow = ui->tableWidget->rowCount();
        if(tablerow <= 11)
        {
            ui->tableWidget->insertRow(ui->tableWidget->rowCount());
            int tablerow = ui->tableWidget->rowCount() - 1;

            ui->tableWidget->setItem(tablerow, 0, new QTableWidgetItem("Fanta"));
            ui->tableWidget->setItem(tablerow, 1, new QTableWidgetItem(QString::number(fantacount)));
            ui->tableWidget->setItem(tablerow, 2, new QTableWidgetItem(QString::number(price)));
        }
        else
        {
           showOverflow();
        }
    }
    else
    {
        QString item;
        int tablerow = 0;
        do
        {
            int column = 0;
            QString item;
            item = ui->tableWidget->item(tablerow, column)->text();
            if (item == "Fanta")
            {
                ui->tableWidget->setItem(tablerow, 1, new QTableWidgetItem(QString::number(fantacount)));
                ui->tableWidget->setItem(tablerow, 2, new QTableWidgetItem(QString::number(price * fantacount)));
                break;
            }
            tablerow++;
        }
        while (tablerow <= 11);
    }
    showTotalFoodAmount();
}

void CustomerWindow::on_pushButton_Sprite_clicked()
{
    int price = 25;
    spritecount++;

    if (spritecount == 1)
    {
        int tablerow = ui->tableWidget->rowCount();
        if(tablerow <= 11)
        {
            ui->tableWidget->insertRow(ui->tableWidget->rowCount());
            int tablerow = ui->tableWidget->rowCount() - 1;

            ui->tableWidget->setItem(tablerow, 0, new QTableWidgetItem("Sprite"));
            ui->tableWidget->setItem(tablerow, 1, new QTableWidgetItem(QString::number(spritecount)));
            ui->tableWidget->setItem(tablerow, 2, new QTableWidgetItem(QString::number(price)));
        }
        else
        {
           showOverflow();
        }
    }
    else
    {
        QString item;
        int tablerow = 0;
        do
        {
            int column = 0;
            QString item;
            item = ui->tableWidget->item(tablerow, column)->text();
            if (item == "Sprite")
            {
                ui->tableWidget->setItem(tablerow, 1, new QTableWidgetItem(QString::number(spritecount)));
                ui->tableWidget->setItem(tablerow, 2, new QTableWidgetItem(QString::number(price * spritecount)));
                break;
            }
            tablerow++;
        }
        while (tablerow <= 11);
    }
    showTotalFoodAmount();
}

void CustomerWindow::on_pushButton_Orange_Juice_clicked()
{
    int price = 25;
    orange_juicecount++;

    if (orange_juicecount == 1)
    {
        int tablerow = ui->tableWidget->rowCount();
        if(tablerow <= 11)
        {
            ui->tableWidget->insertRow(ui->tableWidget->rowCount());
            int tablerow = ui->tableWidget->rowCount() - 1;

            ui->tableWidget->setItem(tablerow, 0, new QTableWidgetItem("Orange Juice"));
            ui->tableWidget->setItem(tablerow, 1, new QTableWidgetItem(QString::number(orange_juicecount)));
            ui->tableWidget->setItem(tablerow, 2, new QTableWidgetItem(QString::number(price)));
        }
        else
        {
           showOverflow();
        }
    }
    else
    {
        QString item;
        int tablerow = 0;
        do
        {
            int column = 0;
            QString item;
            item = ui->tableWidget->item(tablerow, column)->text();
            if (item == "Orange Juice")
            {
                ui->tableWidget->setItem(tablerow, 1, new QTableWidgetItem(QString::number(orange_juicecount)));
                ui->tableWidget->setItem(tablerow, 2, new QTableWidgetItem(QString::number(price * orange_juicecount)));
                break;
            }
            tablerow++;
        }
        while (tablerow <= 11);
    }
    showTotalFoodAmount();
}

void CustomerWindow::on_pushButton_Redbull_clicked()
{
    int price = 25;
    redbullcount++;

    if (redbullcount == 1)
    {
        int tablerow = ui->tableWidget->rowCount();
        if(tablerow <= 11)
        {
            ui->tableWidget->insertRow(ui->tableWidget->rowCount());
            int tablerow = ui->tableWidget->rowCount() - 1;

            ui->tableWidget->setItem(tablerow, 0, new QTableWidgetItem("RedBull"));
            ui->tableWidget->setItem(tablerow, 1, new QTableWidgetItem(QString::number(redbullcount)));
            ui->tableWidget->setItem(tablerow, 2, new QTableWidgetItem(QString::number(price)));
        }
        else
        {
           showOverflow();
        }
    }
    else
    {
        QString item;
        int tablerow = 0;
        do
        {
            int column = 0;
            QString item;
            item = ui->tableWidget->item(tablerow, column)->text();
            if (item == "RedBull")
            {
                ui->tableWidget->setItem(tablerow, 1, new QTableWidgetItem(QString::number(redbullcount)));
                ui->tableWidget->setItem(tablerow, 2, new QTableWidgetItem(QString::number(price * redbullcount)));
                break;
            }
            tablerow++;
        }
        while (tablerow <= 11);
    }
    showTotalFoodAmount();
}

void CustomerWindow::on_pushButton_Hot_Lemon_clicked()
{
    int price = 25;
    hot_lemoncount++;

    if (hot_lemoncount == 1)
    {
        int tablerow = ui->tableWidget->rowCount();
        if(tablerow <= 11)
        {
            ui->tableWidget->insertRow(ui->tableWidget->rowCount());
            int tablerow = ui->tableWidget->rowCount() - 1;

            ui->tableWidget->setItem(tablerow, 0, new QTableWidgetItem("Hot Lemon"));
            ui->tableWidget->setItem(tablerow, 1, new QTableWidgetItem(QString::number(hot_lemoncount)));
            ui->tableWidget->setItem(tablerow, 2, new QTableWidgetItem(QString::number(price)));
        }
        else
        {
           showOverflow();
        }
    }
    else
    {
        QString item;
        int tablerow = 0;
        do
        {
            int column = 0;
            QString item;
            item = ui->tableWidget->item(tablerow, column)->text();
            if (item == "Hot Lemon")
            {
                ui->tableWidget->setItem(tablerow, 1, new QTableWidgetItem(QString::number(hot_lemoncount)));
                ui->tableWidget->setItem(tablerow, 2, new QTableWidgetItem(QString::number(price * hot_lemoncount)));
                break;
            }
            tablerow++;
        }
        while (tablerow <= 11);
    }
    showTotalFoodAmount();
}

void CustomerWindow::on_pushButton_Milkshake_clicked()
{
    int price = 25;
    milkshakecount++;

    if (milkshakecount == 1)
    {
        int tablerow = ui->tableWidget->rowCount();
        if(tablerow <= 11)
        {
            ui->tableWidget->insertRow(ui->tableWidget->rowCount());
            int tablerow = ui->tableWidget->rowCount() - 1;

            ui->tableWidget->setItem(tablerow, 0, new QTableWidgetItem("Milk Shake"));
            ui->tableWidget->setItem(tablerow, 1, new QTableWidgetItem(QString::number(milkshakecount)));
            ui->tableWidget->setItem(tablerow, 2, new QTableWidgetItem(QString::number(price)));
        }
        else
        {
           showOverflow();
        }
    }
    else
    {
        QString item;
        int tablerow = 0;
        do
        {
            int column = 0;
            QString item;
            item = ui->tableWidget->item(tablerow, column)->text();
            if (item == "Milk Shake")
            {
                ui->tableWidget->setItem(tablerow, 1, new QTableWidgetItem(QString::number(milkshakecount)));
                ui->tableWidget->setItem(tablerow, 2, new QTableWidgetItem(QString::number(price * milkshakecount)));
                break;
            }
            tablerow++;
        }
        while (tablerow <= 11);
    }
    showTotalFoodAmount();
}

void CustomerWindow::on_pushButton_Hot_Chocolate_clicked()
{
    int price = 25;
    hot_chocolatecount++;

    if (hot_chocolatecount == 1)
    {
        int tablerow = ui->tableWidget->rowCount();
        if(tablerow <= 11)
        {
            ui->tableWidget->insertRow(ui->tableWidget->rowCount());
            int tablerow = ui->tableWidget->rowCount() - 1;

            ui->tableWidget->setItem(tablerow, 0, new QTableWidgetItem("Hot Chocolate"));
            ui->tableWidget->setItem(tablerow, 1, new QTableWidgetItem(QString::number(hot_chocolatecount)));
            ui->tableWidget->setItem(tablerow, 2, new QTableWidgetItem(QString::number(price)));
        }
        else
        {
           showOverflow();
        }
    }
    else
    {
        QString item;
        int tablerow = 0;
        do
        {
            int column = 0;
            QString item;
            item = ui->tableWidget->item(tablerow, column)->text();
            if (item == "Hot Chocolate")
            {
                ui->tableWidget->setItem(tablerow, 1, new QTableWidgetItem(QString::number(hot_chocolatecount)));
                ui->tableWidget->setItem(tablerow, 2, new QTableWidgetItem(QString::number(price * hot_chocolatecount)));
                break;
            }
            tablerow++;
        }
        while (tablerow <= 11);
    }
    showTotalFoodAmount();
}

void CustomerWindow::updateBalance(int foodamount, QString UserType, int UserBalance, int UserID)
{

    MainWindow connect_database;

    connect_database.sqlOpen();

    QSqlQuery qry;

    int newBalance;
    newBalance = UserBalance - foodamount;

    if (UserType == "student")
    {
            qry.prepare("UPDATE Student_Balance SET Balance = (:balance) WHERE Student_ID = (:StudentID)");
            qry.bindValue(":balance", newBalance);
            qry.bindValue(":StudentID", UserID);

            if (qry.exec())
            {
                qDebug() << "New Balance = " << newBalance;
                qDebug() << "User Type = " << UserType;
                MainWindow *mainwin = new MainWindow;
                mainwin->customerwindow->ui->label_showBalance->setText(QString::number(newBalance));
            }
    }

    if (UserType == "staff")
    {
            qry.prepare("UPDATE Staff_Balance SET Balance = (:balance) WHERE Staff_ID = (:StaffID)");
            qry.bindValue(":balance", newBalance);
            qry.bindValue(":Staff_ID", UserID);

            if (qry.exec())
            {
                ui->label_showBalance->setText(QString::number(newBalance));
            }
    }
    connect_database.sqlClose();
}
