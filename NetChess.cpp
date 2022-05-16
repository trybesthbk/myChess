#include "NetChess.h"
#include <QInputDialog>
#include <QStringList>


NetChess::NetChess(bool is):ChessBoard(nullptr),isServer(is),m_server(nullptr),m_socket(nullptr)
{
    if(isServer)
    {
        int res=QInputDialog::getInt(this,"无","端口号（例如9999）");
        m_server=new QTcpServer(this);
        m_server->listen(QHostAddress::Any,res);
        connect(m_server,SIGNAL(newConnection()),this,SLOT(recvConnect()));
    }else{
        QString text=QInputDialog::getText(this,"无","IP与端口号(例如127.0.0.1,9999)");
        QStringList qsl;
        for(int gap=0;gap<text.size();gap++)
        {
            if(text[gap]==','){
                qsl=text.split(",");
            }
        }
        m_socket=new QTcpSocket(this);
        m_socket->connectToHost(QHostAddress(qsl[0]),qsl[1].toInt());
        connect(m_socket,SIGNAL(readyRead()),this,SLOT(slotRecv()));
    }
}

NetChess::~NetChess()
{
    delete  m_server;
    delete  m_socket;
}

void NetChess::clickComment(int row,int col)
{
    if(m_socket)
    {
        if(isServer^turnRed)return;
        ChessBoard::clickComment(row,col);
        char arry[2];
        arry[0]=row;
        arry[1]=col;
        m_socket->write(arry,2);
    }
}

void NetChess::recvConnect()
{
    if(m_socket)
        return;
    m_socket=m_server->nextPendingConnection();
    connect(m_socket,SIGNAL(readyRead()),this,SLOT(slotRecv()));
}

void NetChess::slotRecv()
{
    QByteArray arry=m_socket->readAll();
    ChessBoard::clickComment(arry[0],arry[1]);
}
