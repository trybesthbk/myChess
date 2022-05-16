#include "ChessBoard.h"
#include "ui_ChessBoard.h"
#include <math.h>
#include <QPainter>
#include <QMessageBox>
#include <QPushButton>

ChessBoard::ChessBoard(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::ChessBoard),
      turnRed(true),selectId(-1),time_out(false)
{
    initPiece(0);
    initPiece(1);
    ui->setupUi(this);
    initLCD();
}

ChessBoard::~ChessBoard()
{
    for(int i=0;i<32;i++)
        delete pes[i];
    delete ui;
}

void ChessBoard::initPiece(bool red)
{
    int num;
    if(red)num=0;
    else num=16;

    pes[num+0]=new piece(C,red?0:9,0,!red);
    pes[num+1]=new piece(M,red?0:9,1,!red);
    pes[num+2]=new piece(X,red?0:9,2,!red);
    pes[num+3]=new piece(S,red?0:9,3,!red);
    pes[num+4]=new piece(J,red?0:9,4,!red);
    pes[num+5]=new piece(S,red?0:9,5,!red);
    pes[num+6]=new piece(X,red?0:9,6,!red);
    pes[num+7]=new piece(M,red?0:9,7,!red);
    pes[num+8]=new piece(C,red?0:9,8,!red);
    pes[num+9]=new piece(P,red?2:7,1,!red);
    pes[num+10]=new piece(P,red?2:7,7,!red);
    pes[num+11]=new piece(B,red?3:6,0,!red);
    pes[num+12]=new piece(B,red?3:6,2,!red);
    pes[num+13]=new piece(B,red?3:6,4,!red);
    pes[num+14]=new piece(B,red?3:6,6,!red);
    pes[num+15]=new piece(B,red?3:6,8,!red);
}

void ChessBoard::initLCD()
{
    m_timer=new QTimer;
    initTime();
    connect(m_timer,SIGNAL(timeout()),this,SLOT(updateTime()));
    m_timer->start(1000);
}

void ChessBoard::initTime()
{
    m_time=new QTime(0,0,0);
    ui->lcdNumber->display(m_time->toString("hh:mm:ss"));
}

void ChessBoard::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QRect rect(0,0,857,937);
    QImage image(":image/board.JPG");
    painter.drawImage(rect,image);
    painter.setFont(QFont("仿宋",RADIUS,2000));
    for(int i=0;i<32;i++)
    {
        if(pes[i]->getDead())continue;
        if(selectId==i)
            painter.setBrush(QColor(64,64,196,80));
        else
            painter.setBrush(QColor(64,64,196,10));
        QPoint pcent=center(i);
        painter.drawEllipse(pcent,RADIUS,RADIUS);
        if(i<16)
            painter.setPen(QColor(0, 0, 0));
        else
            painter.setPen(QColor(255, 0, 0));
        painter.drawText(getRect(pcent), GetName(i), QTextOption(Qt::AlignCenter));
    }
    if(turnRed)
    {
        painter.setPen(QColor(255, 0, 0));
        painter.drawText(QRect(750,350,500,200),"红方行",QTextOption(Qt::AlignCenter));
    }else{
        painter.setPen(QColor(0, 0, 0));
        painter.drawText(QRect(750,350,500,200),"黑方行",QTextOption(Qt::AlignCenter));
    }
    if(time_out)
        painter.drawText(QRect(750,200,500,200),"您已超时",QTextOption(Qt::AlignCenter));
}

void ChessBoard::mousePressEvent(QMouseEvent* ev)
{
    QPoint pt=ev->pos();
    int row=round(double(pt.y()-OFFSET)/INTERVAL);
    int col=round(double(pt.x()-OFFSET)/INTERVAL);
//    QString qs="X:"+QString::number(row)+",Y:"+QString::number(col);
//    QMessageBox::about(this,"title",qs);
    clickComment(row,col);
}

