#ifndef GAME_H
#define GAME_H
#include <piece.h>
#include <QGraphicsScene>
#include <const.h>
#include <QWidget>
#include <QFile>
class Game:public QWidget
{
    Q_OBJECT
public:
    BasicPiece* pieces[2][20];
    BasicPiece* selected[10][10];
    QGraphicsScene* scene;
    int mp[10][10];
    bool isFirst;
    bool isWaiting;
    Game(QGraphicsScene* scene,bool identity,QFile* file);
    void updateMap();
    void loadFromFile(QFile *file);
    void start();
signals:
    void sendData(QByteArray arr);
    void startTimeLimit();
    void stopTimeLimit();
    void lose();
    void win();
public slots:
    void possiblePosition(BasicPiece *);
    void cancelPosition();
    void receivedData(QByteArray arr);
};

#endif // GAME_H
