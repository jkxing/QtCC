#ifndef GAME_H
#define GAME_H
#include <piece.h>
#include <QGraphicsScene>
#include <const.h>
#include <QObject>
#include <QFile>
class Game:public QObject
{
    Q_OBJECT
    BasicPiece* pieces[2][20];
    BasicPiece* selected[10][10];
    int mp[10][10];
    QGraphicsScene* scene;
public:
    Game(QGraphicsScene* scene);
    Game(QGraphicsScene* scene,QFile* file);
    void updateMap();
    void loadFromFile(QFile *file);
    bool isWaiting;
signals:
    void sendData(QByteArray arr);
public slots:
    void possiblePosition(BasicPiece *);
    void cancelPosition(BasicPiece *);
    void receivedData(QByteArray arr);
};

#endif // GAME_H
