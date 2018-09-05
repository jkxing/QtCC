#include "game.h"
#include <QTextStream>
void Game::updateMap()
{
    memset(mp,-1,sizeof(mp));
    for(int i=0;i<16;i++)
    {
        if(pieces[0][i]->isDie==0)
            mp[pieces[0][i]->i][pieces[0][i]->j]=0;
        if(pieces[1][i]->isDie==0)
            mp[pieces[1][i]->i][pieces[1][i]->j]=1;
    }
}
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
    updateMap();
    //QByteArray arr;
    //receivedData(arr);
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
        updateMap();

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
            int cnt=0;
            if(pieces[0][15]->j==pieces[1][15]->j&&piece->j==pieces[1][15]->j&&j!=piece->j)
            {
                for(int k=pieces[1][15]->i+1;k<pieces[0][15]->i;k++)
                    if(mp[k][pieces[1][15]->j]!=-1)
                    {
                        cnt++;
                        if(piece->At(k,pieces[1][15]->j)) cnt--;
                    }
                if(cnt==0) continue;
            }
            if(piece->canMove(i,j,mp))
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
    updateMap();
}

void Game::receivedData(QByteArray arr)
{
    arr.clear();
    arr.append("1111");
    for(int i=0;i<3;i++)
        qDebug()<<"checkarr "<<i<<" "<<arr.at(i);

    if(arr.at(0)=='1')
    {
        int t = arr.at(1)-'0';
        int x = 9-arr.at(2)+'0';
        int y = 8-arr.at(3)+'0';
        scene->removeItem(pieces[1][t]);
        pieces[1][t]->changePosition(x,y);
        scene->addItem(pieces[1][t]);
        for(int i=0;i<16;i++)
            if(pieces[0][i]->At(x,y))
                pieces[0][i]->isDie=1;
        isWaiting = 0;
    }
    if(arr.at(0)=='2')
    {
        arr.remove(0,1);
        QFile *file = new QFile("D:/canju.txt");
        isWaiting = 1;
        if(file->open(QIODevice::ReadWrite))
        {
            file->write(arr);
            file->close();
        }
        loadFromFile(file);
        /*for(int i=0;i<16;i++)
        {
            int x=9-arr.at(i*2+1)+'0';
            int y=8-arr.at(i*2+2)+'0';
            scene->removeItem(pieces[1][i]);
            pieces[1][i]->changePosition(x,y);
            scene->addItem(pieces[1][i]);
        }
        for(int i=0;i<16;i++)
        {
            int x=9-arr.at(i*2+33)+'0';
            int y=8-arr.at(i*2+34)+'0';
            scene->removeItem(pieces[0][i]);
            pieces[0][i]->changePosition(x,y);
            scene->addItem(pieces[0][i]);
        }
        int t = arr.at(65)-'0';
        if(t==0) isWaiting = 1;
        else isWaiting = 0;*/
    }

}

Game::Game(QGraphicsScene *scene, QFile *file):scene(scene)
{
    scene->clear();
    isWaiting = 0;
    loadFromFile(file);
}

void Game::loadFromFile(QFile *file)
{
    if(!file->open(QIODevice::WriteOnly))
    {
        qDebug()<<"load error";
        return;
    }
    QTextStream in(file);
    QString str;
    if(isWaiting)
    {
        while(1)
        {
            in>>str;
            if(str=="black")
                break;
        }
    }
    else
    {
        while(1)
        {
            in>>str;
            if(str=="red")
                break;
        }

    }
}
