#include "showcustomerdataoption.h"
#include "ui_showcustomerdataoption.h"


ShowCustomerDataOption::ShowCustomerDataOption(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ShowCustomerDataOption)
{
    ui->setupUi(this);

    setWindowFlag(Qt::WindowContextHelpButtonHint,false);

    ui->tableView_student->setAlternatingRowColors(true);
    ui->tableView_staff->setAlternatingRowColors(true);

    MainWindow connect_database;

    QSqlQueryModel * modal_student = new QSqlQueryModel();
    QSqlQueryModel * modal_staff = new QSqlQueryModel();

    connect_database.sqlOpen();

    QSqlQuery* qry_student = new QSqlQuery(connect_database.mydb);
    QSqlQuery* qry_staff = new QSqlQuery(connect_database.mydb);
//    QSqlTableModel* qry_staff = new QSqlTableModel();

    qry_student->prepare("SELECT Name, Faculty, Gender, Address, Contact_No, Username FROM Student");
    qry_student->exec();
    modal_student->setQuery(*qry_student);
    ui->tableView_student->setModel(modal_student);

    qry_staff->prepare("SELECT Name, Department, Gender, Address, Contact_No, Username FROM Staff");
    qry_staff->exec();
    modal_staff->setQuery(*qry_staff);
    ui->tableView_staff->setModel(modal_staff);

//    qry_staff->setTable("Staff");
//    qry_staff->select();
//    ui->tableView_2->setModel(qry_staff);

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
