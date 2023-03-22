#include "window.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    window w;
    w.resize(w.sizeHint());
    w.setWindowTitle("06_illumination-Ex03_Gouraud_Shading (Ambient+Diffuse+Specular)");
    w.show();
    return a.exec();
}
