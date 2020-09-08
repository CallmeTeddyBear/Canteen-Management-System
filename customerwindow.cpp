#include "customerwindow.h"
#include "ui_customerwindow.h"

#include "mainwindow.h"
#include "todaysspecialpopup.h"
#include "customeraccountsettings.h"
#include "checkout.h"
#include "logout.h"
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

    TodaysSpecialPopUp *popup = new TodaysSpecialPopUp();
    popup->setModal(true);
    QTimer::singleShot(1000, popup, SLOT(show()));

    ui->stackedWidget->setCurrentIndex(0);

    showBreakfastThumbnails();
    showLunchThumbnails();
    showDinnerThumbnails();
    showDrinksThumbnails();

    showTodaysSpecial_gif();
    showTodaysSpecial();
    showDiscountoffer();
    showList();

    //showBreakfast();

    table_row = 0;

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

                //qry.first();
                QByteArray outByteArray = qry.value(8).toByteArray();
                QPixmap outPixmap = QPixmap();
                outPixmap.loadFromData(outByteArray);
                ui->label_userPicture->setPixmap(outPixmap.scaled(210,210));

                studentID = qry.value(0).toInt();
            }

            qry.prepare("SELECT * FROM Student_Balance WHERE Student_ID = (:StudentID)");
            qry.bindValue(":StudentID", studentID);

            if (qry.exec())
            {
                while(qry.next())
                {
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
            }

            qry.prepare("SELECT * FROM Staff_Balance WHERE Staff_ID = (:Staff_ID)");
            qry.bindValue(":Staff_ID", staffID);

            if (qry.exec())
            {
                while(qry.next())
                {
                    ui->label_showBalance->setText(qry.value(2).toString());
                }
            }
        }
    }
    connect_database.sqlClose();

}

void CustomerWindow::showBreakfast()
{
    const QSize btnSize = QSize(200, 200);
    int i;
    QPushButton *btn[20];
    for (i = 0 ; i < 16; i++)
    {
        btn[i] = new QPushButton(centralWidget());

        btn[i]->setText(QString::number(i));
        btn[i]->setFixedSize(btnSize);
    }

    QGridLayout *btnLayout = new QGridLayout(centralWidget());
    for(i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            btnLayout->addWidget(btn[j + i * 4], 0 + i, j);
            btnLayout->setSpacing(0);
        }
    }
    centralWidget()->setLayout(btnLayout);

}

void CustomerWindow::showList()
{
//    int rows;
//    rows = 4;
//    QStandardItemModel *model = new QStandardItemModel(rows, 3, this);
//    for (int row = 0; row < rows; row++)
//    {
//        for (int col = 0; col < 3; col++)
//        {
//            QModelIndex index = model->index(row, col, QModelIndex());
//            model->setData(index, 0);
//        }
//    }
//    ui->tableView->setModel(model);
    QStringList header;
    header << "Food" << "Qty." << "Price";
    ui->tableWidget->setColumnCount(3);
    ui->tableWidget->setHorizontalHeaderLabels(header);


//    QString food, qty, price;
//    food = "apple";
//    qty = "2";
//    price = "3";
//    enum column
//    {
//        c_food, c_qty, c_price
//    };

//    ui->tableWidget->insertRow(ui->tableWidget->rowCount());
//    int row = ui->tableWidget->rowCount() - 1;
//    ui->tableWidget->setItem(0, 0, new QTableWidgetItem(food));

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
int toastcount = 0;
void CustomerWindow::on_pushButton_Toast_clicked()
{
    int price = 20;
    toastcount++;

    if (toastcount == 1)
    {
        ui->tableWidget->insertRow(ui->tableWidget->rowCount());
        int tablerow = ui->tableWidget->rowCount() - 1;
        ui->tableWidget->setItem(tablerow, 0, new QTableWidgetItem("Toast"));
        ui->tableWidget->setItem(tablerow, 1, new QTableWidgetItem(QString::number(toastcount)));
        ui->tableWidget->setItem(tablerow, 2, new QTableWidgetItem(QString::number(price)));
    }
    else
    {
        int tablerow = ui->tableWidget->rowCount() - 1;
        ui->tableWidget->setItem(tablerow, 1, new QTableWidgetItem(QString::number(toastcount)));
        ui->tableWidget->setItem(tablerow, 2, new QTableWidgetItem(QString::number(price * toastcount)));
    }

}

void CustomerWindow::on_pushButton_Sandwich_clicked()
{
        ui->tableWidget->insertRow(ui->tableWidget->rowCount());
        int tablerow = ui->tableWidget->rowCount() - 1;
        ui->tableWidget->setItem(tablerow, 0, new QTableWidgetItem("Sandwich"));
        ui->tableWidget->setItem(tablerow, 1, new QTableWidgetItem(QString::number(1)));
        ui->tableWidget->setItem(tablerow, 2, new QTableWidgetItem(QString::number(40)));
}

void CustomerWindow::on_pushButton_riceset_clicked()
{
    ui->tableWidget->insertRow(ui->tableWidget->rowCount());
    int tablerow = ui->tableWidget->rowCount() - 1;
    ui->tableWidget->setItem(tablerow, 0, new QTableWidgetItem("Rice Set"));
    ui->tableWidget->setItem(tablerow, 1, new QTableWidgetItem(QString::number(1)));
    ui->tableWidget->setItem(tablerow, 2, new QTableWidgetItem(QString::number(60)));
}

void CustomerWindow::on_pushButton_discardAll_clicked()
{
    ui->tableWidget->setRowCount(0);
    toastcount = 0;

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
    Checkout checkout;
    checkout.setModal(true);
    checkout.exec();
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
    breakfast_foodTitle = "Boiled_Egg";
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
    breakfast_foodTitle = "Puri_Tarkari";
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
    lunch_foodTitle = "Chicken_Biryani";
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
    lunch_foodTitle = "Mutton_Biryani";
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
    lunch_foodTitle = "Red_Sauce_Pasta";
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
    lunch_foodTitle = "White_Sauce_Pasta";
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
    lunch_foodTitle = "Aloo_Chop";
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
    dinner_foodTitle = "Rice_Set";
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
    dinner_foodTitle = "Chicken_Curry";
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
    dinner_foodTitle = "Mutton_Curry";
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
    dinner_foodTitle = "Egg_Curry";
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
    dinner_foodTitle = "Chicken_Roast";
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
    dinner_foodTitle = "Fish_Fry";
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
    dinner_foodTitle = "Paneer_Chilli";
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
    dinner_foodTitle = "Daal_Fry";
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
    dinner_foodTitle = "Chana_Masala";
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
    dinner_foodTitle = "Rajma_Masala";
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
    drinks_foodTitle = "Black_Tea";
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
    drinks_foodTitle = "Black_Coffee";
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
    drinks_foodTitle = "Milk_Tea";
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
    drinks_foodTitle = "Milk_Coffee";
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
    drinks_foodTitle = "Orange_Juice";
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
    drinks_foodTitle = "Hot_Lemon";
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
    drinks_foodTitle = "Hot_Chocolate";
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
