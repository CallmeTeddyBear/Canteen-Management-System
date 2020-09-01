#include "customerwindow.h"
#include "ui_customerwindow.h"

#include "mainwindow.h"
#include "todaysspecialpopup.h"
#include "customeraccountsettings.h"
#include <QTimer>
#include <QMovie>
#include <QSize>
#include <QPainter>

CustomerWindow::CustomerWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CustomerWindow)
{
    ui->setupUi(this);

    setWindowState(Qt::WindowMaximized);

    TodaysSpecialPopUp *popup = new TodaysSpecialPopUp();
    QTimer::singleShot(1000, popup, SLOT(show()));

    ui->stackedWidget->setCurrentIndex(0);

    QMovie *movie = new QMovie("C:/Users/User/Desktop/CanteenManagementSystem/todays_special.gif");
    movie->setScaledSize(QSize().scaled(150, 150, Qt::KeepAspectRatio));
    ui->label_gif->setMovie(movie);
    movie->start();
    connect(movie, SIGNAL(finished()), movie, SLOT(start()));

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
    this->hide();
    parentWidget()->show();
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
