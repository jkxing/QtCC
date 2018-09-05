#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QBrush>
#include <Qt>
#include <position.h>
#include <game.h>
#include <QTcpSocket>
#include <QTcpServer>
#include <QUdpSocket>
#include <QtNetwork>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    QTcpServer *tcpServer;
    QTcpSocket *tcpSocket;
    explicit MainWindow(QWidget *parent = nullptr);
    void connectToHost();
    ~MainWindow();
public slots:
    void on_pushButton_clicked();

    void on_actionexit_triggered();

    void acceptConnection();

    void readData();

    void sendData(QByteArray arr);
private:
    Ui::MainWindow *ui;
    QGraphicsScene* scene;
    Game* game;
};

#endif // MAINWINDOW_H
