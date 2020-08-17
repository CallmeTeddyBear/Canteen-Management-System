#include "showfoodmenu.h"
#include "ui_showfoodmenu.h"

#include "mainwindow.h"
#include <QMessageBox>

ShowFoodMenu::ShowFoodMenu(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ShowFoodMenu)
{
    ui->setupUi(this);
    setWindowFlag(Qt::WindowContextHelpButtonHint,false);

    ui->stackedWidget->setCurrentIndex(0);

    MainWindow connect_database;

    QSqlQueryModel * modal_menu = new QSqlQueryModel();
    QSqlQueryModel *  modal_delete = new QSqlQueryModel();

    connect_database.sqlOpen();

    QSqlQuery* qry = new QSqlQuery(connect_database.mydb);

    qry->prepare("SELECT Food_Title, Food_Type, Price FROM Food_Item");
    qry->exec();
    modal_menu->setQuery(*qry);
    ui->tableView->setModel(modal_menu);

    ui->comboBox_foodType->addItem("Breakfast");
    ui->comboBox_foodType->addItem("Lunch");
    ui->comboBox_foodType->addItem("Dinner");
    ui->comboBox_foodType->addItem("Drinks");

    qry->prepare("SELECT Food_Title FROM Food_Item");
    qry->exec();
    modal_delete->setQuery(*qry);
    ui->comboBox_foodName->setModel(modal_delete);

    connect_database.sqlClose();

    qDebug() << (modal_menu->rowCount());
    qDebug() << (modal_delete->rowCount());
}

void ShowFoodMenu::on_pushButton_home_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);

    MainWindow connect_database;

    QSqlQueryModel * modal_menu = new QSqlQueryModel();

    connect_database.sqlOpen();

    QSqlQuery* qry = new QSqlQuery(connect_database.mydb);

    qry->prepare("SELECT Food_Title, Food_Type, Price FROM Food_Item");
    qry->exec();
    modal_menu->setQuery(*qry);
    ui->tableView->setModel(modal_menu);

    connect_database.sqlClose();
    qDebug() << (modal_menu->rowCount());
}

void ShowFoodMenu::on_pushButton_addFood_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void ShowFoodMenu::on_pushButton_addItem_clicked()
{
    MainWindow connect_database;

    QString foodName,foodType;
    qint8 foodPrice;

    foodName = ui->lineEdit_foodName->text();
    foodType = ui->comboBox_foodType->currentText();
    foodPrice = ui->lineEdit_foodPrice->text().toInt();

    if (!connect_database.sqlOpen())
    {
        qDebug() << "Failed to open the database";
        return;
    }

    connect_database.sqlOpen();
    QSqlQuery qry;

    qry.prepare("INSERT INTO Food_Item(Food_Title, Food_Type, Price) VALUES(?, ?, ?)");
    qry.addBindValue(foodName);
    qry.addBindValue(foodType);
    qry.addBindValue(foodPrice);

    if (qry.exec())
    {
        QMessageBox::information(this, tr("Success"), tr("Food was added"));
        connect_database.sqlClose();
    }
    else
    {
        QMessageBox::critical(this, tr("Error::"), qry.lastError().text());
    }
}

void ShowFoodMenu::on_pushButton_close_clicked()
{
    this->hide();
}

ShowFoodMenu::~ShowFoodMenu()
{
    delete ui;
}

void ShowFoodMenu::on_pushButton_deleteFood_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

void ShowFoodMenu::on_pushButton_delete_clicked()
{
    MainWindow connect_database;

    QString foodName;

    foodName = ui->comboBox_foodName->currentText();

    if (!connect_database.sqlOpen())
    {
        qDebug() << "Failed to open the database";
        return;
    }

    connect_database.sqlOpen();
    QSqlQuery qry;

    qry.prepare("DELETE FROM Food_Item WHERE Food_Title = '"+foodName+"'");

    if (qry.exec())
    {
        QMessageBox::information(this, tr("Success"), tr("Food was deleted"));
        connect_database.sqlClose();
    }
    else
    {
        QMessageBox::critical(this, tr("Error::"), qry.lastError().text());
    }
}
