#ifndef CONNECTDIALOG_H
#define CONNECTDIALOG_H

#include <QDialog>
#include <QtNetwork>
using namespace  std;
namespace Ui {
class ConnectDialog;
}

class ConnectDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConnectDialog(QWidget *parent = nullptr);
    ~ConnectDialog();
    pair<QHostAddress,qint16> work();

private:
    Ui::ConnectDialog *ui;
};

#endif // CONNECTDIALOG_H
