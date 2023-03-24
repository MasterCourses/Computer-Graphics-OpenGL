#include "window.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    window w;
    w.resize(w.sizeHint());
    w.setWindowTitle("07_Ex01_ElementArray");
    w.show();
    return a.exec();
}