void ChessBoard::clickComment(int row,int col)
{
    if(row<0||row>9||col<0||col>8){
        selectId=-1;
        return;
    }
    //选棋子
    if(selectId==-1)
    {
        if(!turnRed)
            for(int i=0;i<16;i++)
            {
                if(pes[i]->getRow()==row&&pes[i]->getCol()==col&&!pes[i]->getDead())
                {
                    selectId=i;
                    update();
                    break;
                }
            }
        else
            for(int i=16;i<32;i++)
            {
                if(pes[i]->getRow()==row&&pes[i]->getCol()==col&&!pes[i]->getDead())
                {
                    selectId=i;
                    update();
                    break;
                }
            }
    }
    //走棋子
    else{
        if(movePiece(selectId,row,col))
        {
            int target=checkPiece(row,col);
            if(!((target<16)^turnRed)||(target==-1))
            {
                if(target!=-1)
                    pes[target]->goDead();
                pes[selectId]->setRow(row);
                pes[selectId]->setCol(col);
                turnRed=!turnRed;
                //如果被将军
                if(target==4||target==20)
                {
                    if(target==4)
                        QString result="红方胜利";
                    else
                        QString result="黑方胜利";
                    int ret = QMessageBox::question(this,
                                "结果", "红方胜利","关闭","重新",0,1);
                     switch(ret)
                     {
                     case 0:
                         this->close();
                         break;
                     case 1:
                         reset();
                         break;
                     default:
                         QMessageBox::critical(this,"error!","出错了");
                         this->close();
                         break;
                     }
                }
                time_out=false;
                initTime();
            }
        }
        selectId=-1;
        update();
    }
}

bool ChessBoard::movePiece(int selectId,int toRow,int toCol)
{
    switch (pes[selectId]->getType()) {
        case C:
            return moveChe(selectId,toRow,toCol);
        case M:
            return moveMa(selectId,toRow,toCol);
        case X:
            return moveXiang(selectId,toRow,toCol);
        case S:
            return moveShi(selectId,toRow,toCol);
        case J:
            return moveJiang(selectId,toRow,toCol);
        case P:
            return movePao(selectId,toRow,toCol);
        case B:
            return moveBing(selectId,toRow,toCol);
        default:
            break;
    }
    return false;
}

void ChessBoard::reset()
{
    initPiece(0);
    initPiece(1);
    m_time->setHMS(0,0,0);
    selectId=-1;
    turnRed=true;
}

bool ChessBoard::moveChe(int selectId,int toRow,int toCol)
{
    int fromRow=pes[selectId]->getRow(),fromCol=pes[selectId]->getCol();
    if(fromRow==toRow&&fromCol==toCol)
        return false;
    if(fromRow!=toRow&&fromCol!=toCol)
        return false;
    return atLine(fromRow,fromCol,toRow,toCol,false);
}

bool ChessBoard::moveMa(int selectId,int toRow,int toCol)
{
    int fromRow=pes[selectId]->getRow(),fromCol=pes[selectId]->getCol();
    if(pow(toRow-fromRow,2)+pow(toCol-fromCol,2)==5)
    {
        if(checkPiece(fromRow+(toRow-fromRow)/2,fromCol+(toCol-fromCol)/2)!=-1)
            return false;
        return true;
    }
    return false;
}

bool ChessBoard::moveXiang(int selectId,int toRow,int toCol)
{
    int fromRow=pes[selectId]->getRow(),fromCol=pes[selectId]->getCol();
    if((fromRow>4)^(toRow>4))return false;
    if(pow(toRow-fromRow,2)+pow(toCol-fromCol,2)==8)
    {
        if(checkPiece((fromRow+toRow)/2,(fromCol+toCol)/2)!=-1)
            return false;
        return true;
    }
    return false;
}

bool ChessBoard::moveShi(int selectId,int toRow,int toCol)
{
    int fromRow=pes[selectId]->getRow(),fromCol=pes[selectId]->getCol();
    if((toRow>2&&toRow<7)||toCol<3||toCol>5)return false;
    if(pow(toRow-fromRow,2)+pow(toCol-fromCol,2)==2)
        return true;
    return false;
}

bool ChessBoard::moveJiang(int selectId,int toRow,int toCol)
{
    int fromRow=pes[selectId]->getRow(),fromCol=pes[selectId]->getCol();
    if((toRow>2&&toRow<7)||toCol<3||toCol>5)return false;
    if(pow(toRow-fromRow,2)+pow(toCol-fromCol,2)==1)
        return true;
    return false;
}

