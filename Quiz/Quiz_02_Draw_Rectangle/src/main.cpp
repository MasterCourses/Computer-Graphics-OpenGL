#include "window.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    window w;
    w.resize(w.sizeHint());
    w.setWindowTitle("02_Quiz_Draw_Rectangle");
    w.show();
    return a.exec();
}
