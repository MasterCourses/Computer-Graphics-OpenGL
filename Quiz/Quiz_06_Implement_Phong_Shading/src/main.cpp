#include "window.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    window w;
    w.resize(w.sizeHint());
    w.setWindowTitle("06_Quiz_Implement_Phong_Shading");
    w.show();
    return a.exec();
}
