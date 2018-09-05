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
    QGraphicsScene* scene;
    QTcpServer *tcpServer;
    QTcpSocket *tcpSocket;
    QTimer* timer;
    Game* game;
    bool identity;
    int seconds;
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void connectToHost(QHostAddress ip,qint16 port);
    void gameStart();
public slots:
    void on_pushButton_clicked();

    void on_actionexit_triggered();

    void acceptConnection();

    void readData();

    void sendData(QByteArray arr);

    void on_pushButton_2_clicked();

    void on_actionadd_triggered();

    void on_pushButton_3_clicked();

    void on_actionconnect_triggered();

    void startTimeLimit();

    void stopTimeLimit();

    void onTimeOut();

    void connectSucceed();

    void loseNoSend();

    void win();

    void lose();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
