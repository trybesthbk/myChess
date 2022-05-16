
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QMainWindow>
#include <QMouseEvent>
#include <QTime>
#include <QTimer>
#include "piece.h"

QT_BEGIN_NAMESPACE
namespace Ui { class ChessBoard; }
QT_END_NAMESPACE

#define RADIUS 45
#define INTERVAL 90
#define OFFSET 60

class ChessBoard : public QMainWindow
{
    Q_OBJECT

public:
    ChessBoard(QWidget *parent = Q_NULLPTR);
    ~ChessBoard();
protected:
    virtual void clickComment(int row,int col);
    bool turnRed;
private:
    Ui::ChessBoard *ui;
    virtual void paintEvent(QPaintEvent *);
    virtual void mousePressEvent(QMouseEvent*);
    bool movePiece(int selectId,int toRow,int toCol);
    bool moveChe(int selectId,int toRow,int toCol);
    bool moveMa(int selectId,int toRow,int toCol);
    bool moveXiang(int selectId,int toRow,int toCol);
    bool moveShi(int selectId,int toRow,int toCol);
    bool moveJiang(int selectId,int toRow,int toCol);
    bool movePao(int selectId,int toRow,int toCol);
    bool moveBing(int selectId,int toRow,int toCol);
    bool atLine(int fromRow,int fromCol,int toRow,int toCol,bool needGap);
    int checkPiece(int row,int col);
    bool checkPiece(int row,int rowcol,int col,bool isrow,bool needGap);
    void initPiece(bool red);
    void initLCD();
    void initTime();
    void reset();
    QString GetName(int n);
    QPoint center(int i){return QPoint(pes[i]->getCol()*INTERVAL+OFFSET,pes[i]->getRow()*INTERVAL+OFFSET);};
    QRect getRect(QPoint center){return QRect(center.rx()-RADIUS,center.ry()-RADIUS,2*RADIUS,2*RADIUS);};

    piece* pes[32];
    QTimer* m_timer;
    QTime* m_time;
    int selectId;
    bool time_out;

private slots:
    void updateTime();
};
#endif // MAINWINDOW_H
