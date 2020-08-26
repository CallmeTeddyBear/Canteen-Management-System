#include "todaysspecialpopup.h"
#include "ui_todaysspecialpopup.h"

#include "mainwindow.h"

TodaysSpecialPopUp::TodaysSpecialPopUp(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TodaysSpecialPopUp)
{
    ui->setupUi(this);

    setWindowFlag(Qt::WindowContextHelpButtonHint,false);

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
            ui->label_showtodaysspecial->setText(qry.value(0).toString());
        }
        connect_database.sqlClose();
    }


}

TodaysSpecialPopUp::~TodaysSpecialPopUp()
{
    delete ui;
}
