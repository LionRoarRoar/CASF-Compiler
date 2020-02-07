#include "creatfiledialog.h"
#include "ui_creatfiledialog.h"
#include "ShowText.h"
#include <QDebug>
creatFileDialog::creatFileDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::creatFileDialog)
{
    ui->setupUi(this);
}

creatFileDialog::~creatFileDialog()
{
    delete ui;
}



void creatFileDialog::on_buttonBox_accepted()
{
    QString carrierPos,rfidNum;
    carrierPos =ui->carrierPos->text();
    rfidNum = ui->carrierPos->text();

    emit requestClearText();
    emit requestSaveData(carrierPos,rfidNum);
    emit sendText("STA");
}
