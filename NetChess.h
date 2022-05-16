#ifndef NETCHESS_H
#define NETCHESS_H

#include <QTcpServer>
#include <QTcpSocket>
#include "ChessBoard.h"

class NetChess:public ChessBoard
{
    Q_OBJECT
public:
    NetChess(bool is);
    ~NetChess();
protected:
    virtual void clickComment(int row,int col);
private:
    bool isServer;
    QTcpServer* m_server;
    QTcpSocket* m_socket;

private slots:
    void recvConnect();
    void slotRecv();
};

#endif // NETCHESS_H
