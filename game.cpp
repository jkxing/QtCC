#include "game.h"


Game::Game(QGraphicsScene* scene):scene(scene){
    //scene->addRect(QRectF(0,0,100,100));
    for(int i=0;i<5;i++)
        pieces[0][i] = new Soldier(Pos[6][i*2],0);
    pieces[0][5]=new Horse(Pos[9][1],0);
    pieces[0][6]=new Horse(Pos[9][7],0);
    pieces[0][7]=new Car(Pos[9][0],0);
    pieces[0][8]=new Car(Pos[9][8],0);
    pieces[0][9]=new Pao(Pos[7][1],0);
    pieces[0][10]=new Pao(Pos[7][7],0);
    pieces[0][11]=new Ele(Pos[9][2],0);
    pieces[0][12]=new Ele(Pos[9][6],0);
    pieces[0][13]=new Man(Pos[9][3],0);
    pieces[0][14]=new Man(Pos[9][5],0);
    pieces[0][15]=new Boss(Pos[9][4],0);
    memset(selected,0,sizeof(selected));
    for(int i=0;i<16;i++)
    {
        scene->addItem(pieces[0][i]);
        QObject::connect(pieces[0][i],SIGNAL(select(BasicPiece*)),this,SLOT(possiblePosition(BasicPiece*)));
        QObject::connect(pieces[0][i],SIGNAL(deselect(BasicPiece*)),this,SLOT(cancelPosition(BasicPiece*)));
    }
    for(int i=0;i<5;i++)
    {
        pieces[1][i] = new Soldier(Pos[3][i*2],1);
    }
    pieces[1][5]=new Horse(Pos[0][1],1);
    pieces[1][6]=new Horse(Pos[0][7],1);
    pieces[1][7]=new Car(Pos[0][0],1);
    pieces[1][8]=new Car(Pos[0][8],1);
    pieces[1][9]=new Pao(Pos[2][1],1);
    pieces[1][10]=new Pao(Pos[2][7],1);
    pieces[1][11]=new Ele(Pos[0][2],1);
    pieces[1][12]=new Ele(Pos[0][6],1);
    pieces[1][13]=new Man(Pos[0][3],1);
    pieces[1][14]=new Man(Pos[0][5],1);
    pieces[1][15]=new Boss(Pos[0][4],1);
    for(int i=0;i<16;i++)
        scene->addItem(pieces[1][i]);
}

void Game::possiblePosition(BasicPiece* piece)
{
    if(piece->id==2)
    {
        qDebug()<<"select select";
        for(int i=0;i<16;i++)
            if(pieces[0][i]->isSelect)
            {
                scene->removeItem(pieces[0][i]);
                pieces[0][i]->isSelect = 0;
                pieces[0][i]->changePosition(piece->i,piece->j);
                for(int k=0;k<16;k++)
                    if(pieces[1][k]->At(piece->i,piece->j))
                    {
                        pieces[1][k]->isDie = 1;
                    }
                scene->addItem(pieces[0][i]);
                qDebug()<<"working "<<i;
                cancelPosition(pieces[0][i]);
            }
        return;
    }
    for(int i=0;i<16;i++)
        if(pieces[0][i]!=piece&&pieces[0][i]->isSelect)
        {
            pieces[0][i]->isSelect=0;
            cancelPosition(pieces[0][i]);
            break;
        }
    for(int i=0;i<10;i++)
    {
        for(int j=0;j<9;j++)
        {
            bool flag=0;
            for(int k=0;k<16;k++)
                if(pieces[0][k]->At(i,j))
                {
                    flag=1;
                    break;
                }
            if(flag) continue;
            if(piece->canMove(i,j))
            {
                selected[i][j] = new BasicPiece(Pos[i][j],2);
                scene->addItem(selected[i][j]);
                QObject::connect(selected[i][j],SIGNAL(select(BasicPiece*)),this,SLOT(possiblePosition(BasicPiece*)));
            }
        }
    }
    updateMap();
}

void Game::cancelPosition(BasicPiece* piece)
{
    for(int i=0;i<10;i++)
    {
        for(int j=0;j<9;j++)
        {
            if(selected[i][j]!=nullptr)
            {
                scene->removeItem(selected[i][j]);
                delete selected[i][j];
                selected[i][j] = nullptr;
            }
        }
    }
}
