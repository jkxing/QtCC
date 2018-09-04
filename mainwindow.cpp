#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <const.h>

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
