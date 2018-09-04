#include "piece.h"
#include <const.h>
BasicPiece::BasicPiece()
{

}

BasicPiece::BasicPiece(Position &pos,int id):x(pos.x),y(pos.y),i(pos.i),j(pos.j),id(id)
{
    isSelect=0;
    isDie = 0;
}

QRectF BasicPiece::boundingRect() const
{
    return QRectF(y-20,x-20,40,40);
}

void BasicPiece::changePosition(int p,int q)
{
    Position tmp = Pos[p][q];
    i=tmp.i;
    j=tmp.j;
    x=tmp.x;
    y=tmp.y;
    update();
}

void BasicPiece::mousePressEvent(QGraphicsSceneMouseEvent* event){
    if(isSelect)
        emit deselect(this);
    else
        emit select(this);
    isSelect^=1;
}

bool BasicPiece::At(int x,int y){
    if(isDie) return 0;
    return i==x&&j==y;
}
bool BasicPiece::canMove(int nx,int ny)
{
    return 0;
}

void BasicPiece::paint(QPainter *painter,const QStyleOptionGraphicsItem *option
, QWidget *widget)
{
    if(isDie) return;
    qDebug()<<"basicpiece drawing";
    QRectF rec = boundingRect();
    QPixmap pixmap(":/image/select");
    painter->drawPixmap(rec,pixmap,QRectF(0,0,50,50));
}

void Soldier::paint(QPainter *painter,const QStyleOptionGraphicsItem *option
, QWidget *widget)
{
    if(isDie) return;
    QRectF rec = boundingRect();
    QPixmap pixmap;
    if(id==1)
    {
        QImage srcImg(":/image/blackSoldier");
        QPoint center = srcImg.rect().center();
        QMatrix matrix;
        matrix.translate(center.x(), center.y());
        matrix.rotate(180);
        QImage dstImg = srcImg.transformed(matrix);
        pixmap = QPixmap::fromImage(dstImg);
    }
    else
        pixmap = QPixmap(":/image/redSoldier");
    painter->drawPixmap(rec,pixmap,QRectF(0,0,50,50));
}

bool Soldier::canMove(int nx,int ny)
{
    if(id==0&&nx==i-1&&j==ny) return 1;
    if(id==1&&nx==i+1&&j==ny) return 1;
    return 0;
}


void Horse::paint(QPainter *painter,const QStyleOptionGraphicsItem *option
, QWidget *widget)
{
    if(isDie) return;
    QRectF rec = boundingRect();
    QPixmap pixmap;
    if(id==1)
    {
        QImage srcImg(":/image/blackHorse");
        QPoint center = srcImg.rect().center();
        QMatrix matrix;
        matrix.translate(center.x(), center.y());
        matrix.rotate(180);
        QImage dstImg = srcImg.transformed(matrix);
        pixmap = QPixmap::fromImage(dstImg);
    }
    else
        pixmap = QPixmap(":/image/redHorse");
    painter->drawPixmap(rec,pixmap,QRectF(0,0,50,50));
}

bool Horse::canMove(int nx,int ny)
{
    if(nx!=x&&ny!=y&&abs(nx-x)+abs(ny-y)==3) return 1;
    return 0;
}


void Pao::paint(QPainter *painter,const QStyleOptionGraphicsItem *option
, QWidget *widget)
{
    if(isDie) return;
    QRectF rec = boundingRect();
    QPixmap pixmap;
    if(id==1)
    {
        QImage srcImg(":/image/blackPao");
        QPoint center = srcImg.rect().center();
        QMatrix matrix;
        matrix.translate(center.x(), center.y());
        matrix.rotate(180);
        QImage dstImg = srcImg.transformed(matrix);
        pixmap = QPixmap::fromImage(dstImg);
    }
    else
        pixmap = QPixmap(":/image/redPao");
    painter->drawPixmap(rec,pixmap,QRectF(0,0,50,50));
}

