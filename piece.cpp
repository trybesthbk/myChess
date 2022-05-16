#include "piece.h"


void piece::init(uc n)
{
    if(n<16){
        row=pieceinfo[n].prow;
    }else{
        n-=16;
        row=10-pieceinfo[n].prow;
    }
    col=pieceinfo[n].pcol;
    type=pieceinfo[n].ptype;
    dead=false;
}

