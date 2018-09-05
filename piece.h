#ifndef PIECE_H
#define PIECE_H

#include <QObject>
#include <QWidget>
#include <vector>
#include <position.h>

#include <QDebug>
#include <QPainter>
#include <QGraphicsItem>
#include <QStylePainter>
#include <QGraphicsObject>
#include <QStyleOptionGraphicsItem>
using namespace std;

class BasicPiece:public QGraphicsObject
{
    Q_OBJECT
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event);
public:
    int x,y,i,j,id;
    bool isSelect,isDie;
    BasicPiece();
    BasicPiece(Position &x,int id);
    virtual bool canMove(int x,int y,int mp[10][10]);
    QRectF boundingRect() const;
    virtual void paint(QPainter *painter,const QStyleOptionGraphicsItem *option, QWidget *widget);
    void changePosition(int i,int j);
    bool At(int x,int y);
public slots:
    //virtual void onclick() = 0;
signals:
    void select(BasicPiece *);
    void deselect(BasicPiece *);
    //void possiblePosition(vector<pair<int,int> >);
};

class Soldier:public BasicPiece
{
public:
    using BasicPiece::BasicPiece;
    bool canMove(int x,int y,int mp[10][10]);
    void paint(QPainter *painter,const QStyleOptionGraphicsItem *option, QWidget *widget);
};

class Horse:public BasicPiece
{
public:
    using BasicPiece::BasicPiece;
    bool canMove(int x,int y,int mp[10][10]);
    void paint(QPainter *painter,const QStyleOptionGraphicsItem *option, QWidget *widget);
};

class Pao:public BasicPiece
{
public:
    using BasicPiece::BasicPiece;
    bool canMove(int x,int y,int mp[10][10]);
    void paint(QPainter *painter,const QStyleOptionGraphicsItem *option, QWidget *widget);
};

class Car:public BasicPiece
{
public:
    using BasicPiece::BasicPiece;
    bool canMove(int x,int y,int mp[10][10]);
    void paint(QPainter *painter,const QStyleOptionGraphicsItem *option, QWidget *widget);
};

class Ele:public BasicPiece
{
public:
    using BasicPiece::BasicPiece;
    bool canMove(int x,int y,int mp[10][10]);
    void paint(QPainter *painter,const QStyleOptionGraphicsItem *option, QWidget *widget);
};

class Man:public BasicPiece
{
public:
    using BasicPiece::BasicPiece;
    bool canMove(int x,int y,int mp[10][10]);
    void paint(QPainter *painter,const QStyleOptionGraphicsItem *option, QWidget *widget);
};

class Boss:public BasicPiece
{
public:
    using BasicPiece::BasicPiece;
    bool canMove(int x,int y,int mp[10][10]);
    void paint(QPainter *painter,const QStyleOptionGraphicsItem *option, QWidget *widget);
};
#endif // PIECE_H
