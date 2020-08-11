#include "showbalance.h"
#include "ui_showbalance.h"

#include "mainwindow.h"

ShowBalance::ShowBalance(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ShowBalance)
{
    ui->setupUi(this);

    setWindowFlag(Qt::WindowContextHelpButtonHint,false);

    MainWindow connect_database;

    QSqlQueryModel * modal_student = new QSqlQueryModel();
    QSqlQueryModel * modal_staff = new QSqlQueryModel();

    connect_database.sqlOpen();

    QSqlQuery* qry = new QSqlQuery(connect_database.mydb);
    QSqlQuery* qry_staff = new QSqlQuery(connect_database.mydb);

    qry->prepare("SELECT * FROM Student_Balance");
    qry->exec();
    modal_student->setQuery(*qry);
    ui->tableView_student->setModel(modal_student);

    qry_staff->prepare("SELECT * FROM Staff_Balance");
    qry_staff->exec();
    modal_staff->setQuery(*qry_staff);
    ui->tableView_staff->setModel(modal_staff);

    connect_database.sqlClose();
    qDebug() << (modal_student->rowCount());
    qDebug() << (modal_staff->rowCount());
}

ShowBalance::~ShowBalance()
{
    delete ui;
}

void ShowBalance::on_pushButton_close_clicked()
{
    this->hide();
}
