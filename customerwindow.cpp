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

                qry.first();
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

void CustomerWindow::on_pushButton_toast_clicked()
{
    ui->tableWidget->insertRow(ui->tableWidget->rowCount());
    int tablerow = ui->tableWidget->rowCount() - 1;
    ui->tableWidget->setItem(tablerow, 0, new QTableWidgetItem("Toast"));
    ui->tableWidget->setItem(tablerow, 1, new QTableWidgetItem(QString::number(1)));
    ui->tableWidget->setItem(tablerow, 2, new QTableWidgetItem(QString::number(20)));
}

void CustomerWindow::on_pushButton_momo_clicked()
{
        ui->tableWidget->insertRow(ui->tableWidget->rowCount());
        int tablerow = ui->tableWidget->rowCount() - 1;
        ui->tableWidget->setItem(tablerow, 0, new QTableWidgetItem("Momo"));
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
