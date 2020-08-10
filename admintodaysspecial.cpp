#include "admintodaysspecial.h"
#include "ui_admintodaysspecial.h"

#include "todaysspecialpopup.h"

AdminTodaysSpecial::AdminTodaysSpecial(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AdminTodaysSpecial)
{
    ui->setupUi(this);

    setWindowFlag(Qt::WindowContextHelpButtonHint,false);

    MainWindow connect_database;

    QSqlQueryModel *  modal = new QSqlQueryModel();
    connect_database.sqlOpen();
    QSqlQuery* qry = new QSqlQuery(connect_database.mydb);

    qry->prepare("SELECT Food_Title FROM Food_Item");
    qry->exec();
    modal->setQuery(*qry);
    ui->comboBox_foodlist->setModel(modal);

    connect_database.sqlClose();
    qDebug() << (modal->rowCount());
}

void AdminTodaysSpecial::on_pushButton_ok_clicked()
{
    QString todays_special;
    todays_special = ui->comboBox_foodlist->currentText();

    TodaysSpecialPopUp popup;
    //connect(this, SIGNAL(send(const QString)), popup, SLOT(receive(const QString)));
    //emit send(todays_special);

    this->hide();
}

void AdminTodaysSpecial::on_pushButton_cancel_clicked()
{
    this->hide();
}

AdminTodaysSpecial::~AdminTodaysSpecial()
{
    delete ui;
}



