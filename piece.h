#ifndef PIECE_H
#define PIECE_H

enum name{J,S,X,M,C,P,B};

class piece
{
public:
    piece(name tp,int r,int c,bool rd):type(tp),row(r),col(c),dead(false),red(rd){};
    name getType(){return type;};
    int getRow(){return row;};
    int getCol(){return col;};
    void setRow(int r){row=r;};
    void setCol(int c){col=c;};
    bool getDead(){return dead;};
    bool getColor(){return red;};
    void updatePos(int r,int c){row=r,col=c;};
    void goDead(){dead=true;};
private:
    name type;
    int row;
    int col;
    bool dead;
    bool red;
};

#endif // PIECE_H
