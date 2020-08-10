#include "showcustomerdataoption.h"
#include "ui_showcustomerdataoption.h"


ShowCustomerDataOption::ShowCustomerDataOption(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ShowCustomerDataOption)
{
    ui->setupUi(this);

    setWindowFlag(Qt::WindowContextHelpButtonHint,false);

    MainWindow connect_database;

    QSqlQueryModel * modal = new QSqlQueryModel();
    connect_database.sqlOpen();
    QSqlQuery* qry = new QSqlQuery(connect_database.mydb);

    qry->prepare("SELECT * FROM Student");
    qry->exec();
    modal->setQuery(*qry);
    ui->tableView->setModel(modal);

    connect_database.sqlClose();
    qDebug() << (modal->rowCount());

}

ShowCustomerDataOption::~ShowCustomerDataOption()
{
    delete ui;
}

void ShowCustomerDataOption::on_pushButton_close_clicked()
{
    this->hide();
}
