#include "SelectMode.h"
#include <QVBoxLayout>
#include <QDebug>
#include <QMessageBox>

SelectMode::SelectMode(QWidget *parent):QDialog(parent)
{
    QVBoxLayout* lay=new QVBoxLayout(this);

    lay->addWidget(btn[0]=new QPushButton("与自己对战"));
    lay->addWidget(btn[1]=new QPushButton("与电脑对战"));
    lay->addWidget(btn[2]=new QPushButton("网络对战"));

    for(unsigned char i=0;i<3;i++)
    {
       connect(btn[i],SIGNAL(clicked()),this,SLOT(ClickSlot()));
    }
}

SelectMode::~SelectMode()
{
    for(int i=0;i<3;++i)
        delete btn[i];
}

void SelectMode::ClickSlot()
{
    QObject* s=sender();
    if(btn[0]==s)
    {
        cb=new ChessBoard;
        cb->show();
    }
    if(btn[2]==s)
    {
        int ret=QMessageBox::question(this,"打开模式","选择作为服务器/客户端启动","服务器","客户端",0,1);
        switch(ret)
        {
            case 0:
                nc=new NetChess(1);
                break;
            case 1:
                nc=new NetChess(0);
                break;
        }
        nc->show();
    }
    accept();
}
