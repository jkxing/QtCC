#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <const.h>

#include <QFileDialog>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>

#include <connectdialog.h>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);
    for(int i=0;i<10;i++)
    {
         for(int j=0;j<9;j++)
         {
             Pos[i][j]=Position(i,j);
         }
    }
    scene->setSceneRect(10,10,521,561);
    tcpServer = new QTcpServer(this);
    QObject::connect(tcpServer,SIGNAL(newConnection()),this,SLOT(acceptConnection()));
    game = nullptr;
    tcpSocket = nullptr;
    ui->pushButton->setEnabled(false);
    ui->pushButton_2->setEnabled(false);
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(onTimeOut()));
    file = nullptr;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::gameStart()
{
    if(game!=nullptr)
    {
        delete game;
        game=nullptr;
    }
    if(scene!=nullptr)
    {
        delete scene;
        scene=nullptr;
    }
    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);
    game = new Game(scene,identity,file);
    connect(game,SIGNAL(startTimeLimit()),this,SLOT(startTimeLimit()));
    connect(game,SIGNAL(stopTimeLimit()),this,SLOT(stopTimeLimit()));
    connect(game,SIGNAL(sendData(QByteArray)),this,SLOT(sendData(QByteArray)));
    connect(game,SIGNAL(win()),this,SLOT(win()));
    connect(game,SIGNAL(lose()),this,SLOT(loseNoSend()));
    game->start();
    ui->pushButton_2->setEnabled(true);
    ui->pushButton->setEnabled(false);
}

void MainWindow::reinit()
{
    timer->stop();
    if(game!=nullptr)
    {
        delete game;
        game = nullptr;
    }
    file = nullptr;
    scene->clear();
    ui->pushButton_2->setEnabled(false);
    if(identity==1)
        ui->pushButton->setEnabled(true);
}

void MainWindow::win()
{
    QMessageBox::information(this,"Congratulation","You win");
    reinit();
}

void MainWindow::lose()
{
    QByteArray arr;
    arr.clear();
    arr.append("3");
    sendData(arr);
    QMessageBox::information(this,"sorry","You lose");
    reinit();
}

void MainWindow::loseNoSend()
{
    QMessageBox::information(this,"sorry","You lose");
}

void MainWindow::startTimeLimit()
{
    qDebug()<<"startTimeLimit";
    seconds = 60;
    timer->start(1000);
}

void MainWindow::stopTimeLimit()
{
    timer->stop();
}

void MainWindow::onTimeOut()
{
    seconds--;
    ui->lcdNumber->display(QString::number(seconds));
    if(seconds==0) lose();
}
void MainWindow::acceptConnection()
{
    identity = 0;
    tcpSocket = tcpServer->nextPendingConnection();
    QObject::connect(tcpSocket,SIGNAL(readyRead()),this,SLOT(readData()));
    qDebug()<<"client connected"<<tcpSocket->peerAddress()<<tcpSocket->peerPort();
    ui->lineEdit->setText(tcpSocket->peerAddress().toString());
    ui->pushButton->setText(tr("start"));
}

void MainWindow::connectToHost(QHostAddress ip,qint16 port)
{
    if(tcpSocket!=nullptr)
    {
        tcpSocket->close();
        delete tcpSocket;
        tcpSocket=nullptr;
    }
    tcpSocket = new QTcpSocket(this);
    QObject::connect(tcpSocket,SIGNAL(connected()),this,SLOT(connectSucceed()));
    QObject::connect(tcpSocket,SIGNAL(readyRead()),this,SLOT(readData()));
    QObject::connect(tcpSocket,SIGNAL(disconnected()),this,SLOT(disConnect()));
    tcpSocket->connectToHost(ip,quint16(port));
    qDebug()<<"connecting to server";
}

void MainWindow::disConnect()
{
    tcpServer->close();
    ui->pushButton->setEnabled(false);
}
void MainWindow::connectSucceed()
{
    identity = 1;
    qDebug()<<"server connected";
    ui->lineEdit->setText(tcpSocket->peerAddress().toString());
    ui->pushButton->setText(tr("ready"));
    ui->pushButton->setEnabled(true);
    if(file!=nullptr)
    {
        qDebug()<<"sendind file";
        file->open(QIODevice::ReadOnly);
        QByteArray arr;
        arr.clear();
        arr.append("2");
        arr.append(file->readAll());
        file->close();
        sendData(arr);
    }
}

void MainWindow::readData()
{
    QByteArray arr = tcpSocket->readAll();
    qDebug()<<"received "<<arr;
    if(arr.at(0)=='2')
    {
        arr.remove(0,1);
        file = new QFile("D:/canju.txt");
        if(file->open(QIODevice::ReadWrite))
        {
            file->write(arr);
            file->close();
        }
    }
    else if(arr.at(0)=='3')
    {
        win();
    }
    else if(arr.at(0)=='4')
    {
        ui->pushButton->setEnabled(true);
    }
    else if(arr.at(0)=='5')
    {
        gameStart();
    }
    else if(arr.at(0)=='6')
    {
        game->save();
    }
    else
    {
        if(game == nullptr) return;
        qDebug()<<"giving to game ";
        game->receivedData(arr);
    }
}

void MainWindow::sendData(QByteArray arr)
{
    qDebug()<<"send "<<arr;
    if(tcpSocket==nullptr)
    {
        qDebug()<<"no socket ";
        return;
    }
    tcpSocket->write(arr);
}

void MainWindow::on_pushButton_clicked()
{
    if(identity==1)
    {
        QByteArray arr;
        arr.clear();
        arr.append("4");
        sendData(arr);
    }
    else
    {
        QByteArray arr;
        arr.clear();
        arr.append("5");
        sendData(arr);
        gameStart();
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    lose();
}

void MainWindow::on_actionexit_triggered()
{
    QFileDialog*fileDialog=new QFileDialog(this);
    fileDialog->setWindowTitle(tr("Open Text"));
    fileDialog->setDirectory("./");
    if(fileDialog->exec() == QDialog::Accepted) {
        QString path = fileDialog->selectedFiles()[0];
        qDebug()<<path;
        file=new QFile(path);
    }
}

void MainWindow::on_actionconnect_triggered()
{
    ConnectDialog dialog(this);
    pair<QHostAddress,qint16> tmp = dialog.work();
    qDebug()<<tmp.first<<" "<<tmp.second;
    connectToHost(tmp.first,tmp.second);
}

void MainWindow::on_actionadd_triggered()
{
    qApp->exit();
}

void MainWindow::on_pushButton_3_clicked()
{

}


void MainWindow::on_actionsave_triggered()
{
    QByteArray arr;
    arr.clear();
    arr.append("6");
    sendData(arr);
    game->save();
}

void MainWindow::on_actionlisten_triggered()
{
    if(!islisten)
    {
        tcpServer->listen(QHostAddress::Any,3737);
        islisten = 1;
        ui->statusbar->showMessage("listening",100000);
    }
}


void MainWindow::on_actiondislisten_triggered()
{
    if(islisten)
    {
        tcpServer->close();
        islisten = 0;
        ui->statusbar->showMessage("cancel listen",1000);
    }
}

void MainWindow::on_actiondisconnect_triggered()
{
    if(tcpSocket->isOpen())
    {
        tcpSocket->disconnectFromHost();
        ui->pushButton->setEnabled(false);
    }
}
