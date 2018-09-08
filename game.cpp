#include "game.h"
#include <QTextStream>
#include <QMessageBox>
#include <QApplication>
void Game::updateMap(bool flag)
{
    memset(mp,-1,sizeof(mp));
    for(int i=0;i<16;i++)
    {
        if(pieces[0][i]->isDie==0)
            mp[pieces[0][i]->i][pieces[0][i]->j]=0;
        if(pieces[1][i]->isDie==0)
            mp[pieces[1][i]->i][pieces[1][i]->j]=1;
        pieces[0][i]->update();
        pieces[1][i]->update();
    }
    for(int i=0;i<16;i++)
    {
       // qDebug()<<i<<" "<<pieces[1][i]->i<<" "<<pieces[1][i]->j<<" "<<pieces[1][i]->canMove(pieces[0][15]->i,pieces[0][15]->j,mp);
        if(flag&&pieces[1][i]->isDie==0&&pieces[1][i]->canMove(pieces[0][15]->i,pieces[0][15]->j,mp))
            QApplication::beep();
    }
}
Game::Game(QGraphicsScene* scene,bool identity,QFile* file):scene(scene){
    //scene->addRect(QRectF(0,0,100,100));
    isWaiting = identity^1;
    isFirst = identity;
    scene->clear();
    for(int i=0;i<5;i++)
    {
        pieces[1][i] = new Soldier(Pos[3][(4-i)*2],1);
    }
    pieces[1][5]=new Horse(Pos[0][7],1);
    pieces[1][6]=new Horse(Pos[0][1],1);
    pieces[1][7]=new Car(Pos[0][8],1);
    pieces[1][8]=new Car(Pos[0][0],1);
    pieces[1][9]=new Pao(Pos[2][7],1);
    pieces[1][10]=new Pao(Pos[2][1],1);
    pieces[1][11]=new Ele(Pos[0][6],1);
    pieces[1][12]=new Ele(Pos[0][2],1);
    pieces[1][13]=new Man(Pos[0][5],1);
    pieces[1][14]=new Man(Pos[0][3],1);
    pieces[1][15]=new Boss(Pos[0][4],1);
    for(int i=0;i<16;i++)
        scene->addItem(pieces[1][i]);
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
        QObject::connect(pieces[0][i],SIGNAL(deselect(BasicPiece*)),this,SLOT(cancelPosition()));
    }
    qDebug()<<file;
    if(file!=nullptr)
    {
        loadFromFile(file);
    }
    updateMap();
    qDebug()<<mp[3][8];
}

void Game::start()
{
    qDebug()<<"checking identity "<<isWaiting;
    if(isWaiting == 0)
        emit startTimeLimit();
}