bool ChessBoard::movePao(int selectId,int toRow,int toCol)
{
    int fromRow=pes[selectId]->getRow(),fromCol=pes[selectId]->getCol();
    if(fromRow==toRow&&fromCol==toCol)
        return false;
    if(fromRow!=toRow&&fromCol!=toCol)
        return false;
    if(checkPiece(toRow,toCol)==-1)
    {
        return atLine(fromRow,fromCol,toRow,toCol,false);
    }else{
        return atLine(fromRow,fromCol,toRow,toCol,true);
    }
    return true;
}

bool ChessBoard::moveBing(int selectId,int toRow,int toCol)
{
    int fromRow=pes[selectId]->getRow(),fromCol=pes[selectId]->getCol();
    if(pow(toRow-fromRow,2)+pow(toCol-fromCol,2)!=1)
        return false;
    if(pes[selectId]->getColor())
    {
        if(pes[selectId]->getRow()>4&&fromRow-toRow!=1)
            return false;
        if(pes[selectId]->getRow()<5&&toRow-fromRow==1)
            return false;
    }else{
        if(pes[selectId]->getRow()<5&&toRow-fromRow!=1)
            return false;
        if(pes[selectId]->getRow()>4&&fromRow-toRow==1)
            return false;
    }
    return true;
}

bool ChessBoard::atLine(int fromRow,int fromCol,int toRow,int toCol,bool needGap)
{
    if(fromRow==toRow)
    {
        int minCol,maxCol;
        if(fromCol<toCol)
        {
            minCol=fromCol;
            maxCol=toCol;
        }else{
            minCol=toCol;
            maxCol=fromCol;
        }
        if(!checkPiece(fromRow,minCol,maxCol,1,needGap))
            return false;
    }else{
        int minRow,maxRow;
        if(fromRow<toRow)
        {
            minRow=fromRow;
            maxRow=toRow;
        }else{
            minRow=toRow;
            maxRow=fromRow;
        }
        if(!checkPiece(minRow,maxRow,fromCol,0,needGap))
            return false;
    }
    return true;
}


int ChessBoard::checkPiece(int row, int col)
{
    for(int i=0;i<32;i++)
    {
        if(!pes[i]->getDead()&&pes[i]->getRow()==row&&pes[i]->getCol()==col)
            return i;
    }
    return -1;
}

bool ChessBoard::checkPiece(int row,int rowcol,int col,bool isrow,bool needGap)
{
    bool gap=false;
    if(isrow)
    {
        for(int i=0;i<32;i++)
        {
            if(!pes[i]->getDead()&&pes[i]->getRow()==row&&pes[i]->getCol()>rowcol&&pes[i]->getCol()<col)
            {
                if(!needGap||gap)
                    return false;
                else
                    gap=true;
            }
        }
    }else{
        for(int i=0;i<32;i++)
        {
            if(!pes[i]->getDead()&&pes[i]->getCol()==col&&pes[i]->getRow()>row&&pes[i]->getRow()<rowcol)
            {
                if(!needGap||gap)
                    return false;
                else
                    gap=true;
            }
        }
    }
    if(needGap&&!gap)
        return false;
    return true;
}

QString ChessBoard::GetName(int n)
{
    bool red=pes[n]->getColor();
    switch (pes[n]->getType()) {
        case C:
            return "車";
        case M:
            return "馬";
        case X:
            if(red)
                return "相";
            else
                return "象";
        case S:
            if(red)
                return "仕";
            else
                return "士";
        case J:
            if(red)
                return "帅";
            else
                return "将";
        case P:
            if(red)
                return "炮";
            else
                return "砲";
        case B:
            if(red)
                return "兵";
            else
                return "卒";
    }
    return "";
}

void ChessBoard::updateTime()
{
    *m_time=m_time->addSecs(1);
    if(m_time->second()==10)
    {
        time_out=true;
        update();
    }
    ui->lcdNumber->display(m_time->toString("hh:mm:ss"));
}
