
#include <QApplication>
#include "SelectMode.h"
#include "ChessBoard.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    SelectMode dlg;
    dlg.setWindowTitle("选择游戏方式");
    dlg.setFixedSize(200,120);
    if(dlg.exec()!=QDialog::Accepted)
        return 0;    
    return a.exec();
}