bool Pao::canMove(int nx,int ny)
{
    if(id==0&&nx==x+1&&y==ny) return 1;
    if(id==1&&nx==x-1&&y==ny) return 1;
    return 0;
}


void Car::paint(QPainter *painter,const QStyleOptionGraphicsItem *option
, QWidget *widget)
{
    if(isDie) return;
    QRectF rec = boundingRect();
    QPixmap pixmap;
    if(id==1)
    {
        QImage srcImg(":/image/blackCar");
        QPoint center = srcImg.rect().center();
        QMatrix matrix;
        matrix.translate(center.x(), center.y());
        matrix.rotate(180);
        QImage dstImg = srcImg.transformed(matrix);
        pixmap = QPixmap::fromImage(dstImg);
    }
    else
        pixmap = QPixmap(":/image/redCar");
    painter->drawPixmap(rec,pixmap,QRectF(0,0,50,50));
}

bool Car::canMove(int nx,int ny)
{
    if(id==0&&nx==x+1&&y==ny) return 1;
    if(id==1&&nx==x-1&&y==ny) return 1;
    return 0;
}


void Ele::paint(QPainter *painter,const QStyleOptionGraphicsItem *option
, QWidget *widget)
{
    if(isDie) return;
    QRectF rec = boundingRect();
    QPixmap pixmap;
    if(id==1)
    {
        QImage srcImg(":/image/blackEle");
        QPoint center = srcImg.rect().center();
        QMatrix matrix;
        matrix.translate(center.x(), center.y());
        matrix.rotate(180);
        QImage dstImg = srcImg.transformed(matrix);
        pixmap = QPixmap::fromImage(dstImg);
    }
    else
        pixmap = QPixmap(":/image/redEle");
    painter->drawPixmap(rec,pixmap,QRectF(0,0,50,50));
}

bool Ele::canMove(int nx,int ny)
{
    if(id==0&&nx==x+1&&y==ny) return 1;
    if(id==1&&nx==x-1&&y==ny) return 1;
    return 0;
}


void Man::paint(QPainter *painter,const QStyleOptionGraphicsItem *option
, QWidget *widget)
{
    if(isDie) return;
    QRectF rec = boundingRect();
    QPixmap pixmap;
    if(id==1)
    {
        QImage srcImg(":/image/blackMan");
        QPoint center = srcImg.rect().center();
        QMatrix matrix;
        matrix.translate(center.x(), center.y());
        matrix.rotate(180);
        QImage dstImg = srcImg.transformed(matrix);
        pixmap = QPixmap::fromImage(dstImg);
    }
    else
        pixmap = QPixmap(":/image/redMan");
    painter->drawPixmap(rec,pixmap,QRectF(0,0,50,50));
}

bool Man::canMove(int nx,int ny)
{
    if(id==0&&nx==x+1&&y==ny) return 1;
    if(id==1&&nx==x-1&&y==ny) return 1;
    return 0;
}

void Boss::paint(QPainter *painter,const QStyleOptionGraphicsItem *option
, QWidget *widget)
{
    if(isDie) return;
    QRectF rec = boundingRect();
    QPixmap pixmap;
    if(id==1)
    {
        QImage srcImg(":/image/blackBoss");
        QPoint center = srcImg.rect().center();
        QMatrix matrix;
        matrix.translate(center.x(), center.y());
        matrix.rotate(180);
        QImage dstImg = srcImg.transformed(matrix);
        pixmap = QPixmap::fromImage(dstImg);
    }
    else
        pixmap = QPixmap(":/image/redBoss");
    painter->drawPixmap(rec,pixmap,QRectF(0,0,50,50));
}

bool Boss::canMove(int nx,int ny)
{
    if(id==0&&nx==x+1&&y==ny) return 1;
    if(id==1&&nx==x-1&&y==ny) return 1;
    return 0;
}
