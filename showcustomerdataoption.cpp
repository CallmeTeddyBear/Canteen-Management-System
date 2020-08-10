#include "showcustomerdataoption.h"
#include "ui_showcustomerdataoption.h"


ShowCustomerDataOption::ShowCustomerDataOption(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ShowCustomerDataOption)
{
    ui->setupUi(this);

    setWindowFlag(Qt::WindowContextHelpButtonHint,false);

    MainWindow connect_database;

    QSqlQueryModel * modal_student = new QSqlQueryModel();
    QSqlQueryModel * modal_staff = new QSqlQueryModel();

    connect_database.sqlOpen();

    QSqlQuery* qry = new QSqlQuery(connect_database.mydb);
    QSqlQuery* qry_staff = new QSqlQuery(connect_database.mydb);

    qry->prepare("SELECT * FROM Student");
    qry->exec();
    modal_student->setQuery(*qry);
    ui->tableView->setModel(modal_student);

    qry_staff->prepare("SELECT * FROM Staff");
    qry_staff->exec();
    modal_staff->setQuery(*qry_staff);
    ui->tableView_2->setModel(modal_staff);

    connect_database.sqlClose();
    qDebug() << (modal_student->rowCount());
    qDebug() << (modal_staff->rowCount());

}

ShowCustomerDataOption::~ShowCustomerDataOption()
{
    delete ui;
}

void ShowCustomerDataOption::on_pushButton_close_clicked()
{
    this->hide();
}
