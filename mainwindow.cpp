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
    tcpServer->listen(QHostAddress::Any,3737);
    QObject::connect(tcpServer,SIGNAL(newConnection()),this,SLOT(acceptConnection()));
    game = nullptr;
    tcpSocket = nullptr;
    ui->pushButton->setEnabled(false);
    ui->pushButton_2->setEnabled(false);
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(onTimeOut()));
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
    game = new Game(scene,identity,nullptr);
    connect(game,SIGNAL(startTimeLimit()),this,SLOT(startTimeLimit()));
    connect(game,SIGNAL(stopTimeLimit()),this,SLOT(stopTimeLimit()));
    connect(game,SIGNAL(sendData(QByteArray)),this,SLOT(sendData(QByteArray)));
    game->start();
    ui->pushButton_2->setEnabled(true);
}

void MainWindow::win()
{
    QMessageBox::information(this,"Congratulation","You win");
}

void MainWindow::lose()
{
    QMessageBox::information(this,"sorry","You lose");
    QByteArray arr;
    arr.clear();
    arr.append("3");
    sendData(arr);
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
    qDebug()<<"working";
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
    tcpSocket = new QTcpSocket(this);
    QObject::connect(tcpSocket,SIGNAL(connected()),this,SLOT(connectSucceed()));
    QObject::connect(tcpSocket,SIGNAL(readyRead()),this,SLOT(readData()));
    tcpSocket->connectToHost(ip,quint16(port));
    qDebug()<<"connecting to server";
}

void MainWindow::connectSucceed()
{
    identity = 1;
    qDebug()<<"server connected";
    ui->lineEdit->setText(tcpSocket->peerAddress().toString());
    ui->pushButton->setText(tr("ready"));
    ui->pushButton->setEnabled(true);
}

void MainWindow::readData()
{
    QByteArray arr = tcpSocket->readAll();
    qDebug()<<"received "<<arr;
    if(arr.at(0)=='3')
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
    /*QFileDialog*fileDialog=new QFileDialog(this);
    fileDialog->setWindowTitle(tr("Open Text"));
    fileDialog->setDirectory("./");
    if(fileDialog->exec() == QDialog::Accepted) {
        QString path = fileDialog->selectedFiles()[0];
        qDebug()<<path;
        QFile *file=new QFile(path);
        file->open(QIODevice::ReadOnly);
        QByteArray arr;
        arr.clear();
        arr.append("2");
        arr.append(file->readAll());
        file->close();
        sendData(arr);
        game = new Game(scene);
        game->loadFromFile(file);
        QObject::connect(game,SIGNAL(sendData(QByteArray)),this,SLOT(sendData(QByteArray)));
    }*/
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

