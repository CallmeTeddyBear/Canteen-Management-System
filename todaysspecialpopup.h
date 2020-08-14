#ifndef TODAYSSPECIALPOPUP_H
#define TODAYSSPECIALPOPUP_H

#include <QDialog>

namespace Ui {
class TodaysSpecialPopUp;
}

class TodaysSpecialPopUp : public QDialog
{
    Q_OBJECT

public:
    explicit TodaysSpecialPopUp(QWidget *parent = nullptr);
    ~TodaysSpecialPopUp();

public slots:
    //void receive(QString todays_special);

private:
    Ui::TodaysSpecialPopUp *ui;
};

#endif // TODAYSSPECIALPOPUP_H
