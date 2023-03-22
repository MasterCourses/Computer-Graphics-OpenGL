#include "window.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    window w;
    w.resize(w.sizeHint());
    w.setWindowTitle("06_illumination-Ex02_Ambient_and_Diffuse");
    w.show();
    return a.exec();
}
