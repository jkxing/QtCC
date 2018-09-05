#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <const.h>

#include <QFileDialog>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);
    int x=70,y=70;
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
    QObject::connect(game,SIGNAL(sendData(QByteArray)),this,SLOT(sendData(QByteArray)));
}


void MainWindow::acceptConnection()
{
    tcpSocket = tcpServer->nextPendingConnection();
    QObject::connect(tcpSocket,SIGNAL(readyRead()),this,SLOT(readData()));
}

void MainWindow::readData()
{
    QByteArray arr = tcpSocket->readAll();
    game->receivedData(arr);
}

void MainWindow::sendData(QByteArray arr)
{
    tcpSocket->write(arr);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    game = new Game(scene);
    //game->start();
}

void MainWindow::on_actionexit_triggered()
{
    QFileDialog*fileDialog=new QFileDialog(this);
    fileDialog->setWindowTitle(tr("Open Text"));
    fileDialog->setDirectory("./");
    if(fileDialog->exec() == QDialog::Accepted) {
        QString path = fileDialog->selectedFiles()[0];
        qDebug()<<path;
        QFile *file=new QFile(path);
        game = new Game(scene,file);
    }
}
