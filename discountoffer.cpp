#include "discountoffer.h"
#include "ui_discountoffer.h"

#include "mainwindow.h"
#include <QMessageBox>

DiscountOffer::DiscountOffer(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DiscountOffer)
{
    ui->setupUi(this);

    setWindowFlag(Qt::WindowContextHelpButtonHint,false);

    ui->stackedWidget_addremove->setCurrentIndex(0);
    ui->groupBox_addDiscountoffer->hide();

    MainWindow connect_database;

    QSqlQueryModel *  modal_fooditem = new QSqlQueryModel();

    connect_database.sqlOpen();

    QSqlQuery* qry = new QSqlQuery(connect_database.mydb);

    qry->prepare("SELECT Food_Title FROM Food_Item");
    qry->exec();
    modal_fooditem->setQuery(*qry);
    ui->comboBox_foodName->setModel(modal_fooditem);

    connect_database.sqlClose();

    qDebug() << (modal_fooditem->rowCount());
}

void DiscountOffer::on_pushButton_addDiscountoffer_clicked()
{
    ui->stackedWidget_addremove->setCurrentIndex(0);
}

void DiscountOffer::on_pushButton_removeDiscountoffer_clicked()
{
    ui->groupBox_addDiscountoffer->hide();
    ui->stackedWidget_addremove->setCurrentIndex(1);
}

void DiscountOffer::on_pushButton_cancel_clicked()
{
    this->hide();
}

void DiscountOffer::on_pushButton_select_clicked()
{
    ui->groupBox_addDiscountoffer->show();

    foodTitle = ui->comboBox_foodName->currentText();

    MainWindow connect_database;

    connect_database.sqlOpen();

    QSqlQuery qry;

    qry.prepare("SELECT * FROM Food_Item WHERE Food_Title = '"+foodTitle+"'");

    if (qry.exec())
    {
        while(qry.next())
        {
            ui->label_showFoodname->setText(qry.value(1).toString());
            ui->label_showPrice->setText(qry.value(3).toString());

            foodID = qry.value(0).toString();
            actualPrice = qry.value(3).toInt();
        }
    }

    connect_database.sqlClose();
}

void DiscountOffer::on_pushButton_addOffer_clicked()
{
    discountPrice = ui->lineEdit_discountPrice->text().toInt();

    if (discountPrice < actualPrice)
    {
        if(discountPrice > 0)
        {
            MainWindow connect_database;

            if (!connect_database.sqlOpen())
            {
                qDebug() << "Failed to open the database";
                return;
            }

            connect_database.sqlOpen();
            QSqlQuery qry;

            qry.prepare("INSERT INTO Discount_Offer(FoodID, Food_Title, Discount_Price) VALUES(?, ?, ?)");
            qry.addBindValue(foodID);
            qry.addBindValue(foodTitle);
            qry.addBindValue(discountPrice);

            if (qry.exec())
            {
                QMessageBox::information(this, tr("Success"), tr("Discount Offer Added"));
                ui->lineEdit_discountPrice->setText("");
                ui->groupBox_addDiscountoffer->hide();
            }
            else
            {
                QMessageBox::critical(this, tr("Error::"), qry.lastError().text());
            }
            connect_database.sqlClose();
        }
        else
        {
            QMessageBox::warning(this, "Error", "Invalid Price");
        }
    }
    else
    {
        QMessageBox::warning(this, "Error", "Discount Price must be less than actual price");
    }
}

DiscountOffer::~DiscountOffer()
{
    delete ui;
}