void Game::possiblePosition(BasicPiece* piece)
{
    qDebug()<<piece->i<<" "<<piece->j;
    if(piece->id==2)
    {
        if(isWaiting) return;
        qDebug()<<"move";
        bool flag=0;
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
                        if(k==15)
                            flag=1;
                    }
                scene->addItem(pieces[0][i]);
                qDebug()<<"working "<<i;
                cancelPosition();
                QByteArray arr;
                arr.clear();
                arr.append("1");
                if(i<10)
                    arr.append("0");
                arr.append(QString::number(i));
                arr.append(QString::number(piece->i));
                arr.append(QString::number(piece->j));
                emit sendData(arr);
                emit stopTimeLimit();
                isWaiting = 1;
                break;
            }
        if(flag)
        {
            emit win();
            return;
        }
        updateMap();
        return;
    }
    for(int i=0;i<16;i++)
        if(pieces[0][i]!=piece&&pieces[0][i]->isSelect)
        {
            pieces[0][i]->isSelect=0;
            cancelPosition();
            break;
        }
    qDebug()<<piece->i<<" "<<piece->j;
    for(int i=0;i<10;i++)
    {
        for(int j=0;j<9;j++)
        {
            int cnt=0;
            if(piece!=pieces[0][15])
            {
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
            }
            else
            {
                if(j==pieces[1][15]->j)
                {
                    for(int k=pieces[1][15]->i+1;k<pieces[0][15]->i;k++)
                        if(mp[k][pieces[1][15]->j]!=-1)
                        {
                            cnt++;
                        }
                    if(cnt==0) continue;
                }
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

char* Game::toChar(int i,int j)
{
    char* ch = new char(7);
    ch[0]='<';
    ch[1]=8-j+'0';
    ch[2]=',';
    ch[3]=9-i+'0';
    ch[4]='>';
    ch[5]=' ';
    ch[6]='\0';
    return ch;
}
char* Game::toChar(int i)
{
    char* ch = new char(3);
    ch[0]=i+'0';
    ch[1]=' ';
    ch[2]='\0';
    return ch;
}
void Game::save()
{
    QFile file("D:/canju.txt");
    file.open(QIODevice::WriteOnly);
    file.write("red\n");
    if(pieces[0][15]->isDie==0)
    {
        file.write("1 ");
        file.write(toChar(pieces[0][15]->i,pieces[0][15]->j));
        file.write("\n");
    }
    else file.write("0\n");
    int cnt=0;
    for(int i=0;i<2;i++)
        if(pieces[0][13+i]->isDie==0) cnt++;
    file.write(toChar(cnt));
    for(int i=0;i<2;i++)
        if(pieces[0][13+i]->isDie==0)
            file.write(toChar(pieces[0][13+i]->i,pieces[0][13+i]->j));
    file.write("\n");
    cnt=0;
    for(int i=0;i<2;i++)
        if(pieces[0][11+i]->isDie==0) cnt++;
    file.write(toChar(cnt));
    for(int i=0;i<2;i++)
        if(pieces[0][11+i]->isDie==0)
            file.write(toChar(pieces[0][11+i]->i,pieces[0][11+i]->j));
    file.write("\n");
    cnt=0;
    for(int i=0;i<2;i++)
        if(pieces[0][5+i]->isDie==0) cnt++;
    file.write(toChar(cnt));
    for(int i=0;i<2;i++)
        if(pieces[0][5+i]->isDie==0)
            file.write(toChar(pieces[0][5+i]->i,pieces[0][5+i]->j));
    file.write("\n");
    cnt=0;
    for(int i=0;i<2;i++)
        if(pieces[0][7+i]->isDie==0) cnt++;
    file.write(toChar(cnt));
    for(int i=0;i<2;i++)
        if(pieces[0][7+i]->isDie==0)
            file.write(toChar(pieces[0][7+i]->i,pieces[0][7+i]->j));
    file.write("\n");
    cnt=0;
    for(int i=0;i<2;i++)
        if(pieces[0][9+i]->isDie==0) cnt++;
    file.write(toChar(cnt));
    for(int i=0;i<2;i++)
        if(pieces[0][9+i]->isDie==0)
            file.write(toChar(pieces[0][9+i]->i,pieces[0][9+i]->j));
    file.write("\n");
    cnt=0;
    for(int i=0;i<5;i++)
        if(pieces[0][i]->isDie==0) cnt++;
    file.write(toChar(cnt));
    for(int i=0;i<5;i++)
        if(pieces[0][i]->isDie==0)
            file.write(toChar(pieces[0][i]->i,pieces[0][i]->j));
    file.write("\n");
    file.write("black\n");
    if(pieces[1][15]->isDie==0)
    {
        file.write("1 ");
        file.write(toChar(pieces[1][15]->i,pieces[1][15]->j));
        file.write("\n");
    }
    else file.write("0\n");
    cnt=0;
    for(int i=0;i<2;i++)
        if(pieces[1][13+i]->isDie==0) cnt++;
    file.write(toChar(cnt));
    for(int i=0;i<2;i++)
        if(pieces[1][13+i]->isDie==0)
            file.write(toChar(pieces[1][13+i]->i,pieces[1][13+i]->j));
    file.write("\n");
    cnt=0;
    for(int i=0;i<2;i++)
        if(pieces[0][11+i]->isDie==0) cnt++;
    file.write(toChar(cnt));
    for(int i=0;i<2;i++)
        if(pieces[1][11+i]->isDie==0)
            file.write(toChar(pieces[1][11+i]->i,pieces[1][11+i]->j));
    file.write("\n");
    cnt=0;
    for(int i=0;i<2;i++)
        if(pieces[1][5+i]->isDie==0) cnt++;
    file.write(toChar(cnt));
    for(int i=0;i<2;i++)
        if(pieces[1][5+i]->isDie==0)
            file.write(toChar(pieces[1][5+i]->i,pieces[1][5+i]->j));
    file.write("\n");
    cnt=0;
    for(int i=0;i<2;i++)
        if(pieces[1][7+i]->isDie==0) cnt++;
    file.write(toChar(cnt));
    for(int i=0;i<2;i++)
        if(pieces[1][7+i]->isDie==0)
            file.write(toChar(pieces[1][7+i]->i,pieces[1][7+i]->j));
    file.write("\n");
    cnt=0;
    for(int i=0;i<2;i++)
        if(pieces[1][9+i]->isDie==0) cnt++;
    file.write(toChar(cnt));
    for(int i=0;i<2;i++)
        if(pieces[1][9+i]->isDie==0)
            file.write(toChar(pieces[1][9+i]->i,pieces[1][9+i]->j));
    file.write("\n");
    cnt=0;
    for(int i=0;i<5;i++)
        if(pieces[1][i]->isDie==0) cnt++;
    file.write(toChar(cnt));
    for(int i=0;i<5;i++)
        if(pieces[1][i]->isDie==0)
            file.write(toChar(pieces[1][i]->i,pieces[1][i]->j));
    file.write("\n");
    file.close();
    return;

}
void Game::cancelPosition()
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
    qDebug()<<"game handling data: "<<arr;
    if(arr.at(0)=='1')
    {
        int t = (arr.at(1)-'0')*10+arr.at(2)-'0';
        int x = 9-arr.at(3)+'0';
        int y = 8-arr.at(4)+'0';
        qDebug()<<t<<" "<<x<<" "<<y;
        scene->removeItem(pieces[1][t]);
        pieces[1][t]->changePosition(x,y);
        scene->addItem(pieces[1][t]);
        for(int i=0;i<16;i++)
            if(pieces[0][i]->At(x,y))
             {
                pieces[0][i]->isDie=1;
                if(i==15)
                {
                    emit lose();
                    return;
                }
             }
        isWaiting = 0;
        emit startTimeLimit();
    }
    updateMap(1);
}

void Game::loadFromFile(QFile *file)
{
    //scene->clear();
    if(!file->open(QIODevice::ReadOnly))
    {
        qDebug()<<"load error";
        return;
    }
    QTextStream in(file);
    QString str;
    in>>str;
    int num,x,y;
    in>>num;
    in>>str;
    y=str.toStdString()[1]-'0';
        x=str.toStdString()[3]-'0';
        qDebug()<<num<<" "<<x<<" "<<y;
        if(!isWaiting)
        {
            x=9-x;
            y=8-y;
            pieces[0][15]->changePosition(x,y);
        }
        else
        {
            pieces[1][15]->changePosition(x,y);
        }
        in>>num;
        for(int i=0;i<num;i++)
        {
            in>>str;
            y=str.toStdString()[1]-'0';
            x=str.toStdString()[3]-'0';
            if(!isWaiting)
            {
                x=9-x;
                y=8-y;
                pieces[0][13+i]->changePosition(x,y);
            }
            else
            {
                pieces[1][13+i]->changePosition(x,y);
            }
        }
        for(int i=13+num;i<15;i++)
        {
            if(!isWaiting)
                pieces[0][i]->isDie=1;
            else
                pieces[1][i]->isDie=1;
        }

        in>>num;
        for(int i=0;i<num;i++)
        {
            in>>str;
            y=str.toStdString()[1]-'0';
            x=str.toStdString()[3]-'0';
            if(!isWaiting)
            {
                x=9-x;
                y=8-y;
                pieces[0][11+i]->changePosition(x,y);
            }
            else
            {
                pieces[1][11+i]->changePosition(x,y);
            }
        }
        for(int i=11+num;i<13;i++)
        {
            if(!isWaiting)
                pieces[0][i]->isDie=1;
            else
                pieces[1][i]->isDie=1;
        }


        in>>num;
        for(int i=0;i<num;i++)
        {
            in>>str;
            y=str.toStdString()[1]-'0';
            x=str.toStdString()[3]-'0';
            if(!isWaiting)
            {
                x=9-x;
                y=8-y;
                pieces[0][5+i]->changePosition(x,y);
            }
            else
            {
                pieces[1][5+i]->changePosition(x,y);
            }
        }
        for(int i=5+num;i<7;i++)
        {
            if(!isWaiting)
                pieces[0][i]->isDie=1;
            else
                pieces[1][i]->isDie=1;
        }

        in>>num;
        for(int i=0;i<num;i++)
        {
            in>>str;
            y=str.toStdString()[1]-'0';
            x=str.toStdString()[3]-'0';
            if(!isWaiting)
            {
                x=9-x;
                y=8-y;
                pieces[0][7+i]->changePosition(x,y);
            }
            else
            {
                pieces[1][7+i]->changePosition(x,y);
            }
        }
        for(int i=7+num;i<9;i++)
        {
            if(!isWaiting)
                pieces[0][i]->isDie=1;
            else
                pieces[1][i]->isDie=1;
        }

        in>>num;
        for(int i=0;i<num;i++)
        {
            in>>str;
            y=str.toStdString()[1]-'0';
            x=str.toStdString()[3]-'0';
            if(!isWaiting)
            {
                x=9-x;
                y=8-y;
                pieces[0][9+i]->changePosition(x,y);
            }
            else
            {
                pieces[1][9+i]->changePosition(x,y);
            }
        }
        for(int i=9+num;i<11;i++)
        {
            if(!isWaiting)
                pieces[0][i]->isDie=1;
            else
                pieces[1][i]->isDie=1;
        }
        in>>num;
        for(int i=0;i<num;i++)
        {
            in>>str;
            y=str.toStdString()[1]-'0';
            x=str.toStdString()[3]-'0';
            if(!isWaiting)
            {
                x=9-x;
                y=8-y;
                pieces[0][i]->changePosition(x,y);
            }
            else
            {
                pieces[1][i]->changePosition(x,y);
            }
        }
        for(int i=num;i<5;i++)
        {
            if(!isWaiting)
                pieces[0][i]->isDie=1;
            else
                pieces[1][i]->isDie=1;
        }

        in>>str;

        in>>num;
            in>>str;
            y=str.toStdString()[1]-'0';
            x=str.toStdString()[3]-'0';
            if(isWaiting)
            {
                pieces[0][15]->changePosition(x,y);
            }
            else
            {
                x=9-x;
                y=8-y;
                pieces[1][15]->changePosition(x,y);
            }

            in>>num;
            for(int i=0;i<num;i++)
            {
                in>>str;
                y=str.toStdString()[1]-'0';
                x=str.toStdString()[3]-'0';
                if(isWaiting)
                {
                    pieces[0][13+i]->changePosition(x,y);
                }
                else
                {
                    x=9-x;
                    y=8-y;
                    pieces[1][13+i]->changePosition(x,y);
                }
            }
            for(int i=13+num;i<15;i++)
            {
                if(isWaiting)
                    pieces[0][i]->isDie=1;
                else
                    pieces[1][i]->isDie=1;
            }

            in>>num;
            for(int i=0;i<num;i++)
            {
                in>>str;
                y=str.toStdString()[1]-'0';
                x=str.toStdString()[3]-'0';
                if(isWaiting)
                {
                    pieces[0][11+i]->changePosition(x,y);
                }
                else
                {
                    x=9-x;
                    y=8-y;
                    pieces[1][11+i]->changePosition(x,y);
                }
            }
            for(int i=11+num;i<13;i++)
            {
                if(isWaiting)
                    pieces[0][i]->isDie=1;
                else
                    pieces[1][i]->isDie=1;
            }


            in>>num;
            for(int i=0;i<num;i++)
            {
                in>>str;
                y=str.toStdString()[1]-'0';
                x=str.toStdString()[3]-'0';
                if(isWaiting)
                {
                    pieces[0][5+i]->changePosition(x,y);
                }
                else
                {
                    x=9-x;
                    y=8-y;
                    pieces[1][5+i]->changePosition(x,y);
                }
            }
            for(int i=5+num;i<7;i++)
            {
                if(isWaiting)
                    pieces[0][i]->isDie=1;
                else
                    pieces[1][i]->isDie=1;
            }

            in>>num;
            for(int i=0;i<num;i++)
            {
                in>>str;
                y=str.toStdString()[1]-'0';
                x=str.toStdString()[3]-'0';
                if(isWaiting)
                {
                    pieces[0][7+i]->changePosition(x,y);
                }
                else
                {
                    x=9-x;
                    y=8-y;
                    pieces[1][7+i]->changePosition(x,y);
                }
            }
            for(int i=7+num;i<9;i++)
            {
                if(isWaiting)
                    pieces[0][i]->isDie=1;
                else
                    pieces[1][i]->isDie=1;
            }

            in>>num;
            for(int i=0;i<num;i++)
            {
                in>>str;
                y=str.toStdString()[1]-'0';
                x=str.toStdString()[3]-'0';
                if(isWaiting)
                {
                    pieces[0][9+i]->changePosition(x,y);
                }
                else
                {
                    x=9-x;
                    y=8-y;
                    pieces[1][9+i]->changePosition(x,y);
                }
            }
            for(int i=9+num;i<11;i++)
            {
                if(isWaiting)
                    pieces[0][i]->isDie=1;
                else
                    pieces[1][i]->isDie=1;
            }
            in>>num;
            for(int i=0;i<num;i++)
            {
                in>>str;
                y=str.toStdString()[1]-'0';
                x=str.toStdString()[3]-'0';
                if(isWaiting)
                {
                    pieces[0][i]->changePosition(x,y);
                }
                else
                {
                    x=9-x;
                    y=8-y;
                    pieces[1][i]->changePosition(x,y);
                }
            }
            for(int i=num;i<5;i++)
            {
                if(isWaiting)
                    pieces[0][i]->isDie=1;
                else
                    pieces[1][i]->isDie=1;
            }
       for(int i=0;i<16;i++)
       {
           //if(pieces[0][i]->isDie) scene->removeItem(pieces[0][i]);
           //if(pieces[1][i]->isDie) scene->removeItem(pieces[1][i]);
       }
}
