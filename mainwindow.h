#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QBrush>
#include <Qt>
#include <position.h>
#include <game.h>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void on_pushButton_clicked();

    void on_actionexit_triggered();

private:
    Ui::MainWindow *ui;
    QGraphicsScene* scene;
    Game* game;
};

#endif // MAINWINDOW_H
