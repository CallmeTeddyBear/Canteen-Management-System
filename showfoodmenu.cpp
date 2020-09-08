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
    QSqlQueryModel *  modal_fooditem = new QSqlQueryModel();

    connect_database.sqlOpen();

    QSqlQuery* qry = new QSqlQuery(connect_database.mydb);

    qry->prepare("SELECT Food_Title, Food_Type, Price FROM Food_Item");
    qry->exec();
    modal_menu->setQuery(*qry);
    ui->tableView->setModel(modal_menu);

    showFoodType();

    qry->prepare("SELECT Food_Title FROM Food_Item");
    qry->exec();
    modal_fooditem->setQuery(*qry);
    ui->comboBox_foodName_edit->setModel(modal_fooditem);
    ui->comboBox_foodName_delete->setModel(modal_fooditem);

    connect_database.sqlClose();

    qDebug() << (modal_menu->rowCount());
    qDebug() << (modal_fooditem->rowCount());
}

void ShowFoodMenu::showFoodType()
{
    ui->comboBox_foodType->addItem("Breakfast");
    ui->comboBox_foodType->addItem("Lunch");
    ui->comboBox_foodType->addItem("Dinner");
    ui->comboBox_foodType->addItem("Drinks");

    ui->comboBox_showFoodType->addItem("Breakfast");
    ui->comboBox_showFoodType->addItem("Lunch");
    ui->comboBox_showFoodType->addItem("Dinner");
    ui->comboBox_showFoodType->addItem("Drinks");
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
        ui->lineEdit_foodName->setText("");
        ui->lineEdit_foodPrice->setText("");
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

void ShowFoodMenu::on_pushButton_editFood_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
    ui->groupBox_editFood->hide();
}

void ShowFoodMenu::on_pushButton_select_clicked()
{
    ui->groupBox_editFood->show();

    QString foodTitle;
    foodTitle = ui->comboBox_foodName_edit->currentText();

    qDebug() << foodTitle;

    MainWindow connect_database;

    connect_database.sqlOpen();

    QSqlQuery qry;

    qry.prepare("SELECT * FROM Food_Item WHERE Food_Title = '"+foodTitle+"'");

    if (qry.exec())
    {
        while(qry.next())
        {
            ui->lineEdit_showFoodName->setText(qry.value(1).toString());
            ui->lineEdit_showFoodPrice->setText(qry.value(3).toString());
        }
    }

    connect_database.sqlClose();

}

void ShowFoodMenu::on_pushButton_deleteFood_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);

    MainWindow connect_database;

    QSqlQueryModel *  modal_fooditem = new QSqlQueryModel();

    connect_database.sqlOpen();

    QSqlQuery* qry = new QSqlQuery(connect_database.mydb);

    qry->prepare("SELECT Food_Title FROM Food_Item");
    qry->exec();
    modal_fooditem->setQuery(*qry);
    ui->comboBox_foodName_delete->setModel(modal_fooditem);

    connect_database.sqlClose();

    qDebug() << (modal_fooditem->rowCount());
}

void ShowFoodMenu::on_pushButton_delete_clicked()
{
    MainWindow connect_database;

    QString foodName;

    foodName = ui->comboBox_foodName_delete->currentText();

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


void ShowFoodMenu::on_pushButton_makeChanges_clicked()
{
    QString foodName, foodType;
    int foodPrice;
    foodName = ui->lineEdit_showFoodName->text();
    foodType = ui->comboBox_showFoodType->currentText();
    foodPrice = ui->lineEdit_showFoodPrice->text().toInt();

    qDebug() << foodName << foodType << foodPrice;

    MainWindow connect_database;

    if (!connect_database.sqlOpen())
    {
        qDebug() << "Failed to open the database";
        return;
    }

    connect_database.sqlOpen();
    QSqlQuery qry;

    qry.prepare("UPDATE Food_Item SET Food_Title = (:foodtitle), Food_Type = (:foodtype), Price = (:foodprice) WHERE Food_Title = '"+foodName+"'");
    qry.bindValue(":foodtitle", foodName);
    qry.bindValue(":foodtype", foodType);
    qry.bindValue(":foodprice", foodPrice);

    if (qry.exec())
    {
        QMessageBox::information(this, tr("Success"), tr("Food Item Changed"));
        connect_database.sqlClose();
        ui->lineEdit_showFoodName->setText("");
        ui->lineEdit_showFoodPrice->setText("");

    }
    else
    {
        QMessageBox::critical(this, tr("Error::"), qry.lastError().text());
    }

}
