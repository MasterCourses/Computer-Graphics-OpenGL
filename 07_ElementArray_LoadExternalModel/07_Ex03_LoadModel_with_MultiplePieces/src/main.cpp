#include "window.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    window w;
    w.resize(w.sizeHint());
    w.setWindowTitle("07_Ex03_LoadModel_with_MultiplePieces");
    w.show();
    return a.exec();
}
