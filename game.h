#ifndef GAME_H
#define GAME_H
#include <piece.h>
#include <QGraphicsScene>
#include <const.h>
#include <QObject>
class Game:public QObject
{
    Q_OBJECT
    BasicPiece* pieces[2][20];
    BasicPiece* selected[10][10];
    QGraphicsScene* scene;
public:
    Game(QGraphicsScene* scene);
public slots:
    void possiblePosition(BasicPiece *);
    void cancelPosition(BasicPiece *);
};

#endif // GAME_H
