#include "connectdialog.h"
#include "ui_connectdialog.h"

ConnectDialog::ConnectDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConnectDialog)
{
    ui->setupUi(this);
}

pair<QHostAddress,qint16> ConnectDialog::work()
{
    if(this->exec()==QDialog::Accepted)
    {
        return make_pair(QHostAddress(ui->lineEdit->text()),ui->lineEdit_2->text().toInt());
    }
    return make_pair(QHostAddress(""),0);
}

ConnectDialog::~ConnectDialog()
{
    delete ui;
}
