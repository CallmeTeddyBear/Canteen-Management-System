#include "checkout.h"
#include "ui_checkout.h"

#include "customerwindow.h"

#include <QDebug>

Checkout::Checkout(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Checkout)
{
    ui->setupUi(this);

    setWindowFlag(Qt::WindowContextHelpButtonHint,false);

    QStringList header;
    header << "Food" << "Quantity" << "Price";
    ui->tableWidget->setColumnCount(3);
    ui->tableWidget->setHorizontalHeaderLabels(header);

    ui->tableWidget->setColumnWidth(0, 500);
    ui->tableWidget->setColumnWidth(1, 200);
    ui->tableWidget->setColumnWidth(2, 269);


}

Checkout::~Checkout()
{
    delete ui;
}
void Checkout::receive_items(QString items[12][3], int table_row, QString Usertype, int userbalance, int ID)//receives item from customer and displays those items selected
{
    for(int row = 0; row <= (table_row - 1); row++)
    {
        ui->tableWidget->insertRow(ui->tableWidget->rowCount());
        for(int column = 0; column <= 2; column++)
        {
            ui->tableWidget->setItem(row, column, new QTableWidgetItem(items[row][column]));
        }

    }

    showTotal(table_row);

    UserType = Usertype;
    UserBalance = userbalance;
    UserID = ID;

}

void Checkout::showTotal(int table_row)
{
    int price = 0;
    total_price = 0;

    qDebug() << "Number of row is " << table_row;
    for(int row = 0; row <= (table_row - 1); row++)
    {
        QString price_string = ui->tableWidget->item(row, 2)->text();

        price = price_string.toInt();
        total_price = total_price + price;
    }
    ui->label_showTotal->setText(QString::number(total_price));
}

void Checkout::on_pushButton_cancel_clicked()
{
    this->hide();
}

void Checkout::on_pushButton_checkout_clicked()
{
    CustomerWindow customerwindow;
    customerwindow.updateBalance(total_price, UserType, UserBalance, UserID);
    this->hide();

}
