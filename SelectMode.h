#ifndef SELECTMODE_H
#define SELECTMODE_H

#include <QDialog>
#include <QPushButton>
#include "ChessBoard.h"
#include "NetChess.h"

class SelectMode:public QDialog
{
    Q_OBJECT
public:
    explicit SelectMode(QWidget* parent=Q_NULLPTR);
    ~SelectMode();

    QPushButton* btn[3];
private:
    ChessBoard* cb;
    NetChess* nc;
private slots:
    void ClickSlot();

};

#endif // SELECTMODE_H
